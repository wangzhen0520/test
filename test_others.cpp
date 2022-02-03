//
// Created by WANGZHEN on 2019/3/31.
//
#include "test_others.h"

#include <fstream>
#include <iostream>
#include <memory>
#include <vector>


using namespace std;

int praseJPG()
{
    fstream fs("test.jpg", ios::in | ios::binary);
    if (!fs.is_open()) {
        return 0;
    }

    fs.seekg(0, fs.end);      //将读取is指针移到离is.end的0处意思是将此指针移到文件末尾
    int length = fs.tellg();  //返回输入流中的当前字符的位置
    fs.seekg(0, fs.beg);      // 将读取is指针移到离is.beg的0处意思是将此指针移到文件开始

    std::unique_ptr<uint8_t[]> pbData(new uint8_t[length + 5]);  // 多申请5个字节, 是以下判断的时候需要用

    fs.read((char*)pbData.get(), length);
    fs.close();

    cout << "length: " << length << endl;

    bool bStop = false;
    uint8_t* pbStart = pbData.get();
    while (!bStop) {
        uint8_t b1 = pbStart[0];
        uint8_t b2 = pbStart[1];
        uint8_t b3 = pbStart[2];
        uint8_t b4 = pbStart[3];
        uint16_t wMark = (uint16_t)b1 << 8 | b2;
        uint16_t wSize = (uint16_t)b3 << 8 | b4;
        switch (wMark) {
            case 0xFFD8:
                cout << hex << uppercase << wMark << ": "
                     << "SOI (Start of Image)" << endl;
                pbStart += sizeof(wSize);
                break;
            case 0xFFE0:
                cout << hex << uppercase << wMark << ": "
                     << "APP0 => " << wSize << endl;
                pbStart += wSize + sizeof(wSize);
                break;
            case 0xFFE1:
                cout << hex << uppercase << wMark << ": "
                     << "APP1 => " << wSize << endl;
                pbStart += wSize + sizeof(wSize);
                break;
            case 0xFFE2:
                cout << hex << uppercase << wMark << ": "
                     << "APP2 => " << wSize << endl;
                pbStart += wSize + sizeof(wSize);
                break;
            case 0xFFE3:
                cout << hex << uppercase << wMark << ": "
                     << "APP3 => " << wSize << endl;
                pbStart += wSize + sizeof(wSize);
                break;
            case 0xFFE4:
                cout << hex << uppercase << wMark << ": "
                     << "APP4 => " << wSize << endl;
                pbStart += wSize + sizeof(wSize);
                break;
            case 0xFFE5:
                cout << hex << uppercase << wMark << ": "
                     << "APP5 => " << wSize << endl;
                pbStart += wSize + sizeof(wSize);
                break;
            case 0xFFE6:
                cout << hex << uppercase << wMark << ": "
                     << "APP6 => " << wSize << endl;
                pbStart += wSize + sizeof(wSize);
                break;
            case 0xFFE7:
                cout << hex << uppercase << wMark << ": "
                     << "APP7 => " << wSize << endl;
                pbStart += wSize + sizeof(wSize);
                break;
            case 0xFFE8:
                cout << hex << uppercase << wMark << ": "
                     << "APP8 => " << wSize << endl;
                pbStart += wSize + sizeof(wSize);
                break;
            case 0xFFE9:
                cout << hex << uppercase << wMark << ": "
                     << "APP9 => " << wSize << endl;
                pbStart += wSize + sizeof(wSize);
                break;
            case 0xFFDB:
                cout << hex << uppercase << wMark << ": "
                     << "DQT => " << wSize << endl;
                pbStart += wSize + sizeof(wSize);
                break;
            case 0xFFC0:
                cout << hex << uppercase << wMark << ": "
                     << "SOF0 => " << wSize << endl;
                pbStart += wSize + sizeof(wSize);
                break;
            case 0xFFC4:
                cout << hex << uppercase << wMark << ": "
                     << "DHT => " << wSize << endl;
                pbStart += wSize + sizeof(wSize);
                break;
            case 0xFFDA:
                cout << hex << uppercase << wMark << ": "
                     << "SOS => " << wSize << endl;
                pbStart += wSize + sizeof(wSize);
                break;
            case 0xFFD9:
                cout << hex << uppercase << wMark << ": "
                     << "EOI (End of Image) " << endl;
                bStop = true;
                break;
            default: pbStart++; break;
        }
    }

    return 0;
}

int test_others(int argc, char* argv[])
{
    std::cout << "test_others" << std::endl;

#if 0
    vector<int> a;
    int i = 0;
    do {
        cin >> i;
        a.push_back(i);
    } while (getchar() != '\n');

    vector<int>::iterator it = a.begin();
    int n = a.size() - 1;
    int* N = new int[n];
    int H = 0;
    for (int i = 0; it != a.end(); it++) {
        // cout << "%d ", *it);

        if (it < a.end() - 1) {
            N[i++] = *it;
        } else {
            H = *it;
        }
    }

    int alltime = 0;
    for (int k = 1;; ++k) {
        alltime = 0;
        for (int j = 0; j < n; ++j) {
            int m = N[j] / k;
            int t = N[j] % k;
            if (m == 0 || t != 0 || N[j] < k) {
                alltime += 1;
            }
            alltime += m;

            // cout << "k: %d, N[%d]: %d,m : %d, alltime: %d, h: %d\n", k, j, N[j], m,
            // alltime, H);
        }

        if (alltime <= H) {
            cout << "kkkk: %d\n", k);
            alltime = 0;
            break;
        }
    }
#endif

#if 0
    extern int optind, opterr, optopt;
    extern char* optarg;

    int i = 0;
    for (int j = 0; j < argc; ++j) {
        cout << "the %d para is %s\n", j, argv[j]);
    }

    int c = 0;
    while ((c = getopt(argc, argv, "han:")) != EOF) {
        cout << "start to process %d para\n", optind);
        switch (c) {
        case 'h':
            cout << "we get option -h\n");
            break;
        case 'a':
            cout << "we get option -a\n");
            break;
        case 'n':
            cout << "we get option -n, para is %s\n", optarg);
            break;
        case '?':
            cout << "unknown option: %c\n", optopt);
            break;
        default:
            break;
        }
    }
#endif

#if 0
    praseJPG();
#endif
    return 0;
}