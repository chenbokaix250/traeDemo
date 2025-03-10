cmake_minimum_required(VERSION 3.10)
project(HelloWorld)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加可执行文件
add_executable(hello_world src/main.cpp)