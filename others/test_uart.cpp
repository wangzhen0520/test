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

int test_uart(int argc, char *argv[])
{
// #define test
#ifdef test
    return;
#else
    cout << __FILE__ << ":" << __func__ << endl;
#endif

    HANDLE hSerial;
    OVERLAPPED overlapped = {0};
    BYTE buffer[BUFSIZE];
    DWORD dwRead;

    // 打开串口
    hSerial = CreateFile("\\\\.\\COM390", GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 0);

    if (hSerial == INVALID_HANDLE_VALUE) {
        printf("Error opening serial port!\n");
        return 1;
    }
    printf("Serial port opened successfully!\n");

    // 设置串口参数
    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    GetCommState(hSerial, &dcbSerialParams);
    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;
    SetCommState(hSerial, &dcbSerialParams);

    // 清空串口
    PurgeComm(hSerial, PURGE_RXCLEAR | PURGE_TXCLEAR);
    SetCommMask(hSerial, EV_RXCHAR | EV_CTS | EV_DSR);

    overlapped.Offset = 0;
    overlapped.OffsetHigh = 0;
    overlapped.hEvent = CreateEvent(0, 1, 0, 0);

    if (overlapped.hEvent == 0) {
        printf("Event creation failed!\n");
        return 1;
    }

    do {
        DWORD dwEvent;

        // 等待串口事件
        if (!WaitCommEvent(hSerial, &dwEvent, &overlapped)) {
            if (GetLastError() == ERROR_IO_PENDING) {
                // 等待异步事件
                WaitForSingleObject(overlapped.hEvent, INFINITE);
                if (!GetOverlappedResult(hSerial, &overlapped, &dwRead, FALSE)) {
                    printf("Error in GetOverlappedResult!\n");
                    continue;
                }
            } else {
                printf("Error in WaitCommEvent!\n");
                continue;
            }
        }

        // 读取数据
        if (!ReadFile(hSerial, &buffer, BUFSIZE, &dwRead, &overlapped)) {
            if (GetLastError() == ERROR_IO_PENDING) {
                // 等待异步读取操作完成
                WaitForSingleObject(overlapped.hEvent, INFINITE);
                if (!GetOverlappedResult(hSerial, &overlapped, &dwRead, FALSE)) {
                    printf("Error in GetOverlappedResult!\n");
                    continue;
                }
            } else {
                printf("Error in ReadFile!\n");
                continue;
            }
        }

        if (dwRead > 0) {
            PrintHex(buffer, dwRead); // 打印十六进制数据

            do {
                DWORD dNoOFBytestoWrite;     // No of bytes to write into the port
                DWORD dNoOfBytesWritten = 0; // No of bytes written to the port
                dNoOFBytestoWrite = dwRead;
                if (!WriteFile(hSerial, buffer, dNoOFBytestoWrite, &dNoOfBytesWritten, &overlapped)) {
                    DWORD err = GetLastError();
                    if (err == ERROR_IO_PENDING) {
                        // 等待异步读取操作完成
                        WaitForSingleObject(overlapped.hEvent, INFINITE);
                        if (!GetOverlappedResult(hSerial, &overlapped, &dwRead, FALSE)) {
                            printf("Error in GetOverlappedResult!\n");
                            continue;
                        }
                    } else {
                        printf("Error:%d in WriteFile!\n", err);
                        continue;
                    }
                }
                break; // 写入成功，跳出循环
            } while (1);
        }
    } while (1);

    // 清理
    CloseHandle(overlapped.hEvent);
    CloseHandle(hSerial);
}

LTC_REGISTER_ACTION(ACTION_OTHERS, test_uart);

#endif // _WIN32