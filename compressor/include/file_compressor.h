#ifndef FILE_COMPRESSOR_H
#define FILE_COMPRESSOR_H

#include <string>

namespace zstd_compressor {

class FileCompressor {
public:
    // 压缩文件
    static bool compress(const std::string& inputFile, const std::string& outputFile, int compressionLevel = 3);
    
    // 解压文件
    static bool decompress(const std::string& inputFile, const std::string& outputFile);
    
    // 获取压缩文件大小
    static size_t getCompressedSize(const std::string& filePath);
    
    // 获取原始文件大小
    static size_t getOriginalSize(const std::string& compressedFilePath);
};

} // namespace zstd_compressor

#endif // FILE_COMPRESSOR_H