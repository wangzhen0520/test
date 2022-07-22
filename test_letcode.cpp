#include "test_letcode.h"

#include "action.h"

using namespace std;

int test_letcode(int argc, char *argv[])
{
    std::cout << "test_letcode" << std::endl;

    for (const auto &func : Singleton<LtcContainer>::instance()->GetAction(ACTION_LTC)) {
        func();
    }

    return 0;
}
