#include <cstdio>
#include <cstring>
#include <iostream>
#include <time.h>
#include <stdint.h>

#include <fstream>

#include "action.h"

// #include "miniz.h"

using namespace std;

#define INPUT_BUFFER_LENGTH (1024)

#define WHITE_LIST_MAX (60)

typedef struct
{
    uint32_t dest_addr; // 终端设备地址
    uint8_t floor;      // 部署楼层
    uint8_t room;       // 户号(烟道号)
} __attribute__((packed)) ft_uart_payload_of_destaddr_s;

typedef struct
{
    uint8_t dest_addr_num; // 终端个数
    ft_uart_payload_of_destaddr_s dest_addr_info[WHITE_LIST_MAX];
} __attribute__((packed)) ft_uart_payload_of_whitelist_s;

#define ROUTE_LIST_MAX (12)
typedef struct
{
    uint8_t floor;                        // 部署楼层
    uint8_t room;                         // 户号(烟道号)
    uint32_t dest_addr;                   // 终端设备地址
    uint8_t router_num;                   // 路由器数量
    uint32_t router_addr[ROUTE_LIST_MAX]; // 共 12 级路由器地址, 从上往下排列, 后续没有的填 0 地址
} __attribute__((packed)) ft_uart_payload_of_route_s;

typedef struct
{
    uint32_t gateway_addr;  // 网关地址
    uint8_t route_list_num; // 路由个数
    ft_uart_payload_of_route_s route_info[WHITE_LIST_MAX];
} __attribute__((packed)) ft_uart_payload_of_route_list_s;

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

void hex_to_ascii(char *dst, char *src, int len)
{
    for (int i = 0; i < len; i++) {
        char dh = '0' + ((src[i] & 0xF0) >> 4);
        char dl = '0' + (src[i] & 0x0F);
        if (dh > '9') {
            dh = dh - '9' - 1 + 'a'; // 或者 dh= dh+ 7;
        }
        if (dl > '9') {
            dl = dl - '9' - 1 + 'a'; // 或者dl = dl + 7;
        }
        dst[2 * i] = dh;
        dst[2 * i + 1] = dl;
    }
}

// 商净08F6B64A.png
int calc_sum(unsigned char *data, int len)
{
    unsigned int crc_ret = 0;
    for (int i = 0; i < len; i++) {
        crc_ret += data[i];
    }
    return crc_ret;
}

void get_white_list_from_file(const char *filename, ft_uart_payload_of_whitelist_s *data)
{
    ifstream readFile;
    readFile.open(filename, ios::in);

    int index = 0;
    if (readFile.is_open()) {
        cout << "Sucess open!" << endl;
        string str;
        while (getline(readFile, str)) {
            // cout << str << endl;
            const char *delim = " ";
            char buf[64];
            snprintf(buf, sizeof(buf), "%s", str.c_str());
            char *p = strtok(buf, delim);
            int item = 0;
            while (p) {
                if (item == 0) {
                    data->dest_addr_info[index].room = atoi(p);
                } else if (item == 1) {
                    data->dest_addr_info[index].floor = atoi(p);
                } else {
                    data->dest_addr_info[index].dest_addr = atoi(p);
                }
                p = strtok(NULL, delim);
                item++;
            }
            index++;
        }
    } else {
        cout << "Open Failure!" << endl;
    }
    readFile.close();
    data->dest_addr_num = index;
}

void get_route_list_from_file(const char *filename, ft_uart_payload_of_route_list_s *data)
{
    ifstream readFile;
    readFile.open(filename, ios::in);

    int index = 0;
    if (readFile.is_open()) {
        cout << "Sucess open!" << endl;
        string str;
        while (getline(readFile, str)) {
            // cout << str << endl;
            const char *delim = " ";
            char buf[512];
            snprintf(buf, sizeof(buf), "%s", str.c_str());
            char *p = strtok(buf, delim);
            int item = 0;
            int sub_item_num = 0;
            while (p) {
                if (item == 0) {
                    data->route_info[index].room = atoi(p);
                } else if (item == 1) {
                    data->route_info[index].floor = atoi(p);
                } else if (item == 2) {
                    // data->route_info[index].dest_addr = atoi(p);
                    sscanf(p, "%x", &data->route_info[index].dest_addr);
                } else {
                    // data->route_info[index].router_addr[sub_item_num] = atoi(p);
                    sscanf(p, "%x", &data->route_info[index].router_addr[sub_item_num]);
                    sub_item_num++;
                }
                p = strtok(NULL, delim);
                item++;
            }
            data->route_info[index].router_num = sub_item_num;
            index++;
        }
    } else {
        cout << "Open Failure!" << endl;
    }
    readFile.close();
    data->route_list_num = index;
}

void show_route_info(ft_uart_payload_of_route_list_s *data)
{
    for (uint16_t i = 0; i < data->route_list_num; i++) {
        ft_uart_payload_of_route_s *p = &data->route_info[i];
        printf("%2u %2u %04X ", p->room, p->floor, p->dest_addr);
        for (uint16_t j = 0; j < p->router_num; j++) {
            printf("%04X ", p->router_addr[j]);
        }
        printf("\n");
    }
}

typedef struct
{
    void (*_add)(void *);
    void (*_replace)(void *, void *);
    void (*_remove)(void *);
} function_ops;

void add_to_list(void *param1)
{
    ft_uart_payload_of_destaddr_s *addr = (ft_uart_payload_of_destaddr_s *)param1;
    printf("add addr:%u %u %u\n", addr->room, addr->floor, addr->dest_addr);
}

void replace_to_list(void *param1, void *param2)
{
    ft_uart_payload_of_destaddr_s *addr1 = (ft_uart_payload_of_destaddr_s *)param1;
    ft_uart_payload_of_destaddr_s *addr2 = (ft_uart_payload_of_destaddr_s *)param2;
    printf("replace addr1:%u %u %u addr2:%u %u %u\n", addr1->room, addr1->floor, addr1->dest_addr, addr2->room,
        addr2->floor, addr2->dest_addr);
}

void remove_to_list(void *param1)
{
    ft_uart_payload_of_destaddr_s *addr = (ft_uart_payload_of_destaddr_s *)param1;

    printf("remove addr:%u %u %u\n", addr->room, addr->floor, addr->dest_addr);
}

void process_white_list(ft_uart_payload_of_whitelist_s *arr1, ft_uart_payload_of_whitelist_s *arr2, function_ops *ops)
{
    uint16_t i = 0, j = 0;
    while (i < arr1->dest_addr_num && j < arr2->dest_addr_num) {
        if (arr1->dest_addr_info[i].room > arr2->dest_addr_info[j].room) {
            // remove add1
            // printf("remove1 addr1:%u %u %u add addr2:%u %u %u\n", arr1->dest_addr_info[i].room,
            //     arr1->dest_addr_info[i].floor, arr1->dest_addr_info[i].dest_addr, arr2->dest_addr_info[j].room,
            //     arr2->dest_addr_info[j].floor, arr2->dest_addr_info[j].dest_addr);
            // printf("add addr2:%u %u %u\n", arr2->dest_addr_info[j].room, arr2->dest_addr_info[j].floor,
            //     arr2->dest_addr_info[j].dest_addr);
            if (ops->_add) {
                ops->_add(&arr2->dest_addr_info[j]);
            }
            j++;
        } else if (arr1->dest_addr_info[i].room < arr2->dest_addr_info[j].room) {
            // remove add1
            // printf("remove2 addr1:%u %u %u add addr2:%u %u %u\n", arr1->dest_addr_info[i].room,
            //     arr1->dest_addr_info[i].floor, arr1->dest_addr_info[i].dest_addr, arr2->dest_addr_info[j].room,
            //     arr2->dest_addr_info[j].floor, arr2->dest_addr_info[j].dest_addr);
            // printf("remove2 addr1:%u %u %u \n", arr1->dest_addr_info[i].room, arr1->dest_addr_info[i].floor,
            //     arr1->dest_addr_info[i].dest_addr);
            if (ops->_remove) {
                ops->_remove(&arr1->dest_addr_info[i]);
            }
            i++;
        } else {
            if (arr1->dest_addr_info[i].floor == arr2->dest_addr_info[j].floor) {
                if (arr1->dest_addr_info[i].dest_addr == arr2->dest_addr_info[j].dest_addr) {
                    // the same
                    // printf("same addr1:%u %u %u addr2:%u %u %u\n", arr1->dest_addr_info[i].room,
                    //     arr1->dest_addr_info[i].floor, arr1->dest_addr_info[i].dest_addr,
                    //     arr2->dest_addr_info[j].room, arr2->dest_addr_info[j].floor,
                    //     arr2->dest_addr_info[j].dest_addr);

                    i++;
                    j++;
                } else {
                    // replace
                    // printf("replace addr1:%u %u %u addr2:%u %u %u\n", arr1->dest_addr_info[i].room,
                    //     arr1->dest_addr_info[i].floor, arr1->dest_addr_info[i].dest_addr,
                    //     arr2->dest_addr_info[j].room, arr2->dest_addr_info[j].floor,
                    //     arr2->dest_addr_info[j].dest_addr);
                    if (ops->_replace) {
                        ops->_replace(&arr1->dest_addr_info[i], &arr2->dest_addr_info[j]);
                    }
                    i++;
                    j++;
                }
            } else if (arr1->dest_addr_info[i].floor > arr2->dest_addr_info[j].floor) {
                // remove
                // printf("remove3 addr1:%u %u %u\n", arr1->dest_addr_info[i].room, arr1->dest_addr_info[i].floor,
                //     arr1->dest_addr_info[i].dest_addr);
                if (ops->_remove) {
                    ops->_remove(&arr1->dest_addr_info[i]);
                }
                i++;
            } else {
                // add
                // printf("add2 addr2:%u %u %u\n", arr2->dest_addr_info[j].room, arr2->dest_addr_info[j].floor,
                //     arr2->dest_addr_info[j].dest_addr);
                if (ops->_add) {
                    ops->_add(&arr2->dest_addr_info[j]);
                }
                j++;
            }
        }
    }

    // printf("i:%u j:%u\n", i, j);

    while (i < arr1->dest_addr_num) {
        // printf("remove addr1:%u %u %u\n", arr1->dest_addr_info[i].room, arr1->dest_addr_info[i].floor,
        //     arr1->dest_addr_info[i].dest_addr);
        if (ops->_remove) {
            ops->_remove(&arr1->dest_addr_info[i]);
        }
        i++;
    }

    while (j < arr2->dest_addr_num) {
        // printf("add addr2:%u %u %u\n", arr2->dest_addr_info[j].room, arr2->dest_addr_info[j].floor,
        //     arr2->dest_addr_info[j].dest_addr);
        if (ops->_add) {
            ops->_add(&arr2->dest_addr_info[j]);
        }
        j++;
    }
}

void process_white_list2(ft_uart_payload_of_whitelist_s *arr1, ft_uart_payload_of_whitelist_s *arr2, function_ops *ops)
{
    uint16_t i = 0, j = 0;
    while (i < arr1->dest_addr_num && j < arr2->dest_addr_num) {
        if (arr1->dest_addr_info[i].room > arr2->dest_addr_info[j].room ||
            (arr1->dest_addr_info[i].room == arr2->dest_addr_info[j].room &&
                arr1->dest_addr_info[i].floor < arr2->dest_addr_info[j].floor)) {
            if (ops->_add) {
                ops->_add(&arr2->dest_addr_info[j]);
            }
            j++;
        } else if (arr1->dest_addr_info[i].room < arr2->dest_addr_info[j].room ||
            (arr1->dest_addr_info[i].room == arr2->dest_addr_info[j].room &&
                arr1->dest_addr_info[i].floor > arr2->dest_addr_info[j].floor)) {
            if (ops->_remove) {
                ops->_remove(&arr1->dest_addr_info[i]);
            }
            i++;
        } else if (arr1->dest_addr_info[i].floor == arr2->dest_addr_info[j].floor) {
            if (arr1->dest_addr_info[i].dest_addr != arr2->dest_addr_info[j].dest_addr) {
                if (ops->_replace) {
                    ops->_replace(&arr1->dest_addr_info[i], &arr2->dest_addr_info[j]);
                }
            }
            i++;
            j++;
        }
    }

    while (i < arr1->dest_addr_num) {
        if (ops->_remove) {
            ops->_remove(&arr1->dest_addr_info[i]);
        }
        i++;
    }

    while (j < arr2->dest_addr_num) {
        if (ops->_add) {
            ops->_add(&arr2->dest_addr_info[j]);
        }
        j++;
    }
}

void test_crc(int argc, char *argv[])
{
// #define test
#ifdef test
    return;
#else
    cout << __func__ << endl;
#endif

#if 0
    // uint8_t buf[] = {0xf4, 0xf5, 0x08, 0x01, 0x08, 0x09, 0x10, 0x00, 0x00, 0x00, 0x13}; // 2320
    // uint8_t buf[] = {0xf4, 0xf5, 0x08, 0x01, 0x08, 0x09, 0x11, 0x00, 0x00, 0x00, 0x14}; // 2321
    uint8_t buf[] = {0xf4, 0xf5, 07, 0x01, 0x03, 0x05, 0x12, 0x00, 0xcb, 0xd1}; // 2321
    // uint8_t buf[] = {0xf4, 0xf5, 0x4d, 0x02, 0x08, 0x09, 0x10, 0x00, 0x00, 0x00, 0x83, 0x00, 0x08, 0x14, 0x00, 0x00,
    //     0x00, 0x00, 0x00, 0x50, 0x00, 0x4f, 0x53, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
    //     0x0e, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    //     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x01,
    //     0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x63, 0x00, 0x00, 0xc4};
    uint16_t crc = calc_sum(buf, sizeof(buf) - 1);
    printf("%02x \n", crc & 0xff);

    crc = MODBUS_CRC16_v3(buf, sizeof(buf) - 2);
    printf("%02x %02x\n", crc & 0xff, (crc >> 8) & 0xff);

    char dst[sizeof(buf) * 2 + 1] = {0};
    hex_to_ascii(dst, (char *)buf, sizeof(buf));

    printf("%d %s\n", strlen(dst), dst);
#endif

#if 0
    uint8_t buf[] = {0xf4, 0xf5, 0x00, 0x4d, 0x02, 0x02, 0x09, 0x09, 0x13, 0x00, 0x01, 0x00, 0x03, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x01, 0x01, 0x6d, 0x01, 0x02, 0x2d,
        0x5f, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x1e, 0x00, 0x00, 0x00, 0x10, 0x00, 0x01, 0x00, 0x01, 0x00, 0x02,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf2, 0x01, 0x00, 0x00, 0x00};

    int size = sizeof(buf) / sizeof(buf[0]);

    uint16_t crc = MODBUS_CRC16_v3(buf, size - 2);
    printf("%02x %02x\n", crc & 0xff, (crc >> 8) & 0xff);

    buf[size - 2] = (crc >> 8) & 0xff;
    buf[size - 1] = crc & 0xff;

    hexdump2(buf, size);

#endif

#if 0
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

    char dst[INPUT_BUFFER_LENGTH * 2 + 1] = {0};
    hex_to_ascii(dst, (char *)buf, INPUT_BUFFER_LENGTH);
    // printf("size[%lu] dst: %s\n", strlen(dst), dst);

    uLong src_len = INPUT_BUFFER_LENGTH;
    uLong cmp_len = compressBound(src_len);
    uLong uncomp_len = src_len;
    mz_uint8 *pCmp = (mz_uint8 *)malloc((size_t)cmp_len);
    mz_uint8 *pUncomp = (mz_uint8 *)malloc((size_t)src_len);
    if ((!pCmp) || (!pUncomp)) {
        printf("Out of memory!\n");
        return;
    }

    int cmp_status = compress(pCmp, &cmp_len, (const unsigned char *)buf, src_len);
    if (cmp_status != Z_OK) {
        printf("compress() failed!\n");
        free(pCmp);
        free(pUncomp);
        return;
    }

    printf("Compressed from %u to %u bytes\n", (mz_uint32)src_len, (mz_uint32)cmp_len);
    for (int i = 0; i < cmp_len; i++) {
        printf("%02x ", pCmp[i]);
    }
    printf("\n");

    cmp_status = uncompress(pUncomp, &uncomp_len, pCmp, cmp_len);
    if (cmp_status != Z_OK) {
        printf("uncompress failed!\n");
        free(pCmp);
        free(pUncomp);
        return;
    }
    printf("Decompressed from %u to %u bytes\n", (mz_uint32)cmp_len, (mz_uint32)uncomp_len);
    for (int i = 0; i < uncomp_len; i++) {
        printf("%02x ", pUncomp[i]);
    }
    printf("\n");

    if ((uncomp_len != src_len) || (memcmp(pUncomp, buf, (size_t)src_len))) {
        printf("Decompression failed!\n");
        free(pCmp);
        free(pUncomp);
        return;
    }

    free(pCmp);
    free(pUncomp);

    for (i = 0; i < n; i++)
        fncrc(buf, INPUT_BUFFER_LENGTH);
#endif

#if 0
    ft_uart_payload_of_whitelist_s orig = {};
    ft_uart_payload_of_whitelist_s newer = {};
    get_white_list_from_file("../addr1.txt", &orig);
    get_white_list_from_file("../addr2.txt", &newer);

    for (uint16_t i = 0; i < orig.dest_addr_num; i++) {
        printf("[%u] addr1: %u %u %u\n", i, orig.dest_addr_info[i].room, orig.dest_addr_info[i].floor,
            orig.dest_addr_info[i].dest_addr);
    }
    printf("\n");

    for (uint16_t i = 0; i < newer.dest_addr_num; i++) {
        printf("[%u] addr2: %u %u %u\n", i, newer.dest_addr_info[i].room, newer.dest_addr_info[i].floor,
            newer.dest_addr_info[i].dest_addr);
    }

    printf("\n");

    function_ops funcs = {
        ._add = add_to_list,
        ._replace = replace_to_list,
        ._remove = remove_to_list,
    };
    process_white_list2(&orig, &newer, &funcs);
#endif

#if 0
    ft_uart_payload_of_route_list_s orig = {};
    get_route_list_from_file("../router.txt", &orig);
    show_route_info(&orig);

#endif
}

LTC_REGISTER_ACTION(ACTION_OTHERS, test_crc);
