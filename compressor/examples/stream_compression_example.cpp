#include "stream_compressor.h"
#include <iostream>
#include <vector>
#include <string>
#include <chrono>

int main() {
    // 创建一些测试数据
    std::string testData = "这是一个测试数据，用于演示zstd流压缩。这段文本将被重复多次以创建更大的数据量。";
    std::vector<char> data;
    
    // 重复数据以创建更大的测试数据
    for (int i = 0; i < 1000; ++i) {
        data.insert(data.end(), testData.begin(), testData.end());
    }
    
    std::cout << "原始数据大小: " << data.size() << " 字节" << std::endl;
    
    // 创建压缩器
    zstd_compressor::StreamCompressor compressor(3);
    
    // 单次压缩/解压
    auto startTime = std::chrono::high_resolution_clock::now();
    
    std::vector<char> compressed = compressor.compress(data);
    
    auto compressTime = std::chrono::high_resolution_clock::now();
    auto compressDuration = std::chrono::duration_cast<std::chrono::milliseconds>(
        compressTime - startTime).count();
    
    std::vector<char> decompressed = compressor.decompress(compressed);
    
    auto decompressTime = std::chrono::high_resolution_clock::now();
    auto decompressDuration = std::chrono::duration_cast<std::chrono::milliseconds>(
        decompressTime - compressTime).count();
    
    std::cout << "单次压缩大小: " << compressed.size() << " 字节" << std::endl;
    std::cout << "压缩比例: " << (float)compressed.size() / data.size() * 100 << "%" << std::endl;
    std::cout << "单次压缩时间: " << compressDuration << " 毫秒" << std::endl;
    std::cout << "单次解压时间: " << decompressDuration << " 毫秒" << std::endl;
    std::cout << "解压后数据大小: " << decompressed.size() << " 字节" << std::endl;
    std::cout << "数据完整性检查: " << (data == decompressed ? "通过" : "失败") << std::endl;
    
    // 流式压缩/解压
    std::cout << "\n===== 流式压缩/解压 =====" << std::endl;
    
    // 将数据分成多个块
    const size_t chunkSize = 1024;
    std::vector<std::vector<char>> chunks;
    
    for (size_t i = 0; i < data.size(); i += chunkSize) {
        size_t size = std::min(chunkSize, data.size() - i);
        chunks.push_back(std::vector<char>(data.begin() + i, data.begin() + i + size));
    }
    
    std::cout << "分块数量: " << chunks.size() << std::endl;
    
    // 流式压缩
    startTime = std::chrono::high_resolution_clock::now();
    
    compressor.startCompression();
    std::vector<std::vector<char>> compressedChunks;
    
    for (const auto& chunk : chunks) {
        compressedChunks.push_back(compressor.compressChunk(chunk));
    }
    
    std::vector<char> finalCompressedChunk = compressor.endCompression();
    if (!finalCompressedChunk.empty()) {
        compressedChunks.push_back(finalCompressedChunk);
    }
    
    compressTime = std::chrono::high_resolution_clock::now();
    compressDuration = std::chrono::duration_cast<std::chrono::milliseconds>(
        compressTime - startTime).count();
    
    // 计算总压缩大小
    size_t totalCompressedSize = 0;
    for (const auto& chunk : compressedChunks) {
        totalCompressedSize += chunk.size();
    }
    
    // 流式解压
    compressor.startDecompression();
    std::vector<std::vector<char>> decompressedChunks;
    
    for (const auto& chunk : compressedChunks) {
        decompressedChunks.push_back(compressor.decompressChunk(chunk));
    }
    
    compressor.endDecompression();
    
    decompressTime = std::chrono::high_resolution_clock::now();
    decompressDuration = std::chrono::duration_cast<std::chrono::milliseconds>(
        decompressTime - compressTime).count();
    
    // 合并解压后的数据
    std::vector<char> streamDecompressed;
    for (const auto& chunk : decompressedChunks) {
        streamDecompressed.insert(streamDecompressed.end(), chunk.begin(), chunk.end());
    }
    
    std::cout << "流式压缩大小: " << totalCompressedSize << " 字节" << std::endl;
    std::cout << "压缩比例: " << (float)totalCompressedSize / data.size() * 100 << "%" << std::endl;
    std::cout << "流式压缩时间: " << compressDuration << " 毫秒" << std::endl;
    std::cout << "流式解压时间: " << decompressDuration << " 毫秒" << std::endl;
    std::cout << "解压后数据大小: " << streamDecompressed.size() << " 字节" << std::endl;
    std::cout << "数据完整性检查: " << (data == streamDecompressed ? "通过" : "失败") << std::endl;
    
    return 0;
}