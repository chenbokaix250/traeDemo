# 文件压缩示例
add_executable(file_compression_example file_compression_example.cpp)
target_link_libraries(file_compression_example zstd_compressor)

# 流压缩示例
add_executable(stream_compression_example stream_compression_example.cpp)
target_link_libraries(stream_compression_example zstd_compressor)

# 压缩级别比较示例
add_executable(compression_level_example compression_level_example.cpp)
target_link_libraries(compression_level_example zstd_compressor)