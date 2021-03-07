cmake_minimum_required(VERSION 3.10)

project(test)

# 这段代码可以区分操作系统
MESSAGE("Identifying the OS...")
if(WIN32)
  MESSAGE("This is Windows.")
elseif(APPLE)
  MESSAGE("This is MacOS.")
elseif(UNIX)
  MESSAGE("This is Linux.")
endif()
# 这段代码可以区分操作系统

set(CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

# add_definitions(-DDEBUG) # 相当于代码中 #define DEBUG

# 设置目录
SET(JSON_PATH ${CMAKE_CURRENT_SOURCE_DIR}/jsoncpp-0.y.z)

# 头文件包含目录
INCLUDE_DIRECTORIES(
  .
  ${JSON_PATH}
  )

# 添加源代码文件夹, 自动扫描所有文件
AUX_SOURCE_DIRECTORY(. DIR_SRCS)
AUX_SOURCE_DIRECTORY(${JSON_PATH} DIR_SRCS)

add_executable(test ${DIR_SRCS})
