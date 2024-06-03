#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/times.h>
#include <unistd.h>
#include <algorithm>

#include <string.h>

#include <mutex>
#include <string>

#include "Time.h"

namespace {
static bool enableTimezone = true;

/// 每月的天数
static int mday[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

/// 从1月1日起至本月1日的天数
static int monthdays[12] = {
    0,
    31,                                                   // 1
    31 + 28,                                              // 2
    31 + 28 + 31,                                         // 3
    31 + 28 + 31 + 30,                                    // 4
    31 + 28 + 31 + 30 + 31,                               // 5
    31 + 28 + 31 + 30 + 31 + 30,                          // 6
    31 + 28 + 31 + 30 + 31 + 30 + 31,                     // 7
    31 + 28 + 31 + 30 + 31 + 30 + 31 + 31,                // 8
    31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30,           // 9
    31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31,      // 10
    31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30, // 11
};

/// 是否闰年
inline bool is_leap_year(int year)
{
    return ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0);
}

/// 日期规范化
inline void normalize_date(int &year, int &month, int &day)
{
    if (year < 1970) {
        year = 1970;
    } else if (year > 2038) {
        year = 2038;
    }

    if (month <= 0) {
        month = 1;
    } else if (month > 12) {
        month = 12;
    }

    if (day <= 0) {
        day = 1;
    } else if (day > mday[month - 1]) {
        if (!(month == 2 && day == 29 && is_leap_year(year))) {
            day = mday[month - 1];
        }
    }
}

/// 时间规范化
inline void normalize_time1(int &hour, int &minute, int &second)
{
    if (hour < 0) {
        hour = 0;
    } else if (hour > 23) {
        hour = 23;
        minute = 59;
        second = 59;
    }

    if (minute < 0) {
        minute = 0;
    } else if (minute > 59) {
        minute = 59;
    }

    if (second < 0) {
        second = 0;
    } else if (second > 59) {
        second = 59;
    }
}

/// 得到自1970-1-1起的总天数
inline int date_to_days(int year, int month, int day)
{
    // 每年365天 + 润年增加的天数 + 当年天数
    int leap_days = (year - 1968) / 4;
    int days = (year - 1970) * 365 + leap_days + monthdays[month - 1] + (day - 1);
    if (is_leap_year(year) && month < 3) //还没到闰月
    {
        --days;
    }

    return days;
}

/// 计算星期数
inline int date_to_weekday(int year, int month, int day)
{
    int leap_days = (year - 1968) / 4;
    int wdays = (year - 1970) + leap_days + monthdays[month - 1] + (day - 1) + 4;
    if (is_leap_year(year) && month < 3) //还没到闰月
    {
        --wdays;
    }

    return wdays % 7;
}

/// 把自1970-1-1起的天数转换成日期
inline void days_to_date(int days, int &year, int &month, int &day)
{
    year = days / 365 + 1970;
    days %= 365;

    int leap_days = (year - 1 - 1968) / 4;
    bool is_leap = false;
    if (days < leap_days) {
        --year;
        is_leap = is_leap_year(year);
        days += is_leap ? 366 : 365;
    } else {
        is_leap = is_leap_year(year);
    }

    days -= leap_days;

    for (month = 1; month <= 11; ++month) {
        if (month == 3) {
            if (is_leap) {
                if (days == monthdays[2]) {
                    month = 2;
                    break;
                }
                days--;
            }
        }
        if (days < monthdays[month]) {
            break;
        }
    }

    day = days - monthdays[month - 1] + 1;
}

/// 返回本地时间与 GMT 时间相差的秒数; 本地时间 + 相差的秒数 = GMT时间
inline long get_timezone()
{
    if (!enableTimezone)
        return 0;

#if defined(__linux__)
    // timezone是系统提供的全局变量，但是在时区变化时不会自动更新，需要用tzset进行刷新。
    // 但是tzset不是一个线程安全的函数，这里加锁只能保证该应用程序中不会出现并发调用tzset的情况。
    static std::mutex s_mtxtz;
    std::unique_lock<std::mutex> _(s_mtxtz);
    tzset();
    return timezone;
#endif
}

static std::string s_format = "yyyy-MM-dd HH:mm:ss";

static FT::CTime::DateFormat s_dateFormat = FT::CTime::ymd;
static char s_dateSeparator = '-';
static bool s_12hour = false;

} // namespace

namespace FT {
/// SystemTime 规范化 (值校验，生成星期)
void normalize_time(SystemTime &time)
{
    normalize_date(time.year, time.month, time.day);
    time.wday = date_to_weekday(time.year, time.month, time.day);
    normalize_time1(time.hour, time.minute, time.second);
}

/// UTC 转换为 SystemTime
void utc_to_time(uint64_t utc, SystemTime &time)
{
    uint64_t seconds = utc - get_timezone();

    enum
    {
        DAY_SECONDS = 24 * 60 * 60
    };
    int days = int(seconds / DAY_SECONDS);
    days_to_date(days, time.year, time.month, time.day);

    int sec = int(seconds % DAY_SECONDS);
    time.hour = sec / 3600;
    sec %= 3600;
    time.minute = sec / 60;
    time.second = sec % 60;
    time.wday = date_to_weekday(time.year, time.month, time.day);
}

/// SystemTime 转换为 UTC
uint64_t time_to_utc(SystemTime const &time)
{
    int year = time.year, month = time.month, day = time.day;
    normalize_date(year, month, day);
    int days = date_to_days(year, month, day);

    int hour = time.hour, minute = time.minute, second = time.second;
    normalize_time1(hour, minute, second);

    enum
    {
        DAY_SECONDS = 24 * 60 * 60
    };
    uint64_t seconds = uint64_t(days) * DAY_SECONDS + hour * 3600 + minute * 60 + second;
    return seconds + get_timezone();
}

/// 本地时间秒数 转换为 SystemTime
void seconds_to_time(uint64_t localseconds, SystemTime &time)
{
    uint64_t seconds = localseconds;

    enum
    {
        DAY_SECONDS = 24 * 60 * 60
    };
    int days = int(seconds / DAY_SECONDS);
    days_to_date(days, time.year, time.month, time.day);

    int sec = int(seconds % DAY_SECONDS);
    time.hour = sec / 3600;
    sec %= 3600;
    time.minute = sec / 60;
    time.second = sec % 60;
    time.wday = date_to_weekday(time.year, time.month, time.day);
}

/// SystemTime 转换为 本地时间秒数
uint64_t time_to_seconds(SystemTime const &time)
{
    int year = time.year, month = time.month, day = time.day;
    normalize_date(year, month, day);
    int days = date_to_days(year, month, day);

    int hour = time.hour, minute = time.minute, second = time.second;
    normalize_time1(hour, minute, second);

    enum
    {
        DAY_SECONDS = 24 * 60 * 60
    };
    uint64_t seconds = uint64_t(days) * DAY_SECONDS + hour * 3600 + minute * 60 + second;
    return seconds;
}

const CTime CTime::minTime(2000, 1, 1, 0, 0, 0);
const CTime CTime::maxTime(2038, 1, 1, 0, 0, 0);

void setSysCurrentTime(const CTime &time, int toleranceSeconds)
{
    //标识是否更改过系统时间，需要调用回调函数
    bool bNeedCallback = true;
    //标识预设的rtc时钟同步的结果
    bool bPreRtcSync = true;
    CTime temp = time;

    do {
        if (temp < CTime::minTime) {
            temp = CTime::minTime;
        } else if (temp > CTime::maxTime) {
            temp = CTime::maxTime;
        }

        if (toleranceSeconds) {
            if (abs((int)(CTime::getCurrentTime() - temp)) <= toleranceSeconds) {
                bNeedCallback = false;
                break;
            }
        }

        struct timeval tv;

        tv.tv_sec = temp.makeTime();
        tv.tv_usec = 0;

        // 设置系统时间
        printf("CTime::setCurrentTime to %04d-%02d-%02d %02d:%02d:%02d\n", temp.year, temp.month, temp.day, temp.hour,
            temp.minute, temp.second);

        if (settimeofday(&tv, 0) != 0) {
            break;
        }

    } while (0);
}

CTime::CTime()
{
}

CTime::CTime(uint64_t time)
{
    breakTime(time);
}

CTime::CTime(int vyear, int vmonth, int vday, int vhour, int vmin, int vsec)
{
    struct tm t;

    year = vyear;
    t.tm_year = year - 1900;
    month = vmonth;
    t.tm_mon = vmonth - 1;
    day = t.tm_wday = vday;
    hour = t.tm_hour = vhour;
    minute = t.tm_min = vmin;
    second = t.tm_sec = vsec;

    // 值校验，规范化，生成星期
    normalize_time(*this);
}

uint64_t CTime::makeTime() const
{
    return time_to_utc(*this);
}

void CTime::breakTime(uint64_t time)
{
    time_t tt = (time_t)time; // linux下可能是32位，会溢出
    if (time != (uint64_t)tt) {
        printf("CTime::breakTime overflowed!\n");
    }

    utc_to_time(time, *this);
}

CTime CTime::operator+(int64_t seconds) const
{
    CTime time;

    seconds_to_time(time_to_seconds(*this) + seconds, time);
    return time;
}

CTime CTime::operator-(int64_t seconds) const
{
    CTime time;

    seconds_to_time(time_to_seconds(*this) - seconds, time);
    return time;
}

int64_t CTime::operator-(const CTime &time) const
{

    return ((int64_t)time_to_seconds(*this) - (int64_t)time_to_seconds(time));
}

CTime &CTime::operator+=(int64_t seconds)
{

    seconds_to_time(time_to_seconds(*this) + seconds, *this);
    return *this;
}

CTime &CTime::operator-=(int64_t seconds)
{

    seconds_to_time(time_to_seconds(*this) - seconds, *this);
    return *this;
}

bool CTime::operator==(const CTime &time) const
{
    return (second == time.second) && (minute == time.minute) && (hour == time.hour) && (day == time.day) &&
           (month == time.month) && (year == time.year);
}

bool CTime::operator!=(const CTime &time) const
{
    return !operator==(time);
}

bool CTime::operator<(const CTime &time) const
{
    return year < time.year || (year == time.year && month < time.month) ||
           (year == time.year && month == time.month && day < time.day) ||
           (year == time.year && month == time.month && day == time.day && hour < time.hour) ||
           (year == time.year && month == time.month && day == time.day && hour == time.hour && minute < time.minute) ||
           (year == time.year && month == time.month && day == time.day && hour == time.hour && minute == time.minute &&
               second < time.second);
}

bool CTime::operator>(const CTime &time) const
{
    return time.operator<(*this);
}

bool CTime::operator>=(const CTime &time) const
{
    return !operator<(time);
}

bool CTime::operator<=(const CTime &time) const
{
    return !operator>(time);
}

CTime CTime::getCurrentTime()
{
    struct timeval tv;
    struct tm t;
    gettimeofday(&tv, 0);
    localtime_r(&tv.tv_sec, &t);

    CTime r;
    r.year = t.tm_year + 1900;
    r.month = t.tm_mon + 1;
    r.wday = t.tm_wday;
    r.day = t.tm_mday;
    r.hour = t.tm_hour;
    r.minute = t.tm_min;
    r.second = t.tm_sec;
    return r;
}

void CTime::setCurrentTime(const CTime &time, int toleranceSeconds)
{
    setSysCurrentTime(time, toleranceSeconds);
}

uint64_t CTime::getCurrentMilliSecond()
{
    struct timespec tp;
    long ret = clock_gettime(CLOCK_MONOTONIC, &tp);

    uint64_t ms = 0;
    if (ret == 0) {
        ms = (uint64_t)1 * tp.tv_sec * 1000 + tp.tv_nsec / 1000000;
    } else {
        printf("CTime::getCurrentMilliSecond, clock_gettime failed, error : %ld\n", ret);
    }

    return ms;
}

uint64_t CTime::getCurrentMicroSecond()
{
    struct timespec tp;
    long ret = clock_gettime(CLOCK_MONOTONIC, &tp);

    uint64_t us = 0;
    if (ret == 0) {
        us = (uint64_t)1 * tp.tv_sec * 1000 * 1000 + tp.tv_nsec / 1000;
    } else {
        printf("CTime::getCurrentMicroSecond, clock_gettime failed, error : %ld\n", ret);
    }

    return us;
}

//即使设置了格式，也只是按照格式format字符串中的y以及m和d等来进行数值取值，年月日顺序、分隔符、小时制都需要由mask告知是否按照format中进行调整
void CTime::format(char *buf, const char *format, int mask) const
{
    if (buf == NULL) {
        return;
    }

    char temp[8] = {0};
    char tempYear[8] = {0};
    char tempMonth[8] = {0};
    char tempDay[8] = {0};
    const char *str[3] = {0};
    size_t size = strlen(format);

    buf[0] = '\0';

    // 日期字符串格式化
    int ny = 0, nm = 0, nd = 0;
    const char *p = format;
    while (*p) {
        if (*p == 'y') {
            ny++;
        } else if (*p == 'M') {
            nm++;
        } else if (*p == 'd') {
            nd++;
        }
        p++;
    }
    if (ny > 2) {
        tempYear[0] = year / 1000 + '0';
        tempYear[1] = year % 1000 / 100 + '0';
        tempYear[2] = year % 100 / 10 + '0';
        tempYear[3] = year % 10 + '0';
        tempYear[4] = 0;
    } else if (ny > 0) {
        tempYear[0] = year % 100 / 10 + '0';
        tempYear[1] = year % 10 + '0';
        tempYear[2] = 0;
    } else {
        tempYear[0] = '\0';
    }

    if (nm > 0) {
        int si = 0;
        if (month >= 10 || nm > 1) {
            tempMonth[si++] = month / 10 + '0';
        }
        tempMonth[si++] = month % 10 + '0';
        tempMonth[si] = 0;
    } else {
        tempMonth[0] = '\0';
    }

    if (nd > 0) {
        int si = 0;
        if (day >= 10 || nd > 1) {
            tempDay[si++] = day / 10 + '0';
        }
        tempDay[si++] = day % 10 + '0';
        tempDay[si] = 0;
    } else {
        tempDay[0] = '\0';
    }

    // 日期字符串排序
    DateFormat df = s_dateFormat;
    if (mask & fmDateFormat) {
        char const *pos1 = std::find(format, format + size, 'y');
        char const *pos2 = std::find(format, format + size, 'M');
        char const *pos3 = std::find(format, format + size, 'd');

        if (pos1 < pos2 && pos2 < pos3) {
            df = ymd;
        } else if (pos2 < pos3 && pos3 < pos1) {
            df = mdy;
        } else if (pos3 < pos2 && pos2 < pos1) {
            df = dmy;
        } else {
            printf("error format \n");
        }
    }

    if (df == ymd) {
        str[0] = tempYear;
        str[1] = tempMonth;
        str[2] = tempDay;
    } else if (df == mdy) {
        str[0] = tempMonth;
        str[1] = tempDay;
        str[2] = tempYear;
    } else if (df == dmy) {
        str[0] = tempDay;
        str[1] = tempMonth;
        str[2] = tempYear;
    }

    // 去掉无效的日期字符串
    if (str[0][0] == '\0') {
        str[0] = str[1];
        str[1] = str[2];
    } else if (str[1][0] == '\0') {
        str[1] = str[2];
    }

    // 12小时换算
    int is_12hour = (mask & fmHourFormat) ? (std::find(format, format + size, 'h') != format + size) : s_12hour;
    int h = this->hour;
    if (is_12hour) {
        if (h > 12) {
            h -= 12;
        } else if (h == 0) {
            h = 12;
        }
    }

    // 扫描格式字符串，逐步格式化
    int istr = 0;
    for (size_t i = 0; i < size; i++) {
        size_t n = i;
        switch (format[i]) {
        case '-':
        case '.':
        case '/': {
            char ds = (mask & fmSeparator) ? format[i] : s_dateSeparator;
            strncat(buf, &ds, 1);
        } break;

        case 'y':
        case 'M':
        case 'd':
            while (n < size && format[++n] == format[i])
                ;
            strncat(buf, str[istr++], 4);
            i = n - 1;
            break;

        case 'H':
        case 'h': {
            while (n < size && (format[n] == 'h' || format[n] == 'H'))
                ++n;

            int si = 0;
            if (h >= 10 || n > i + 1) {
                temp[si++] = h / 10 + '0';
            }
            temp[si++] = h % 10 + '0';
            temp[si] = 0;
            strncat(buf, temp, 4);
            i = n - 1;

        } break;

        case 'm': {
            while (n < size && format[++n] == 'm')
                ;
            int si = 0;
            if (minute >= 10 || n > i + 1) {
                temp[si++] = minute / 10 + '0';
            }
            temp[si++] = minute % 10 + '0';
            temp[si] = 0;
            strncat(buf, temp, 4);
            i = n - 1;
        } break;

        case 's': {
            while (n < size && format[++n] == 's')
                ;
            int si = 0;
            if (second >= 10 || n > i + 1) {
                temp[si++] = second / 10 + '0';
            }
            temp[si++] = second % 10 + '0';
            temp[si] = 0;
            strncat(buf, temp, 4);
            i = n - 1;
        } break;

        case 't':
            if (is_12hour) {
                while (n < size && format[++n] == 't')
                    ;
                strncat(buf, hour / 12 ? "PM" : "AM", 2);
                i = n - 1;
            } else {
                n = strlen(buf);
                while (n > 0 && buf[n - 1] == ' ') // 删除多余的空格
                {
                    buf[n - 1] = '\0';
                    n--;
                }
            }
            break;

        default:
            strncat(buf, format + i, 1);
            break;
        }
    }
}

bool CTime::parse(const char *buf, const char *format, int mask)
{
    // mask无效，因为必须按照format给出的格式进行解析数据
    int n[6] = {0}; // 存放解析出的数字
    int count = 0;  // 解析出的数字个数
    char const *p = buf;
    size_t size = strlen(format);
    while (count < 6) {
        while (*p >= '0' && *p <= '9') {
            n[count] = n[count] * 10 + (*p - '0');
            ++p;
        }

        ++count;

        while (*p != 0 && (*p < '0' || *p > '9')) {
            ++p;
        }

        if (*p == 0) {
            break;
        }
    }

    // 获取日期格式，年月日的先后顺序
    std::string timeFormat(format);
    size_t pos1 = timeFormat.find('y');
    size_t pos2 = timeFormat.find('M');
    size_t pos3 = timeFormat.find('d');
    if (pos1 < pos2 && pos2 < pos3) {
        year = n[0];
        month = n[1];
        day = n[2];
    } else if (pos2 < pos3 && pos3 < pos1) {
        month = n[0];
        day = n[1];
        year = n[2];
    } else if (pos3 < pos2 && pos2 < pos1) {
        day = n[0];
        month = n[1];
        year = n[2];
    }

    //不支持时分秒的顺序变更
    hour = n[3];
    minute = n[4];
    second = n[5];

    // 是否按12小时换算,存储在CTime中的小时都是24小时的数字，默认必须有如果有hh的话，tt是必须有的
    int is_12hour = (std::find(format, format + size, 'h') != format + size);

    //判断时间是否齐备
    if (count != 6) {
        printf("CTime parser input information is not enough.\n");
        goto parse_error;
    }

    if (is_12hour && hour > 12) {
        printf("CTime parser input hour format is not accord with real value format.\n");
        goto parse_error;
    } else if (is_12hour) {
        if (strstr(buf, "PM") != NULL) {
            if (hour != 12) {
                hour += 12;
            }
        } else if (strstr(buf, "AM") != NULL) {
            if (hour == 12) {
                hour = 0;
            }
        } else {
            printf("12 hours time point is unclear.\n");
            goto parse_error;
        }
    }

    normalize_time(*this);
    return true;

parse_error:
    day = 0;
    month = 0;
    year = 0;
    hour = 0;
    minute = 0;
    second = 0;

    return false;
}

} // namespace FT