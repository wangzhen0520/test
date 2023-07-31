cmake_minimum_required(VERSION 3.10)

project(miniz)

AUX_SOURCE_DIRECTORY(. MINIZ_SRCS)

add_library(${PROJECT_NAME} STATIC ${MINIZ_SRCS})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/miniz)
