cmake_minimum_required(VERSION 3.10)

project(TestDemo)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 查找Boost库
find_package(Boost REQUIRED COMPONENTS system)

# 添加测试可执行文件
add_executable(TestDemo launch_modules.cpp ../client/ModuleB.cpp ../client/ModuleC.cpp ../client/BaseFrame.cpp)

# 包含头文件目录
target_include_directories(TestDemo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../client ${Boost_INCLUDE_DIRS})

# 链接Boost库
target_link_libraries(TestDemo Boost::system)