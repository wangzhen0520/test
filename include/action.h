#ifndef ACTION_H
#define ACTION_H

#include "utils.h"

#include <iostream>
#include <functional>
#include <vector>
#include <map>

using ACTION_TYPE = enum {
    ACTION_KS,
    ACTION_LTC,
    ACTION_OTHERS,
};

using ActionFunc = std::function<void()>;

#define LTC_REGISTER_ACTION(type, func) \
    static RegistAction(REGIST_OCCURRENCES)(type, func)

class LtcContainer {
public:
    void RegisterAction(ACTION_TYPE type, ActionFunc func)
    {
        std::unique_lock<std::mutex> _(m_actionMutx);
        m_action[type].push_back(func);
    }

    std::vector<ActionFunc> &GetAction(ACTION_TYPE type)
    {
        std::unique_lock<std::mutex> _(m_actionMutx);
        return m_action[type];
    }

private:
    SINGLETON_DECL(LtcContainer);

private:
    std::mutex m_actionMutx;
    std::map<ACTION_TYPE, std::vector<ActionFunc>> m_action;
};

class RegistAction {
public:
    RegistAction(ACTION_TYPE type, ActionFunc func)
    {
        Singleton<LtcContainer>::instance()->RegisterAction(type, func);
    }
    ~RegistAction() {}
};

#define REGIST_VARNAME(base, line)              EVERY_N_VARNAME_CONCAT(base, line)
#define EVERY_N_VARNAME_CONCAT(base, line)      base## line
#define REGIST_OCCURRENCES                      REGIST_VARNAME(XXX, __LINE__)

#endif // ACTION_H
