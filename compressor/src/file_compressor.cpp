#include "file_compressor.h"
#include <zstd.h>
#include <fstream>
#include <vector>
#include <iostream>

namespace zstd_compressor {

bool FileCompressor::compress(const std::string& inputFile, const std::string& outputFile, int compressionLevel) {
    // 打开输入文件
    std::ifstream inFile(inputFile, std::ios::binary);
    if (!inFile) {
        std::cerr << "无法打开输入文件: " << inputFile << std::endl;
        return false;
    }
    
    // 获取文件大小
    inFile.seekg(0, std::ios::end);
    size_t fileSize = inFile.tellg();
    inFile.seekg(0, std::ios::beg);
    
    // 读取文件内容
    std::vector<char> buffer(fileSize);
    if (fileSize > 0) {
        inFile.read(buffer.data(), fileSize);
    }
    inFile.close();
    
    // 计算压缩缓冲区大小
    size_t compressBound = ZSTD_compressBound(fileSize);
    std::vector<char> compressedBuffer(compressBound);
    
    // 压缩数据
    size_t compressedSize = ZSTD_compress(
        compressedBuffer.data(), compressBound,
        buffer.data(), fileSize,
        compressionLevel
    );
    
    // 检查压缩是否成功
    if (ZSTD_isError(compressedSize)) {
        std::cerr << "压缩错误: " << ZSTD_getErrorName(compressedSize) << std::endl;
        return false;
    }
    
    // 写入压缩数据到输出文件
    std::ofstream outFile(outputFile, std::ios::binary);
    if (!outFile) {
        std::cerr << "无法创建输出文件: " << outputFile << std::endl;
        return false;
    }
    
    outFile.write(compressedBuffer.data(), compressedSize);
    outFile.close();
    
    return true;
}

bool FileCompressor::decompress(const std::string& inputFile, const std::string& outputFile) {
    // 打开压缩文件
    std::ifstream inFile(inputFile, std::ios::binary);
    if (!inFile) {
        std::cerr << "无法打开压缩文件: " << inputFile << std::endl;
        return false;
    }
    
    // 获取压缩文件大小
    inFile.seekg(0, std::ios::end);
    size_t compressedSize = inFile.tellg();
    inFile.seekg(0, std::ios::beg);
    
    // 读取压缩数据
    std::vector<char> compressedBuffer(compressedSize);
    if (compressedSize > 0) {
        inFile.read(compressedBuffer.data(), compressedSize);
    }
    inFile.close();
    
    // 获取原始大小
    unsigned long long originalSize = ZSTD_getFrameContentSize(compressedBuffer.data(), compressedSize);
    if (originalSize == ZSTD_CONTENTSIZE_UNKNOWN || originalSize == ZSTD_CONTENTSIZE_ERROR) {
        std::cerr << "无法确定原始大小" << std::endl;
        return false;
    }
    
    // 分配解压缓冲区
    std::vector<char> decompressedBuffer(originalSize);
    
    // 解压数据
    size_t decompressedSize = ZSTD_decompress(
        decompressedBuffer.data(), originalSize,
        compressedBuffer.data(), compressedSize
    );
    
    // 检查解压是否成功
    if (ZSTD_isError(decompressedSize)) {
        std::cerr << "解压错误: " << ZSTD_getErrorName(decompressedSize) << std::endl;
        return false;
    }
    
    // 写入解压数据到输出文件
    std::ofstream outFile(outputFile, std::ios::binary);
    if (!outFile) {
        std::cerr << "无法创建输出文件: " << outputFile << std::endl;
        return false;
    }
    
    outFile.write(decompressedBuffer.data(), decompressedSize);
    outFile.close();
    
    return true;
}

size_t FileCompressor::getCompressedSize(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary | std::ios::ate);
    if (!file) {
        return 0;
    }
    return file.tellg();
}

size_t FileCompressor::getOriginalSize(const std::string& compressedFilePath) {
    std::ifstream file(compressedFilePath, std::ios::binary);
    if (!file) {
        return 0;
    }
    
    // 获取文件大小
    file.seekg(0, std::ios::end);
    size_t fileSize = file.tellg();
    file.seekg(0, std::ios::beg);
    
    // 读取压缩数据
    std::vector<char> buffer(fileSize);
    if (fileSize > 0) {
        file.read(buffer.data(), fileSize);
    }
    file.close();
    
    // 获取原始大小
    unsigned long long originalSize = ZSTD_getFrameContentSize(buffer.data(), fileSize);
    if (originalSize == ZSTD_CONTENTSIZE_UNKNOWN || originalSize == ZSTD_CONTENTSIZE_ERROR) {
        return 0;
    }
    
    return originalSize;
}

} // namespace zstd_compressor