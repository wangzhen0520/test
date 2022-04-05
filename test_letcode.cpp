#include "test_letcode.h"

#include <iostream>

#include "action.h"

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

    extern void test_tree_104();
    test_tree_104();

    extern void test_tree_114();
    test_tree_114();

    extern void test_tree_654();
    test_tree_654();

    extern void test_tree_105_106();
    test_tree_105_106();

    extern void test_tree_537();
    test_tree_537();

    extern void test_tree_55();
    test_tree_55();

    extern void test_tree_198();
    test_tree_198();

    extern void test_tree_73();
    test_tree_73();

    extern void test_tree_74();
    test_tree_74();

    extern void test_tree_79();
    test_tree_79();

    extern void test_tree_34();
    test_tree_34();

    extern void test_tree_153();
    test_tree_153();

    extern void test_tree_162();
    test_tree_162();

    extern void test_tree_82();
    test_tree_82();

    for (const auto &iter : Singleton<LtcContainer>::instance()->GetAction()) {
        iter();
    }
    
    return 0;
}
