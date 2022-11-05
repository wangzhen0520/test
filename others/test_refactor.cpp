#include "action.h"

#include "framework/register.h"

#include "reader.h"
#include "writer.h"

using namespace std;
using namespace mynamespace;

void test_refactor()
{
#define test
#ifdef test
    return;
#else
    cout << __FILE__ << ":" << __func__ << endl;
#endif

    Register rg;
    rg.register_();

    // Factory ft;
    
    Reader *prd = (Reader *)Factory::GetInstance().Create("Reader"); // 反射的使用
    if (prd) {
        prd->Process();                    // 多态
    }

    Writer *pwt = (Writer *)Factory::GetInstance().Create("Writer"); // 反射的使用
    if (pwt) {
        pwt->Process();                  // 多态
    }
}

LTC_REGISTER_ACTION(ACTION_OTHERS, test_refactor);
