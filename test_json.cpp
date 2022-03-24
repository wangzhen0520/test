#include "test_json.h"

#include <iostream>

#include "jsonMerge.h"

int test_json(int argc, char* argv[])
{
    std::cout << "test_json" << std::endl;

#if 0
    std::string srcfile("");
    std::string customefile("");
    if (argc == 3) {
        srcfile = argv[1];
        customefile = argv[2];
    } else {
        std::cout << "help: [cmd] src dst" << std::endl;
        return -1;
    }

    jsonMerge(srcfile, customefile);
#endif

    return 0;
}