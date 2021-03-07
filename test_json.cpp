#include <fstream>
#include <iostream>
#include <algorithm>
#include <string>

#include "test_json.h"

int write_json(std::string& filename, Json::Value& root)
{
    std::ofstream out(filename);
    if (!out.is_open()) {
        std::cout << "write file: " << filename << "] failed" << std::endl;
        return -1;
    }

    Json::StyledWriter write;
    out << write.write(root);
    out.close();

    return 0;
}

int read_json(std::string& filename, Json::Value& root)
{
    std::ifstream in(filename);
    if (!in.is_open()) {
        std::cout << "open file: [" << filename << "] failed" << std::endl;
        return -1;
    }

    Json::Reader read;
    if (!read.parse(in, root)) {
        std::cout << "parse file: [" << filename << "] failed" << std::endl;
        in.close();
        return -1;
    }

    in.close();
    return 0;
}

std::string getFirstGradeConfigName(const char* name)
{
    std::string findStr = ".[]";
    std::string retVal = name;

    std::string::iterator iter = std::find_first_of(retVal.begin(), retVal.end(),
        findStr.begin(), findStr.end());
    if (iter != retVal.end()) {
        retVal = std::string(retVal.begin(), iter);
    }
    return retVal;
}

// 合并两个Json文件
void replaceConfig(Json::Value& dest, const Json::Value& src, bool fillEmptyOnly)
{
    if (src.type() == Json::arrayValue) {
        unsigned int size = src.size();

        for (unsigned int index = 0; index < size; ++index) {
            replaceConfig(dest[index], src[index]);
        }
    } else if (src.type() == Json::objectValue) {
        Json::Value::Members members(src.getMemberNames());
        for (Json::Value::Members::iterator it = members.begin();
             it != members.end();
             ++it) {
            const std::string& name = *it;

            replaceConfig(dest[name], src[name]);
        }
    } else if (!fillEmptyOnly || dest.type() == Json::nullValue) {
        dest = src;
    }
}

int jsonMerge(std::string &srcfile, std::string &customefile)
{
    Json::Value m_configAll = Json::nullValue;
    int ret = read_json(srcfile, m_configAll);
    if (ret != 0) {
        std::cout << "jsonMerge read: [" << srcfile << "] failed" << std::endl;
        return ret;
    }
    // std::cout << m_configAll << std::endl;

    Json::Value m_configCustom = Json::nullValue;
    ret = read_json(customefile, m_configCustom);
    if (ret != 0) {
        std::cout << "jsonMerge read: [" << customefile << "] failed" << std::endl;
        return ret;
    }
    // std::cout << m_configCustom << std::endl;

    Json::Value::Members members(m_configCustom.getMemberNames());
    for (Json::Value::Members::iterator it = members.begin(); it != members.end(); ++it) {
        const std::string& name = *it;
        // std::cout << name << std::endl;

        Json::Path configPath(name);
        Json::Value& customConfig = configPath.make(m_configCustom);
        Json::Value& allConfig = configPath.make(m_configAll);

        replaceConfig(allConfig, customConfig);
    }

    // srcfile = "./test1.txt";
    ret = write_json(srcfile, m_configAll);
    if (ret != 0) {
        std::cout << "jsonMerge write: [" << srcfile << "] failed" << std::endl;
        return ret;
    }

    std::cout << "jsonMerge: [" << srcfile << "] success" << std::endl;
    return 0;
}

// 配置替换流程条件为：
// 1、默认配置类型发生变化，且当前配置不为空
// 2、当前配置空，直接替换
// 3、Object类型，成员项递归执行替换流程
// 4、array类型，长度发生变化，直接替换
// 5、array类型，长度一致，成员递归执行替换流程
bool XorReplaceConfig(Json::Value& result, const Json::Value& table1, Json::Value& table2)
{
    if (table1.type() != table2.type()) {
        if (table1.type() == Json::nullValue) {
            return (result.type() == Json::nullValue ? false : true);
        }
        if (!(table1.type() == Json::uintValue && table2.type() == Json::intValue)) {
            result = Json::Value::null; //需要增加这个函数调用，防止部分json表由于类型不一致，引起死机
            result = table1;

            table2 = table1;
        }
        return ((result.type() == Json::nullValue) && (table1 != Json::nullValue) && (table2 != Json::nullValue) ? false : true);
    }

    if (result.type() == Json::nullValue) {
        if (table1 != table2) {
            result = table1;

            table2 = table1;
        }
    } else if (table1.type() == Json::arrayValue) {
        if (table1.size() != table2.size()) {
            result = table1;

            table2 = table1;

            return ((result.type() == Json::nullValue) && (table1 != Json::nullValue) && (table2 != Json::nullValue) ? false : true);
        }

        bool ret;
        unsigned int resIndex = 0;
        unsigned index = 0;
        for (index = 0; index < table1.size(); ++index) {
            ret = XorReplaceConfig(result[resIndex], table1[index], table2[index]);
            if (ret) {
                ++resIndex;
            }
        }

        if (resIndex < table1.size()) { //若有效结果少于数组长度
            result.resize(resIndex);
        }
    } else if (table1.type() == Json::objectValue) { //Object 类型若是删减配置项，不会影响当前配置，不将其删除。
        bool ret;
        Json::Value::Members members(table1.getMemberNames());
        for (Json::Value::Members::iterator it = members.begin(); it != members.end(); ++it) {
            const std::string& name = *it;
            ret = XorReplaceConfig(result[name], table1[name], table2[name]);
            if (!ret) { //递归后若为Json::nullValue，则将该member删除
                result.removeMember(name);
            }
        }
        if (result.size() == 0) { //Object递归后若没有成员，则将该Object置null,供上层删除此成员
            result = Json::nullValue;
        }
    }

    return ((result.type() == Json::nullValue) && (table1 != Json::nullValue) && (table2 != Json::nullValue) ? false : true);
}

int test_json(int argc, char* argv[])
{
    std::cout << "test_json" << std::endl;

#if 1
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