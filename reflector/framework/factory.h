#pragma once
#ifndef FRAMEWORK_FACTORY_H_
#define FRAMEWORK_FACTORY_H_

#include <map>
#include <string>
#include <iostream>
#include <cstring>

#include "processor.h"

namespace mynamespace {
/**
 * 处理器类名与实例的映射 (class_map_) 的Key的比较
 */
struct string_cmp {
    bool operator () (const std::string &a, const std::string &b) const
    {
        return strcmp(a.c_str(), b.c_str()) < 0;
    }
};

class Factory {
public:
    Factory()
    {
        class_map_.clear();
    }
    ~Factory() {}

    /* *
     * 注册处理器
     */
    void Register(const std::string &className, Processor *processor)
    {
        auto iter_ = class_map_.find(className);  // 先查找map中是否存在处理器
        if (iter_ == class_map_.end()) { // 如果不存在，则插入key-value对
            class_map_.insert(std::pair<std::string, Processor *>(className, processor));
        } else { // 如果存在，则更新key对应的value值
            iter_->second = processor;
        }
    }

    /* *
     * 获得处理器实例
     */
    Processor *Create(const std::string &className)
    { // 通过类名的字符串，返回对应类的对象
        auto it = class_map_.find(className);
        if (it == class_map_.end()) {
            std::cout << "In factory, " << className << " object is no exist." << std::endl;
            return nullptr;
        } else {
            return it->second;
        }
    }

    void Destroy()
    { // 销毁工厂中的处理器对象
        for (iter_ = class_map_.begin(); iter_ != class_map_.end(); iter_++) {
            if (iter_->second) {
                delete (iter_->second);
                (iter_->second) = nullptr;
            }
        }
        class_map_.clear();
    }

    static Factory &GetInstance() {
        static Factory pfactory;
        return pfactory;
    }
private:
    /* *
     * 处理器类名与实例的映射
     */
    std::map<std::string, Processor *, string_cmp> class_map_;
    std::map<std::string, Processor *, string_cmp>::iterator iter_;
};

/**
 * 在具体实现类的头文件添加这个宏，获取类名的方法
 */
#define GETCLASSNAME(str_class_name) \
public:                              \
    static std::string ClassName()   \
    {                                \
        return str_class_name;       \
    }
} // namespace mynamespace

#endif // FRAMEWORK_FACTORY_H_
