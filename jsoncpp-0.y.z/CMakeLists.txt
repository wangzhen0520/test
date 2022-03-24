cmake_minimum_required(VERSION 3.10)

project(jsoncpp)

AUX_SOURCE_DIRECTORY(. JS_SRCS)

add_library(${PROJECT_NAME} STATIC ${JS_SRCS})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/json)
