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

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

get_filename_component(CURDIR ${CMAKE_CURRENT_SOURCE_DIR} NAME ABSOLUTE)
set(TRAGET_NAME ${CURDIR})

# add_definitions(-DDEBUG) # 相当于代码中 #define DEBUG

# 设置目录
SET(JSON_PATH ${CMAKE_CURRENT_SOURCE_DIR}/jsoncpp-0.y.z)
SET(JSON_C_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build/_deps/json-c-src/)
SET(JSON_MERGE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/jsonmerge)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/)

# 头文件包含目录
INCLUDE_DIRECTORIES(
  .
  ./include
  ${JSON_PATH}
  ${JSON_C_PATH}
  ${JSON_MERGE_PATH}
  ${CMAKE_CURRENT_SOURCE_DIR}/build/_deps/json-c-build/
  ${CMAKE_CURRENT_SOURCE_DIR}/reflction
  ${CMAKE_CURRENT_SOURCE_DIR}/reflction/framework
)

# 添加源代码文件夹, 自动扫描所有文件
AUX_SOURCE_DIRECTORY(. DIR_SRCS)
AUX_SOURCE_DIRECTORY(./leetcode DIR_SRCS)
AUX_SOURCE_DIRECTORY(./jsonmerge DIR_SRCS)
AUX_SOURCE_DIRECTORY(./reflector DIR_SRCS)
AUX_SOURCE_DIRECTORY(./reflector/framework DIR_SRCS)
AUX_SOURCE_DIRECTORY(./others DIR_SRCS)

include(FetchContent)
# FetchContent_Declare(json-c
#   GIT_REPOSITORY https://github.com/json-c/json-c.git
#   GIT_TAG master
#   CONFIGURE_COMMAND ""
#   BUILD_COMMAND ""
#   )
FetchContent_Declare(json-c
  URL https://codeload.github.com/json-c/json-c/zip/refs/heads/json-c-0.15)
FetchContent_MakeAvailable(json-c)

# 添加子工程
# add_subdirectory(json-c-0.15)
add_subdirectory(jsoncpp-0.y.z)

link_directories(${LIBRARY_OUTPUT_PATH})

add_executable(${TRAGET_NAME} ${DIR_SRCS})

# 链接子工程
target_link_libraries(${TRAGET_NAME} PRIVATE jsoncpp json-c)
