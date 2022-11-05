#ifndef FRAMEWORK_PROCESSOR_H_
#define FRAMEWORK_PROCESSOR_H_

#include <iostream>

namespace mynamespace {
class Processor {
public:
    Processor() {}
    virtual ~Processor() {}
    virtual int Process() = 0; // 返回错误码,返回0表示处理器处理成功，不为0都表示不成功
private:
};
} // namespace mynamespace
#endif
