#ifndef FRAMEWORK_WRITER_H
#define FRAMEWORK_WRITER_H

#include "framework/processor.h"
#include "framework/factory.h"

namespace mynamespace {
class Writer : public Processor {
public:
    Writer() {}
    ~Writer() {}
    int Process();

private:
    GETCLASSNAME("Writer") // 注意要添加获取类名字符串的宏
};
} // namespace mynamespace
#endif // FRAMEWORK_READER_H
