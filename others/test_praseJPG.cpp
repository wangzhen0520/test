#include "action.h"

#include <fstream>

using namespace std;

void test_praseJPG()
{
#define test
#ifdef test
    return;
#else
    cout << __func__ << endl;
#endif

    fstream fs("test.jpg", ios::in | ios::binary);
    if (!fs.is_open()) {
        return;
    }

    fs.seekg(0, fs.end);      //将读取is指针移到离is.end的0处意思是将此指针移到文件末尾
    int length = fs.tellg();  //返回输入流中的当前字符的位置
    fs.seekg(0, fs.beg);      // 将读取is指针移到离is.beg的0处意思是将此指针移到文件开始

    std::unique_ptr<uint8_t[]> pbData(new uint8_t[length + 5]);  // 多申请5个字节, 是以下判断的时候需要用

    fs.read((char *)pbData.get(), length);
    fs.close();

    cout << "length: " << length << endl;

    bool bStop = false;
    uint8_t *pbStart = pbData.get();
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
}

LTC_REGISTER_ACTION(ACTION_OTHERS, test_praseJPG);