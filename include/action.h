#ifndef ACTION_H
#define ACTION_H

#include "utils.h"

#include <iostream>
#include <functional>
#include <vector>

using ActionFunc = std::function<void()>;

#define LTC_REGISTER_ACTION(func) \
    static RegistAction(REGIST_OCCURRENCES)(func)

class LtcContainer {
public:
    void RegisterAction(ActionFunc func)
    {
        std::unique_lock<std::mutex> _(m_actionMutx);
        m_action.push_back(func);
    }

    std::vector<ActionFunc> &GetAction()
    {
        std::unique_lock<std::mutex> _(m_actionMutx);
        return m_action;
    }

private:
    SINGLETON_DECL(LtcContainer);

private:
    std::mutex m_actionMutx;
    std::vector<ActionFunc> m_action;
};

class RegistAction {
public:
    RegistAction(ActionFunc func)
    {
        Singleton<LtcContainer>::instance()->RegisterAction(func);
    }
    ~RegistAction() {}
};

#define REGIST_VARNAME(base, line)              EVERY_N_VARNAME_CONCAT(base, line)
#define EVERY_N_VARNAME_CONCAT(base, line)      base## line
#define REGIST_OCCURRENCES                      REGIST_VARNAME(XXX, __LINE__)

#endif // ACTION_H
