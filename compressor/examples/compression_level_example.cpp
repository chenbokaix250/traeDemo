#include "file_compressor.h"
#include <iostream>
#include <fstream>  // 添加这个头文件
#include <chrono>
#include <vector>
#include <iomanip>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "用法: " << argv[0] << " <输入文件>" << std::endl;
        return 1;
    }
    
    std::string inputFile = argv[1];
    
    // 获取原始文件大小
    std::ifstream inFile(inputFile, std::ios::binary | std::ios::ate);
    if (!inFile) {
        std::cerr << "无法打开输入文件: " << inputFile << std::endl;
        return 1;
    }
    size_t originalSize = inFile.tellg();
    inFile.close();
    
    std::cout << "原始文件大小: " << originalSize << " 字节" << std::endl;
    std::cout << "\n压缩级别比较:\n" << std::endl;
    
    // 表头
    std::cout << std::setw(10) << "压缩级别" 
              << std::setw(15) << "压缩大小(字节)" 
              << std::setw(15) << "压缩比例(%)" 
              << std::setw(15) << "压缩时间(ms)" 
              << std::setw(15) << "解压时间(ms)" 
              << std::endl;
    std::cout << std::string(70, '-') << std::endl;
    
    // 测试不同压缩级别
    for (int level = 1; level <= 22; level += 3) {
        std::string outputFile = inputFile + ".zst.level" + std::to_string(level);
        std::string decompressedFile = outputFile + ".decompressed";
        
        // 压缩
        auto startTime = std::chrono::high_resolution_clock::now();
        bool compressResult = zstd_compressor::FileCompressor::compress(inputFile, outputFile, level);
        auto compressTime = std::chrono::high_resolution_clock::now();
        auto compressDuration = std::chrono::duration_cast<std::chrono::milliseconds>(
            compressTime - startTime).count();
        
        if (!compressResult) {
            std::cerr << "压缩失败，级别: " << level << std::endl;
            continue;
        }
        
        // 获取压缩文件大小
        std::ifstream compressedFile(outputFile, std::ios::binary | std::ios::ate);
        if (!compressedFile) {
            std::cerr << "无法打开压缩文件: " << outputFile << std::endl;
            continue;
        }
        size_t compressedSize = compressedFile.tellg();
        compressedFile.close();
        
        // 解压
        auto startDecompressTime = std::chrono::high_resolution_clock::now();
        bool decompressResult = zstd_compressor::FileCompressor::decompress(outputFile, decompressedFile);
        auto decompressTime = std::chrono::high_resolution_clock::now();
        auto decompressDuration = std::chrono::duration_cast<std::chrono::milliseconds>(
            decompressTime - startDecompressTime).count();
        
        if (!decompressResult) {
            std::cerr << "解压失败，级别: " << level << std::endl;
            continue;
        }
        
        // 计算压缩比例
        float compressionRatio = (float)compressedSize / originalSize * 100;
        
        // 输出结果
        std::cout << std::setw(10) << level 
                  << std::setw(15) << compressedSize 
                  << std::setw(15) << std::fixed << std::setprecision(2) << compressionRatio 
                  << std::setw(15) << compressDuration 
                  << std::setw(15) << decompressDuration 
                  << std::endl;
    }
    
    return 0;
}