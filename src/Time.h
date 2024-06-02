#ifndef __TIME_WRAPPER_H__
#define __TIME_WRAPPER_H__

#include <cstdint>
#include <string>

namespace FT {

/// 时间结构
struct SystemTime
{
    int year;   ///< 年
    int month;  ///< 月，Jan = 1, Feb = 2 ...
    int day;    ///< 日
    int wday;   ///< 星期，Sun = 0, Mon = 1 ...
    int hour;   ///< 时
    int minute; ///< 分
    int second; ///< 秒
    int isdst;  ///< 夏令时标识
};

class CTime : public SystemTime {
public:
    /// 日期顺序格式
    enum DateFormat
    {
        ymd,
        mdy,
        dmy
    };

    /// 格式化模式选项
    enum FormatMask
    {
        fmGeneral = 0,    ///< 只指定是否显示以及起始值
        fmSeparator = 1,  ///< 指定分隔符
        fmDateFormat = 2, ///< 指定年月日顺序
        fmHourFormat = 4, ///< 指定小时制式

        fmAll = fmSeparator | fmDateFormat | fmHourFormat ///< 指定所有格式选项
    };

    /// 缺省构造函数
    CTime();

    /// 相对时间构造函数
    CTime(uint64_t time);

    /// 普通时间构造函数
    CTime(int vyear, int vmonth, int vday, int vhour, int vmin, int vsec);

    /// 得到相对时间，相对时间指从GMT 1970-1-1 00:00:00 到某个时刻经过的秒数
    /// \return 相对时间
    uint64_t makeTime() const;

    /// 分解相对时间
    /// \param time 相对时间
    void breakTime(uint64_t time);

    /// 时间调节
    /// \param seconds 调节的秒数
    /// \return 调节后新的时间对象
    CTime operator+(int64_t seconds) const;

    /// 时间调节
    /// \param seconds 调节的秒数
    /// \return 调节后新的时间对象
    CTime operator-(int64_t seconds) const;

    /// 时间差运算
    /// \param time 相比较的时间
    /// \return 比较的结果，为(*this - time)得到的秒数
    int64_t operator-(const CTime &time) const;

    /// 时间调节
    /// \param seconds 调节的秒数
    /// \return 对象本身
    CTime &operator+=(int64_t seconds);

    /// 时间调节
    /// \param seconds 调节的秒数
    /// \return 对象本身
    CTime &operator-=(int64_t seconds);

    /// 时间比较
    /// \param time 相比较的时间
    /// \return 相等返回true，否则返回false
    bool operator==(const CTime &time) const;

    /// 时间比较
    /// \param time 相比较的时间
    /// \return 不等返回true，否则返回false
    bool operator!=(const CTime &time) const;

    /// 时间比较
    /// \param time 相比较的时间
    /// \return <返回true，否则返回false
    bool operator<(const CTime &time) const;

    /// 时间比较
    /// \param time 相比较的时间
    /// \return <=返回true，否则返回false
    bool operator<=(const CTime &time) const;

    /// 时间比较
    /// \param time 相比较的时间
    /// \return >返回true，否则返回false
    bool operator>(const CTime &time) const;

    /// 时间比较
    /// \param time 相比较的时间
    /// \return >=返回true，否则返回false
    bool operator>=(const CTime &time) const;

    /// 时间格式化
    /// \param buf 字符串缓冲，一定要足够大
    /// \param format 格式化字符串，如"yyyy-MM-dd HH:mm:ss tt"
    /// yy = 年，不带世纪 yyyy = 年，带世纪
    /// M = 非0起始月 MM = 0起始月 MMMM = 月名称
    /// d = 非0起始日 dd = 0起始日
    /// H = 非0起始24小时 HH = 0起始24小时 h = 非0起始12小时 hh = 0起始12小时
    /// tt = 显示上午或下午
    /// \param mask 格式选项，指定日期分隔符，年月日顺序，小时制式是否由统一的格
    /// 式决定。相应位置0，表示使用统一格式，置1，表示使用format指定的格式
    void format(char *buf, const char *format = "yyyy-MM-dd HH:mm:ss", int mask = fmGeneral) const;

    /// 时间字符串解析
    /// \param buf 输入的字符串缓冲
    /// \param format 格式化字符串，暂时只支持默认的"yyyy-MM-dd HH:mm:ss"
    /// \param mask 格式选项，指定日期分隔符，年月日顺序，小时制式是否由统一的格
    /// 式决定。相应位置0，表示使用统一格式，置1，表示使用format指定的格式
    /// \return 是否成功
    bool parse(const char *buf, const char *format = "yyyy-MM-dd HH:mm:ss", int mask = fmGeneral);

    /// 得到本地当前系统时间
    static CTime getCurrentTime();

    /// 设置本地当前系统时间
    /// \param time 新的时间
    /// \param toleranceSeconds 容差，表示容许设置时间和当前差多少秒内不做修改
    static void setCurrentTime(const CTime &time, int toleranceSeconds = 0);

    /// 得到从系统启动到现在的毫秒数
    static uint64_t getCurrentMilliSecond();

    /// 得到从系统启动到现在的微秒数
    static uint64_t getCurrentMicroSecond();

public:
    static const CTime minTime; ///< 有效的最小时间
    static const CTime maxTime; ///< 有效的最大时间
};

};     // namespace FT
#endif // __TIME_WRAPPER_H__