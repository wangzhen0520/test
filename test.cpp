#include <iostream>

#include "test.h"

#include "test_json.h"
#include "test_letcode.h"
#include "test_others.h"

int main(int argc, char* argv[])
{
    std::cout << "Hello, World!" << std::endl;

    (void)test_letcode(argc, argv);

    (void)test_others(argc, argv);

    (void)test_json(argc, argv);

    return 0;
}