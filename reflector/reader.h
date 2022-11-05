#ifndef FRAMEWORK_READER_H
#define FRAMEWORK_READER_H

#include "framework/processor.h"
#include "framework/factory.h"

namespace mynamespace {
class Reader : public Processor {
public:
    Reader() {}
    ~Reader() {}
    int Process();

private:
    GETCLASSNAME("Reader") // 注意要添加获取类名字符串的宏
};
} // namespace mynamespace
#endif // FRAMEWORK_READER_H
