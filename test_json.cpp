#include "test_json.h"

#include <iostream>

// #include "jsonMerge.h"
#include "json.h"

int test_json(int argc, char *argv[])
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

#if 1
    struct json_object *new_obj;
    new_obj = json_tokener_parse("[\"abc\",null,\"def\",12]");
    std::cout << "new_obj_.to.string()=" << json_object_to_json_string(new_obj) << std::endl;
    json_object_put(new_obj);
#endif

    return 0;
}