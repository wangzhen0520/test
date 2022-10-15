#ifndef UTILS_H
#define UTILS_H

#include <mutex>
#include <memory>
#include <iostream>
#include <sys/time.h>

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

class TimeCost
{
private:
    struct timeval start_;
    struct timeval end_;
public:
    TimeCost() {
        gettimeofday(&start_, NULL);
    };
    void OperateCost(const std::string &opr) {
        gettimeofday(&end_, NULL);
        int cost = (end_.tv_sec - start_.tv_sec) * 1000 + (end_.tv_usec - start_.tv_usec) / 1000;
        std::cout << opr << " cost time: " << cost << "ms" << std::endl;
    }
    ~TimeCost() {
        gettimeofday(&end_, NULL);
        int cost = (end_.tv_sec - start_.tv_sec) * 1000 + (end_.tv_usec - start_.tv_usec) / 1000;
        std::cout << "cost time: " << cost << "ms" << std::endl;
    };
};

#endif  // UTILS_H
