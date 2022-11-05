#ifndef FRAMEWORK_REGISTER_H_
#define FRAMEWORK_REGISTER_H_

#include "factory.h"

#define REGISTER(str, pt) Factory::GetInstance().Register(str, pt);

namespace mynamespace {
class Register {
public:
    Register() {}
    ~Register() {}

    void register_();
private:
    // Factory *pfactory;
};
} // end mynamespace
#endif // FRAMEWORK_REGISTER_H_
