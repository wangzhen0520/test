#include "framework/register.h"

#include "reader.h"
#include "writer.h"

namespace mynamespace {
void Register::register_()
{
    /* *
     * 处理器具体实现的注册方法，注册到工厂里面
     * TODO :类注册后期放到配置文件中
     */
    REGISTER(Reader::ClassName(), new Reader());
    REGISTER(Writer::ClassName(), new Writer());
}
} // end mynamespace
