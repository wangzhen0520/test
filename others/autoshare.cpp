//
// Created by WANGZHEN on 2020/1/5.
//
#include "autoshare.h"

#include <iostream>
#include <memory>
#include <functional>

#include "action.h"

using namespace std;


void func0(std::shared_ptr<int> sp)
{
    std::cout << "fun0:" << sp.use_count() << std::endl;
}
void func1(std::shared_ptr<int>& sp)
{
    std::cout << "fun1:" << sp.use_count() << std::endl;
}

#if 1
// 抽象OSD处理器
class OSDProcessorUnit {
public:
    OSDProcessorUnit() {cout << "OSDProcessorUnit" << endl;}
    virtual ~OSDProcessorUnit() {cout << "~OSDProcessorUnit" << endl;}

    // 处理算法结果数据
    virtual int64_t Process()
    {
        cout << "OSDProcessorUnit Process" << endl;
        return 0;
    }
};

// OSD 处理器
class OSDProcessorDecorate : public std::enable_shared_from_this<OSDProcessorDecorate>, public OSDProcessorUnit {
public:
    OSDProcessorDecorate() : m_pstOSDResultProcessor(nullptr)
    {
        cout << "OSDProcessorDecorate" << endl;
    }
    ~OSDProcessorDecorate() {cout << "~OSDProcessorDecorate" << endl;}
    void SetResultProcessor(const std::shared_ptr<OSDProcessorUnit> &pstOSDResultProcessor)
    {
        if (GetPtr() == pstOSDResultProcessor) {
            cout << "OSDProcessorDecorate the same" << endl;
            return;
        }
        m_pstOSDResultProcessor = pstOSDResultProcessor;
    }

    // 获取指向当前对象的指针
    std::shared_ptr<OSDProcessorUnit> GetPtr()
    {
        return shared_from_this();
    }

    virtual int64_t Process()
    {
        cout << "OSDProcessorDecorate Process" << endl;
        int64_t result = 0;

        if (m_pstOSDResultProcessor != nullptr) {
            result = m_pstOSDResultProcessor->Process();
        }

        return result;
    }
protected:
    // OSDProcessorUnit *m_pstOSDResultProcessor;
    std::shared_ptr<OSDProcessorUnit> m_pstOSDResultProcessor;
};

class CTimeOSDBuilder : public OSDProcessorDecorate {
public:
    CTimeOSDBuilder() {cout << "CTimeOSDBuilder" << endl;}

    ~CTimeOSDBuilder() {cout << "~CTimeOSDBuilder" << endl;}

    virtual int64_t Process() {
        cout << "CTimeOSDBuilder Process" << endl;

        OSDProcessorDecorate::Process();
        return 0;
    }
};

class CPlateIdOSDBuilder : public OSDProcessorDecorate {
public:
    CPlateIdOSDBuilder() {cout << "CPlateIdOSDBuilder" << endl;}

    ~CPlateIdOSDBuilder() {cout << "~CPlateIdOSDBuilder" << endl;}

    virtual int64_t Process() {
        cout << "CPlateIdOSDBuilder Process" << endl;

        OSDProcessorDecorate::Process();
        return 0;
    }
};
#endif


using RegulFunc = std::function<std::shared_ptr<OSDProcessorDecorate>()>;


void test_autoshared(int argc, char *argv[])
{
#define test
#ifdef test
    return;
#else
    std::cout << __FILE__ << ":" << __func__ << std::endl;
#endif

#if 0
    auto sp = std::make_shared<int>(1024);
    std::cout << sp.use_count() << std::endl;

    func0(sp);
    if (sp.get()) {
        std::cout << sp.get() << std::endl;
    }
    sp.reset(new int[100]);
    func1(sp);
    if (sp.get()) {
        std::cout << sp.get() << std::endl;
    }
#endif

#if 0
    std::shared_ptr<OSDProcessorUnit> tmpPtr = nullptr;
    // tmpPtr->Process();

    auto tmpPtr1 = make_shared<CTimeOSDBuilder>();
    cout << "tmpPtr1" << endl << endl;
    auto tmpPtr2 = make_shared<CPlateIdOSDBuilder>();
    cout << "tmpPtr2" << endl << endl;

    // tmpPtr1->SetResultProcessor(tmpPtr1);
    tmpPtr1->SetResultProcessor(tmpPtr2);
    cout << "SetResultProcessor" << endl << endl;

    // tmpPtr2->SetResultProcessor(tmpPtr1);

    tmpPtr = tmpPtr1;
    tmpPtr->Process();
    cout << "tmpPtr1 Process" << endl << endl;
#endif

#if 0
    std::shared_ptr<OSDProcessorUnit> tmpPtr = nullptr;

    REGIST_CONTAINER(int32_t, RegulFunc, 0, []()->std::shared_ptr<OSDProcessorDecorate> {
            return std::make_shared<CTimeOSDBuilder>();
        });

    auto tmpPtr1 = REGIST_CONTAINER_GET(int32_t, RegulFunc, 0)();
    cout << "tmpPtr1" << endl << endl;

    REGIST_CONTAINER(int32_t, RegulFunc, 1, []()->std::shared_ptr<OSDProcessorDecorate> {
            return std::make_shared<CPlateIdOSDBuilder>();
        });

    auto tmpPtr2 = REGIST_CONTAINER_GET(int32_t, RegulFunc, 1)();
    cout << "tmpPtr2" << endl << endl;

    // tmpPtr1->SetResultProcessor(tmpPtr1);
    tmpPtr1->SetResultProcessor(tmpPtr2);
    cout << "SetResultProcessor" << endl << endl;

    // tmpPtr2->SetResultProcessor(tmpPtr1);

    tmpPtr = tmpPtr1;
    tmpPtr->Process();
    cout << "tmpPtr1 Process" << endl << endl;

#endif
}

LTC_REGISTER_ACTION(ACTION_OTHERS, test_autoshared);