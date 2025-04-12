#ifdef _WIN32
#include "action.h"

#include <windows.h>
#include <stdio.h>

using namespace std;

#define BUFSIZE 256

#define MAX_HEX_BUFFER_LEN (128)
#define TEMP_HEX_BUFFER_LEN (8)

#define filename(x) strrchr(x, '\\') ? (strrchr(x, '\\') + 1) : (strrchr(x, '/') ? strrchr(x, '/') + 1 : x)
#define __FILENAME__S (filename(__FILE__))
#define PrintHex(szData, nLen) LogPrintHex(__FILENAME__S, __FUNCTION__, __LINE__, szData, nLen)

void LogPrintHex(const char *szFile, const char *szFunction, const int nLine, const unsigned char *szData, int nLen)
{
    printf("%s::%s<%d>Start PrintHexBuf nLen[%d]", szFile, szFunction, nLine, nLen);

    int i, j;
    char szTmp[TEMP_HEX_BUFFER_LEN] = {0};
    char szTmp1[MAX_HEX_BUFFER_LEN] = {0};
    char szTmp2[MAX_HEX_BUFFER_LEN] = {0};
    int nLineNum = 16;           /// 每行显示的字符数，默认16个
    int nRows = nLen / nLineNum; /// 整行数

    for (i = 0; i < nRows; i++) {
        sprintf(szTmp1, "\n%04x  ", i * nLineNum); /// 打印行号
        int nNum = i * nLineNum;

        for (j = 0; j < nLineNum; j++) /// 打印本行的16进制数据
        {
            sprintf(szTmp, "%02x ", szData[nNum + j]);
            strcat(szTmp1, szTmp);
        }
        strcpy(szTmp, "  ");
        strcat(szTmp1, szTmp);
        for (j = 0; j < nLineNum; j++) /// 打印本行的字符
        {
            if (szData[nNum + j] <= 32 || szData[nNum + j] >= 127) /// 将不可见字符作为空格输出
            {
                strcpy(szTmp, ".");
                strcat(szTmp2, szTmp);
            } else {
                sprintf(szTmp, "%c", szData[nNum + j]);
                strcat(szTmp2, szTmp);
            }
        }
        printf("%s%s", szTmp1, szTmp2);
        memset(szTmp, 0, TEMP_HEX_BUFFER_LEN);
        memset(szTmp1, 0, MAX_HEX_BUFFER_LEN);
        memset(szTmp2, 0, MAX_HEX_BUFFER_LEN);
    }

    int nLeftLen = nLen % nLineNum;
    if (nLeftLen != 0) {
        sprintf(szTmp1, "\n%04x  ", nLen - nLeftLen);
        for (j = 0; j < nLeftLen; j++) {
            sprintf(szTmp, "%02x ", szData[nLen - nLeftLen + j]);
            strcat(szTmp1, szTmp);
        }

        for (j = nLeftLen; j < nLineNum; j++) {
            strcpy(szTmp, "   ");
            strcat(szTmp1, szTmp);
        }

        strcpy(szTmp, "  ");
        strcat(szTmp1, szTmp);

        for (j = 0; j < nLeftLen; j++) {
            if (szData[nLen - nLeftLen + j] <= 32 || szData[nLen - nLeftLen + j] >= 127) {
                strcpy(szTmp, ".");
                strcat(szTmp2, szTmp);
            } else {
                sprintf(szTmp, "%c", szData[nLen - nLeftLen + j]);
                strcat(szTmp2, szTmp);
            }
        }
        printf("%s%s\n", szTmp1, szTmp2);
    } else {
        printf("\n");
    }
    printf("\nEnd PrintHexBuf\n");
}


HANDLE hComm; // 串口句柄

// 读取线程函数
DWORD WINAPI ReadThread(LPVOID lpParam)
{
    OVERLAPPED osRead = {0};
    char buffer[128];
    DWORD bytesRead;

    // 创建事件对象用于重叠 I/O
    osRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (osRead.hEvent == NULL) {
        fprintf(stderr, "创建事件失败!\n");
        return 1;
    }

    while (1) {
        // 从串口读取数据
        if (ReadFile(hComm, buffer, sizeof(buffer) - 1, &bytesRead, &osRead)) {
            if (bytesRead > 0) {
                buffer[bytesRead] = '\0'; // 确保字符串结束
                printf("接收 %lu 字节: %s\n", bytesRead, buffer);
            }
        } else {
            if (GetLastError() == ERROR_IO_PENDING) {
                // 等待读取完成
                WaitForSingleObject(osRead.hEvent, INFINITE);
                GetOverlappedResult(hComm, &osRead, &bytesRead, FALSE);
                if (bytesRead > 0) {
                    buffer[bytesRead] = '\0'; // 确保字符串结束
                    printf("接收 %lu 字节: %s\n", bytesRead, buffer);
                }
            } else {
                fprintf(stderr, "读取数据失败!\n");
                break;
            }
        }
    }

    CloseHandle(osRead.hEvent);
    return 0;
}

// 写入线程函数
DWORD WINAPI WriteThread(LPVOID lpParam)
{
    OVERLAPPED osWrite = {0};
    const char *dataToSend = "Hello, Serial Port!";
    DWORD bytesWritten;

    // 创建事件对象用于重叠 I/O
    osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (osWrite.hEvent == NULL) {
        fprintf(stderr, "创建事件失败!\n");
        return 1;
    }

    while (1) {
        // 发送数据
        if (WriteFile(hComm, dataToSend, strlen(dataToSend), &bytesWritten, &osWrite)) {
            printf("发送 %lu 字节: %s\n", bytesWritten, dataToSend);
        } else {
            if (GetLastError() == ERROR_IO_PENDING) {
                // 等待写入完成
                WaitForSingleObject(osWrite.hEvent, INFINITE);
                GetOverlappedResult(hComm, &osWrite, &bytesWritten, FALSE);
                printf("发送 %lu 字节: %s\n", bytesWritten, dataToSend);
            } else {
                fprintf(stderr, "发送数据失败!\n");
                break;
            }
        }
        Sleep(2000); // 每两秒发送一次
    }

    CloseHandle(osWrite.hEvent);
    return 0;
}

// 设置串口参数
void SetupCommPort(HANDLE hComm)
{
    DCB dcb;
    memset(&dcb, 0, sizeof(dcb));
    dcb.DCBlength = sizeof(DCB);

    if (!GetCommState(hComm, &dcb)) {
        fprintf(stderr, "获取串口状态失败!\n");
        return;
    }

    dcb.BaudRate = CBR_9600;   // 波特率
    dcb.StopBits = ONESTOPBIT; // 停止位
    dcb.Parity = NOPARITY;     // 奇偶校验位
    dcb.ByteSize = 8;          // 数据位

    if (!SetCommState(hComm, &dcb)) {
        fprintf(stderr, "设置串口状态失败!\n");
        return;
    }

    // 清除缓冲区
    PurgeComm(hComm, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
    EscapeCommFunction(hComm, SETDTR);
}

// 设置串口超时
void SetupCommPortTimeout(HANDLE hComm)
{
    COMMTIMEOUTS timeouts = {0};
    timeouts.ReadIntervalTimeout = 50;        // 设置字符间隔超时
    timeouts.ReadTotalTimeoutConstant = 1024; // 设置总读取超时
    timeouts.WriteTotalTimeoutMultiplier = 2; // 每字节的额外超时乘数
    timeouts.WriteTotalTimeoutConstant = 0;   // 总写入超时常量

    if (!SetCommTimeouts(hComm, &timeouts)) {
        fprintf(stderr, "设置超时失败!\n");
    }
}

void test_uart(int argc, char *argv[])
{
// #define test
#ifdef test
    return;
#else
    cout << __FILE__ << ":" << __func__ << endl;
#endif

    // 打开串口
    hComm = CreateFile("COM3", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
    if (hComm == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "打开串口失败: %lu\n", GetLastError());
        return;
    }

    // 设置串口配置
    SetupCommPort(hComm);
    SetupCommPortTimeout(hComm);

    // 创建接收线程
    HANDLE hReadThread = CreateThread(NULL, 0, ReadThread, NULL, 0, NULL);
    if (hReadThread == NULL) {
        fprintf(stderr, "创建接收线程失败: %lu\n", GetLastError());
        CloseHandle(hComm);
        return;
    }

    // 创建发送线程
    HANDLE hWriteThread = CreateThread(NULL, 0, WriteThread, NULL, 0, NULL);
    if (hWriteThread == NULL) {
        fprintf(stderr, "创建发送线程失败: %lu\n", GetLastError());
        CloseHandle(hReadThread);
        CloseHandle(hComm);
        return;
    }

    // 等待线程结束
    WaitForSingleObject(hReadThread, INFINITE);
    WaitForSingleObject(hWriteThread, INFINITE);

    // 设置串口和清理
    EscapeCommFunction(hComm, CLRDTR);                                               // 清除数据终端就绪信号
    PurgeComm(hComm, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR); // 清除缓冲区
    CloseHandle(hReadThread);
    CloseHandle(hWriteThread);
    CloseHandle(hComm);
}

LTC_REGISTER_ACTION(ACTION_OTHERS, test_uart);

#endif // _WIN32