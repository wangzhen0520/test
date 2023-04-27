#include <cstdio>
#include <cstring>
#include <iostream>
#include <time.h>
#include <stdint.h>

#include "action.h"

using namespace std;

#define INPUT_BUFFER_LENGTH (1024)

/*
CRC16_CCITT：
多项式x^16+x^12+x^5+1（0x1021），初始值0x0000，低位在前，高位在后，结果与0x0000异或；

CRC16_CCITT_FALSE：
多项式x^16+x^12+x^5+1（0x1021），初始值0xFFFF，低位在后，高位在前，结果与0x0000异或；

CRC16_XMODEM：
多项式x^16+x^12+x^5+1（0x1021），初始值0x0000，低位在后，高位在前，结果与0x0000异或；

CRC16_X25：
多项式x^16+x^12+x^5+1（0x1021），初始值0x0000，低位在前，高位在后，结果与0xFFFF异或；

CRC16_MODBUS：
多项式x^16+x^15+x^2+1（0x8005），初始值0xFFFF，低位在前，高位在后，结果与0x0000异或；

CRC16_IBM：
多项式x^16+x^15+x^2+1（0x8005），初始值0x0000，低位在前，高位在后，结果与0x0000异或；

CRC16_MAXIM：
多项式x^16+x^15+x^2+1（0x8005），初始值0x0000，低位在前，高位在后，结果与0xFFFF异或；

CRC16_USB：
多项式x^16+x^15+x^2+1（0x8005），初始值0xFFFF，低位在前，高位在后，结果与0xFFFF异或；

CRC16的算法原理：
1. 根据CRC16的标准选择初值CRCIn的值；
2. 将数据的第一个字节与CRCIn高8位异或；
3. 判断最高位，若该位为 0 左移一位，若为 1 左移一位再与多项式Hex码异或；
4. 重复3直至8位全部移位计算结束；
5. 重复将所有输入数据操作完成以上步骤，所得16位数即16位CRC校验码。
*/

static uint16_t MODBUS_CRC16_v1(const unsigned char *buf, unsigned int len)
{
    uint16_t crc = 0xFFFF;
    char i = 0;

    while (len--) {
        crc ^= (*buf++);

        for (i = 0; i < 8; i++) {
            if (crc & 1) {
                crc >>= 1;
                crc ^= 0xA001;
            } else {
                crc >>= 1;
            }
        }
    }

    return crc;
}

static uint16_t MODBUS_CRC16_v2(const unsigned char *buf, unsigned int len)
{
    static const uint16_t table[2] = {0x0000, 0xA001};
    uint16_t crc = 0xFFFF;
    char bit = 0;
    unsigned int _xor = 0;

    while (len--) {
        crc ^= (*buf++);

        for (bit = 0; bit < 8; bit++) {
            _xor = crc & 1;
            crc >>= 1;
            crc ^= table[_xor];
        }
    }

    return crc;
}

static uint16_t MODBUS_CRC16_v3(const unsigned char *buf, unsigned int len)
{
    static const uint16_t table[256] = {0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241, 0xC601, 0x06C0,
        0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440, 0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
        0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841, 0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1,
        0xDA81, 0x1A40, 0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41, 0x1400, 0xD4C1, 0xD581, 0x1540,
        0xD701, 0x17C0, 0x1680, 0xD641, 0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040, 0xF001, 0x30C0,
        0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240, 0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
        0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41, 0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1,
        0xF881, 0x3840, 0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41, 0xEE01, 0x2EC0, 0x2F80, 0xEF41,
        0x2D00, 0xEDC1, 0xEC81, 0x2C40, 0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640, 0x2200, 0xE2C1,
        0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041, 0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
        0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441, 0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0,
        0x6E80, 0xAE41, 0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840, 0x7800, 0xB8C1, 0xB981, 0x7940,
        0xBB01, 0x7BC0, 0x7A80, 0xBA41, 0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40, 0xB401, 0x74C0,
        0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640, 0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
        0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241, 0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1,
        0x9481, 0x5440, 0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40, 0x5A00, 0x9AC1, 0x9B81, 0x5B40,
        0x9901, 0x59C0, 0x5880, 0x9841, 0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40, 0x4E00, 0x8EC1,
        0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41, 0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
        0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040};

    uint8_t _xor = 0;
    uint16_t crc = 0xFFFF;

    while (len--) {
        _xor = (*buf++) ^ crc;
        crc >>= 8;
        crc ^= table[_xor];
    }

    return crc;
}

static uint16_t MODBUS_CRC16_v4(const unsigned char *buf, unsigned int len)
{
    static const uint16_t table[16] = {0x0000, 0xcc01, 0xd801, 0x1400, 0xf001, 0x3c00, 0x2800, 0xe401, 0xa001, 0x6c00,
        0x7800, 0xb401, 0x5000, 0x9c01, 0x8801, 0x4400};

    uint16_t crc = 0xFFFF;

    while (len--) {
        crc = table[((*buf) ^ crc) & 0xF];
        crc ^= (crc >> 4);

        crc = table[(((*buf++) >> 4) ^ crc) & 0xF];
        crc ^= (crc >> 4);
    }

    return crc;
}

static void get_random_buffer(unsigned char *buf, unsigned int len)
{
    unsigned int i = 0;
    srand(time(NULL));
    for (i = 0; i < len; i++)
        buf[i] = rand() % 256;
}

void test_crc(int argc, char *argv[])
{
#define test
#ifdef test
    return;
#else
    cout << __func__ << endl;
#endif

    unsigned long i = 0;
    unsigned long n = 0;
    unsigned char buf[INPUT_BUFFER_LENGTH] = {0};
    uint16_t (*fncrc)(const unsigned char *, unsigned int);

    if (argc != 3) {
        fprintf(stderr, "Syntax: %s <ALGORITHM> <NUM_TESTS>\n", argv[0]);
        return;
    }

    switch (atoi(argv[1])) {
    case 1:
        fncrc = MODBUS_CRC16_v1;
        break;
    case 2:
        fncrc = MODBUS_CRC16_v2;
        break;
    case 3:
        fncrc = MODBUS_CRC16_v3;
        break;
    case 4:
        fncrc = MODBUS_CRC16_v4;
        break;

    default:
        fprintf(stderr, "Invalid Algorithm, (expected: 1, 2, 3 or 4)\n");
        return;
    }

    n = atol(argv[2]);

    get_random_buffer(buf, INPUT_BUFFER_LENGTH);

    for (int i = 0; i < sizeof(buf); i++) {
        printf("%02x ", buf[i]);
    }
    printf("\n");

    char buf2[INPUT_BUFFER_LENGTH * 2 + 1] = {0};
    for (int i = 0; i < sizeof(buf); i++) {
        char dh = '0' + ((buf[i] & 0xF0) >> 4);
        char dl = '0' + (buf[i] & 0x0F);
        if (dh > '9') {
            dh = dh - '9' - 1 + 'A'; // 或者 dh= dh+ 7;
        }
        if (dl > '9') {
            dl = dl - '9' - 1 + 'A'; // 或者dl = dl + 7;
        }
        buf2[2 * i] = dh;
        buf2[2 * i + 1] = dl;
    }

    printf("size[%lu] buf2: %s\n", strlen(buf2), buf2);

    for (i = 0; i < n; i++)
        fncrc(buf, INPUT_BUFFER_LENGTH);
}

LTC_REGISTER_ACTION(ACTION_OTHERS, test_crc);
