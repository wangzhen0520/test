#ifndef UTILS_H
#define UTILS_H

#include <sys/time.h>
#include <mutex>
#include <memory>
#include <string>
#include <iostream>

template <typename T>
class Singleton {
public:
    template <typename... Args>
    static inline std::shared_ptr<T> instance(Args &&...args)
    {
        // using double-check-lock.
        if (!instance_.get()) {
            std::unique_lock<std::mutex> _(instanceMutex_);
            if (!instance_.get()) {
                instance_.reset(new T(std::forward<Args>(args)...));
            }
        }
        return instance_;
    }

private:
    Singleton() = default;
    virtual ~Singleton() = default;
    Singleton(const Singleton &) = default;
    Singleton &operator=(const Singleton &) = delete;

    // instance
    static std::shared_ptr<T> instance_;
    static std::mutex instanceMutex_;
};

template <typename T>
std::shared_ptr<T> Singleton<T>::instance_;

template <typename T>
std::mutex Singleton<T>::instanceMutex_;

#define SINGLETON_DECL(type)            \
    friend class std::shared_ptr<type>; \
    friend class Singleton<type>;

class TimeCost {
public:
    explicit TimeCost(const std::string &funcName)
        : m_funcName(funcName), m_start{}, m_end{}, m_operStart{}, m_operEnd{}, m_interval(0)
    {
        gettimeofday(&m_start, nullptr);
        m_operStart = m_start;
    }

    TimeCost(const std::string &funcName, uint32_t interval)
        : m_funcName(funcName), m_start{}, m_end{}, m_operStart{}, m_operEnd{}, m_interval(interval)
    {
        gettimeofday(&m_start, nullptr);
        m_operStart = m_start;
    }

    ~TimeCost()
    {
        gettimeofday(&m_end, nullptr);

        int64_t cost = (int64_t)((m_end.tv_sec - m_start.tv_sec) * 1000) + (m_end.tv_usec - m_start.tv_usec) / 1000;

        if (cost > m_interval) {
            std::cout << m_funcName << " cost:" << cost << " ms" << std::endl;
        }
    }

    /*
     * 打印单个操作的耗时
     * @param operatorName 操作名称
     */
    void OperatorCost(const std::string &operatorName)
    {
        gettimeofday(&m_operEnd, nullptr);

        int64_t cost = (int64_t)((m_operEnd.tv_sec - m_operStart.tv_sec) * 1000) +
                       (m_operEnd.tv_usec - m_operStart.tv_usec) / 1000;

        if (cost > m_interval) {
            std::cout << m_funcName << " " << operatorName << " cost:" << cost << " ms" << std::endl;
        }

        m_operStart = m_operEnd;
    }

private:
    std::string m_funcName;
    struct timeval m_start;
    struct timeval m_end;
    struct timeval m_operStart;
    struct timeval m_operEnd;
    uint32_t m_interval;  // 超时打印
};

void hexdump(const unsigned char *p, unsigned int len);
void hexdump2(const unsigned char *p, unsigned int len);

#endif  // UTILS_H
