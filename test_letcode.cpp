#include "test_letcode.h"

#include <iostream>

using namespace std;

int test_letcode(int argc, char* argv[])
{
    std::cout << "test_letcode" << std::endl;

    extern void test_substring();
    test_substring();

    extern void test_listNode();
    test_listNode();

    extern void test_twosum();
    test_twosum();

    extern void test_trap();
    test_trap();

    extern void test_maxArea();
    test_maxArea();

    extern void test_uf1();
    test_uf1();

    extern void test_uf2();
    test_uf2();

    return 0;
}