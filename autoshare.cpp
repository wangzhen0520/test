//
// Created by WANGZHEN on 2020/1/5.
//

#include <iostream>
#include <memory>

void func0(std::shared_ptr<int> sp)
{
    std::cout << "fun0:" << sp.use_count() << std::endl;
}
void func1(std::shared_ptr<int>& sp)
{
    std::cout << "fun1:" << sp.use_count() << std::endl;
}

int autoshared(int argc, char* argv[])
{
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

    return 0;
}