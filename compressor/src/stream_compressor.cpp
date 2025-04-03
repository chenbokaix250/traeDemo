#include "stream_compressor.h"
#include <zstd.h>
#include <iostream>

namespace zstd_compressor {

StreamCompressor::StreamCompressor(int compressionLevel)
    : compressionLevel_(compressionLevel),
      cStream_(nullptr),
      dStream_(nullptr),
      isCompressing_(false),
      isDecompressing_(false) {
}

StreamCompressor::~StreamCompressor() {
    if (cStream_) {
        ZSTD_freeCStream(static_cast<ZSTD_CStream*>(cStream_));
    }
    if (dStream_) {
        ZSTD_freeDStream(static_cast<ZSTD_DStream*>(dStream_));
    }
}

std::vector<char> StreamCompressor::compress(const std::vector<char>& data) {
    return compress(data.data(), data.size());
}

std::vector<char> StreamCompressor::compress(const char* data, size_t size) {
    // 计算压缩缓冲区大小
    size_t compressBound = ZSTD_compressBound(size);
    std::vector<char> compressedBuffer(compressBound);
    
    // 压缩数据
    size_t compressedSize = ZSTD_compress(
        compressedBuffer.data(), compressBound,
        data, size,
        compressionLevel_
    );
    
    // 检查压缩是否成功
    if (ZSTD_isError(compressedSize)) {
        std::cerr << "压缩错误: " << ZSTD_getErrorName(compressedSize) << std::endl;
        return {};
    }
    
    // 调整缓冲区大小为实际压缩大小
    compressedBuffer.resize(compressedSize);
    return compressedBuffer;
}

std::vector<char> StreamCompressor::decompress(const std::vector<char>& compressedData) {
    return decompress(compressedData.data(), compressedData.size());
}

std::vector<char> StreamCompressor::decompress(const char* compressedData, size_t compressedSize) {
    // 获取原始大小
    unsigned long long originalSize = ZSTD_getFrameContentSize(compressedData, compressedSize);
    if (originalSize == ZSTD_CONTENTSIZE_UNKNOWN || originalSize == ZSTD_CONTENTSIZE_ERROR) {
        std::cerr << "无法确定原始大小" << std::endl;
        return {};
    }
    
    // 分配解压缓冲区
    std::vector<char> decompressedBuffer(originalSize);
    
    // 解压数据
    size_t decompressedSize = ZSTD_decompress(
        decompressedBuffer.data(), originalSize,
        compressedData, compressedSize
    );
    
    // 检查解压是否成功
    if (ZSTD_isError(decompressedSize)) {
        std::cerr << "解压错误: " << ZSTD_getErrorName(decompressedSize) << std::endl;
        return {};
    }
    
    // 调整缓冲区大小为实际解压大小
    decompressedBuffer.resize(decompressedSize);
    return decompressedBuffer;
}

void StreamCompressor::startCompression() {
    // 如果已经有压缩流，先释放
    if (cStream_) {
        ZSTD_freeCStream(static_cast<ZSTD_CStream*>(cStream_));
    }
    
    // 创建新的压缩流
    cStream_ = ZSTD_createCStream();
    if (!cStream_) {
        std::cerr << "无法创建压缩流" << std::endl;
        return;
    }
    
    // 初始化压缩流
    size_t initResult = ZSTD_initCStream(static_cast<ZSTD_CStream*>(cStream_), compressionLevel_);
    if (ZSTD_isError(initResult)) {
        std::cerr << "初始化压缩流错误: " << ZSTD_getErrorName(initResult) << std::endl;
        ZSTD_freeCStream(static_cast<ZSTD_CStream*>(cStream_));
        cStream_ = nullptr;
        return;
    }
    
    isCompressing_ = true;
}

std::vector<char> StreamCompressor::compressChunk(const std::vector<char>& chunk) {
    if (!isCompressing_ || !cStream_) {
        std::cerr << "压缩流未初始化" << std::endl;
        return {};
    }
    
    // 准备输入缓冲区
    ZSTD_inBuffer input = { chunk.data(), chunk.size(), 0 };
    
    // 准备输出缓冲区 (最大可能的压缩大小)
    size_t outSize = ZSTD_compressBound(chunk.size());
    std::vector<char> outBuffer(outSize);
    ZSTD_outBuffer output = { outBuffer.data(), outSize, 0 };
    
    // 压缩数据块
    size_t remainingToFlush = ZSTD_compressStream(static_cast<ZSTD_CStream*>(cStream_), &output, &input);
    if (ZSTD_isError(remainingToFlush)) {
        std::cerr << "压缩流错误: " << ZSTD_getErrorName(remainingToFlush) << std::endl;
        return {};
    }
    
    // 调整输出缓冲区大小
    outBuffer.resize(output.pos);
    return outBuffer;
}

std::vector<char> StreamCompressor::endCompression() {
    if (!isCompressing_ || !cStream_) {
        std::cerr << "压缩流未初始化" << std::endl;
        return {};
    }
    
    // 准备输出缓冲区
    std::vector<char> outBuffer(ZSTD_compressBound(0));
    ZSTD_outBuffer output = { outBuffer.data(), outBuffer.size(), 0 };
    
    // 结束压缩流
    size_t remainingToFlush = ZSTD_endStream(static_cast<ZSTD_CStream*>(cStream_), &output);
    if (remainingToFlush > 0) {
        std::cerr << "压缩流结束错误: 仍有数据需要刷新" << std::endl;
    }
    
    // 释放压缩流
    ZSTD_freeCStream(static_cast<ZSTD_CStream*>(cStream_));
    cStream_ = nullptr;
    isCompressing_ = false;
    
    // 调整输出缓冲区大小
    outBuffer.resize(output.pos);
    return outBuffer;
}

void StreamCompressor::startDecompression() {
    // 如果已经有解压流，先释放
    if (dStream_) {
        ZSTD_freeDStream(static_cast<ZSTD_DStream*>(dStream_));
    }
    
    // 创建新的解压流
    dStream_ = ZSTD_createDStream();
    if (!dStream_) {
        std::cerr << "无法创建解压流" << std::endl;
        return;
    }
    
    // 初始化解压流
    size_t initResult = ZSTD_initDStream(static_cast<ZSTD_DStream*>(dStream_));
    if (ZSTD_isError(initResult)) {
        std::cerr << "初始化解压流错误: " << ZSTD_getErrorName(initResult) << std::endl;
        ZSTD_freeDStream(static_cast<ZSTD_DStream*>(dStream_));
        dStream_ = nullptr;
        return;
    }
    
    isDecompressing_ = true;
}

std::vector<char> StreamCompressor::decompressChunk(const std::vector<char>& compressedChunk) {
    if (!isDecompressing_ || !dStream_) {
        std::cerr << "解压流未初始化" << std::endl;
        return {};
    }
    
    // 准备输入缓冲区
    ZSTD_inBuffer input = { compressedChunk.data(), compressedChunk.size(), 0 };
    
    // 准备输出缓冲区 (估计解压大小)
    std::vector<char> outBuffer(compressedChunk.size() * 4); // 假设解压比例为4倍
    ZSTD_outBuffer output = { outBuffer.data(), outBuffer.size(), 0 };
    
    // 解压数据块
    size_t result = ZSTD_decompressStream(static_cast<ZSTD_DStream*>(dStream_), &output, &input);
    if (ZSTD_isError(result)) {
        std::cerr << "解压流错误: " << ZSTD_getErrorName(result) << std::endl;
        return {};
    }
    
    // 调整输出缓冲区大小
    outBuffer.resize(output.pos);
    return outBuffer;
}

std::vector<char> StreamCompressor::endDecompression() {
    if (!isDecompressing_ || !dStream_) {
        std::cerr << "解压流未初始化" << std::endl;
        return {};
    }
    
    // 释放解压流
    ZSTD_freeDStream(static_cast<ZSTD_DStream*>(dStream_));
    dStream_ = nullptr;
    isDecompressing_ = false;
    
    return {};
}

} // namespace zstd_compressor