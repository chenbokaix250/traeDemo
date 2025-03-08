cmake_minimum_required(VERSION 3.10)
project(PreciseTimer)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(timer_example timer_example.cpp)

# 如果需要链接其他库
# target_link_libraries(timer_example pthread)

# 在某些系统上可能需要显式链接pthread
if(UNIX AND NOT APPLE)
    target_link_libraries(timer_example pthread)
endif()