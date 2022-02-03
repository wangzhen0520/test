#ifndef TEST_JSON_H
#define TEST_JSON_H

#include "json/json.h"

int test_json(int argc, char* argv[]);

/// 用src配置项替换dest相应配置项
/// \param fillEmptyOnly true-只替换dest中为空，src中不为空的项，false-替换所有项
/// 注意：此函数只增加、修改dest配置项，但不会删减dest中配置项。
void replaceConfig(Json::Value& dest, const Json::Value& src, bool fillEmptyOnly = false);

#endif  // TEST_JSON_H