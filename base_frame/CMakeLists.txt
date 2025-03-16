cmake_minimum_required(VERSION 3.10)

# 项目名称
project(BaseFrameDemo)

# 设置C++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 添加源文件
add_executable(BaseFrameDemo main.cpp BaseFrame.cpp)

# 包含头文件目录
target_include_directories(BaseFrameDemo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})