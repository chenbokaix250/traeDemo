cmake_minimum_required(VERSION 3.10)

project(ClientDemo)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 设置CMake策略以消除警告
if(POLICY CMP0167)
    cmake_policy(SET CMP0167 NEW)
endif()

# 查找Boost库
find_package(Boost REQUIRED COMPONENTS system)

# 添加源文件
add_executable(ClientDemo main.cpp BaseFrame.cpp)

# 包含头文件目录
target_include_directories(ClientDemo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${Boost_INCLUDE_DIRS})

# 链接Boost库
target_link_libraries(ClientDemo Boost::system)