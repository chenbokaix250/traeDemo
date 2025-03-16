cmake_minimum_required(VERSION 3.10)

project(ServerDemo)

# 设置C++标准为C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 设置CMake策略以消除警告
if(POLICY CMP0167)
    cmake_policy(SET CMP0167 NEW)
endif()

find_package(Boost REQUIRED COMPONENTS system)

add_executable(ServerDemo main.cpp HeartbeatServer.cpp)

target_include_directories(ServerDemo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ServerDemo Boost::system)