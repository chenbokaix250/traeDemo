cmake_minimum_required(VERSION 3.10)
project(ZstdCompressor VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 尝试使用 pkg-config 查找 zstd 库
find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    pkg_check_modules(ZSTD libzstd)
endif()

# 如果 pkg-config 未找到 zstd，尝试直接查找
if(NOT ZSTD_FOUND)
    find_path(ZSTD_INCLUDE_DIR zstd.h
        PATHS /usr/local/include /opt/homebrew/include /usr/include
    )
    find_library(ZSTD_LIBRARY
        NAMES zstd
        PATHS /usr/local/lib /opt/homebrew/lib /usr/lib
    )
    
    if(ZSTD_INCLUDE_DIR AND ZSTD_LIBRARY)
        set(ZSTD_FOUND TRUE)
        set(ZSTD_INCLUDE_DIRS ${ZSTD_INCLUDE_DIR})
        set(ZSTD_LIBRARIES ${ZSTD_LIBRARY})
        message(STATUS "Found zstd: ${ZSTD_LIBRARY}")
    endif()
endif()

# 检查是否找到 zstd
if(NOT ZSTD_FOUND)
    message(FATAL_ERROR "Could not find zstd library. Please install it using 'brew install zstd'")
endif()

# 包含目录
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${ZSTD_INCLUDE_DIRS}
)

# 添加库
add_library(zstd_compressor
    src/file_compressor.cpp
    src/stream_compressor.cpp
)

# 链接 zstd 库
target_link_libraries(zstd_compressor PUBLIC ${ZSTD_LIBRARIES})

# 设置链接目录
link_directories(${ZSTD_LIBRARY_DIRS})

# 添加示例
add_subdirectory(examples)

# 安装头文件
install(DIRECTORY include/ DESTINATION include)

# 安装库
install(TARGETS zstd_compressor
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)