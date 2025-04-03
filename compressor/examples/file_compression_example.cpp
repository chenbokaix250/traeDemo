#include "file_compressor.h"
#include <iostream>
#include <chrono>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cout << "用法: " << argv[0] << " <输入文件> <输出文件>" << std::endl;
        return 1;
    }
    
    std::string inputFile = argv[1];
    std::string outputFile = argv[2];
    std::string decompressedFile = outputFile + ".decompressed";
    
    // 记录开始时间
    auto startTime = std::chrono::high_resolution_clock::now();
    
    // 压缩文件
    std::cout << "压缩文件: " << inputFile << " -> " << outputFile << std::endl;
    if (!zstd_compressor::FileCompressor::compress(inputFile, outputFile)) {
        std::cerr << "压缩失败" << std::endl;
        return 1;
    }
    
    // 记录压缩时间
    auto compressTime = std::chrono::high_resolution_clock::now();
    auto compressDuration = std::chrono::duration_cast<std::chrono::milliseconds>(
        compressTime - startTime).count();
    
    // 获取文件大小
    size_t originalSize = zstd_compressor::FileCompressor::getCompressedSize(inputFile);
    size_t compressedSize = zstd_compressor::FileCompressor::getCompressedSize(outputFile);
    
    // 解压文件
    std::cout << "解压文件: " << outputFile << " -> " << decompressedFile << std::endl;
    if (!zstd_compressor::FileCompressor::decompress(outputFile, decompressedFile)) {
        std::cerr << "解压失败" << std::endl;
        return 1;
    }
    
    // 记录解压时间
    auto decompressTime = std::chrono::high_resolution_clock::now();
    auto decompressDuration = std::chrono::duration_cast<std::chrono::milliseconds>(
        decompressTime - compressTime).count();
    
    // 输出统计信息
    std::cout << "原始大小: " << originalSize << " 字节" << std::endl;
    std::cout << "压缩大小: " << compressedSize << " 字节" << std::endl;
    std::cout << "压缩比例: " << (originalSize > 0 ? (float)compressedSize / originalSize * 100 : 0) << "%" << std::endl;
    std::cout << "压缩时间: " << compressDuration << " 毫秒" << std::endl;
    std::cout << "解压时间: " << decompressDuration << " 毫秒" << std::endl;
    
    return 0;
}