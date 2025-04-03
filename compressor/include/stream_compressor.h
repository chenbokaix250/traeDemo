#ifndef STREAM_COMPRESSOR_H
#define STREAM_COMPRESSOR_H

#include <vector>
#include <string>

namespace zstd_compressor {

class StreamCompressor {
public:
    StreamCompressor(int compressionLevel = 3);
    ~StreamCompressor();

    // 压缩数据块
    std::vector<char> compress(const std::vector<char>& data);
    std::vector<char> compress(const char* data, size_t size);
    
    // 解压数据块
    std::vector<char> decompress(const std::vector<char>& compressedData);
    std::vector<char> decompress(const char* compressedData, size_t compressedSize);
    
    // 流式压缩 - 开始新的压缩会话
    void startCompression();
    
    // 流式压缩 - 添加数据
    std::vector<char> compressChunk(const std::vector<char>& chunk);
    
    // 流式压缩 - 结束压缩并获取剩余数据
    std::vector<char> endCompression();
    
    // 流式解压 - 开始新的解压会话
    void startDecompression();
    
    // 流式解压 - 添加数据
    std::vector<char> decompressChunk(const std::vector<char>& compressedChunk);
    
    // 流式解压 - 结束解压并获取剩余数据
    std::vector<char> endDecompression();

private:
    int compressionLevel_;
    void* cStream_;  // ZSTD_CStream*
    void* dStream_;  // ZSTD_DStream*
    bool isCompressing_;
    bool isDecompressing_;
};

} // namespace zstd_compressor

#endif // STREAM_COMPRESSOR_H