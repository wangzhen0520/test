cmake_minimum_required(VERSION 3.10)

project(test)

option (SDL_SUPPORT "sdl support on or off" OFF)

# 这段代码可以区分操作系统
message("Identifying the OS...")
if(WIN32)
  set(SDL_SUPPORT ON)
  message("This is Windows.")
elseif(APPLE)
  message("This is MacOS.")
elseif(UNIX)
  message("This is Linux.")
endif()


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

get_filename_component(CURDIR ${CMAKE_CURRENT_SOURCE_DIR} NAME ABSOLUTE)
set(TRAGET_NAME ${CURDIR})

set(CMAKE_BUILD_TYPE "Debug")

# add_definitions(-DDEBUG) # 相当于代码中 #define DEBUG

# 设置目录
set(JSON_PATH ${CMAKE_CURRENT_SOURCE_DIR}/3rd/jsoncpp-0.y.z)
set(JSON_MERGE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/jsonmerge)
# 重设目标生成目录
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR})
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})

# 添加源代码文件夹, 自动扫描所有文件
aux_source_directory(. DIR_SRCS)
aux_source_directory(./src DIR_SRCS)
aux_source_directory(./src/utils DIR_SRCS)
aux_source_directory(./leetcode DIR_SRCS)
aux_source_directory(./jsonmerge DIR_SRCS)
aux_source_directory(./reflector DIR_SRCS)
aux_source_directory(./reflector/framework DIR_SRCS)
aux_source_directory(./others DIR_SRCS)

include(FetchContent)
# FetchContent_Declare(json-c
#   GIT_REPOSITORY https://github.com/json-c/json-c.git
#   GIT_TAG master
#   CONFIGURE_COMMAND ""
#   BUILD_COMMAND ""
#   )
FetchContent_Declare(json-c
  URL https://codeload.github.com/json-c/json-c/zip/refs/heads/json-c-0.15
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ext/json-c
  )
FetchContent_GetProperties(json-c)
if(NOT json-c_POPULATED)
  FetchContent_Populate(json-c)
  add_subdirectory(${json-c_SOURCE_DIR} ${json-c_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()
set(JSON_C_ROOT_DIR ${json-c_SOURCE_DIR})
set(JSON_C_INCLUDE_DIR ${json-c_BINARY_DIR})

set(SDL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rd/SDL/include)
set(SDL_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rd/SDL/lib)
set(SDL_DLL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/3rd/SDL/SDL2.dll)

set(MINIZ_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rd/miniz)

# message("1111 "${json-c_BINARY_DIR})
# message("2222 "${json-c_SOURCE_DIR})

# 头文件包含目录
include_directories(
  .
  ./include
  ./src
  ./src/uitls
  ${JSON_PATH}
  ${JSON_MERGE_PATH}
  ${JSON_C_ROOT_DIR}
  ${JSON_C_INCLUDE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/reflector
  ${SDL_INCLUDE_DIR}
  ${MINIZ_INCLUDE_DIR}
)

set(3rd_library jsoncpp json-c miniz)

if(SDL_SUPPORT)
  list(APPEND 3rd_library SDL2)

add_custom_command(OUTPUT COPY_RES
  COMMAND ${CMAKE_COMMAND} -E copy ${SDL_DLL_PATH} ${EXECUTABLE_OUTPUT_PATH}
  )
add_custom_target(CopyTask ALL DEPENDS COPY_RES)
endif()

# 添加子工程
# add_subdirectory(json-c-0.15)
add_subdirectory(3rd/jsoncpp-0.y.z)
add_subdirectory(3rd/miniz)

link_directories(${LIBRARY_OUTPUT_PATH})
if(SDL_SUPPORT)
link_directories(${SDL_LIB_DIR})
endif()

add_executable(${TRAGET_NAME} ${DIR_SRCS})

# 链接子工程
target_link_libraries(${TRAGET_NAME} PRIVATE ${3rd_library})
