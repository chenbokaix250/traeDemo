cmake_minimum_required(VERSION 3.10)
project(multi_thread_demo)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 设置编译器和编译选项
if(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.15" CACHE STRING "Minimum OS X deployment version")
endif()

# 查找必要的包
find_package(Threads REQUIRED)

# 添加可执行文件
add_executable(thread_basic src/thread_basic.cpp)

# 链接线程库
target_link_libraries(thread_basic PRIVATE Threads::Threads)