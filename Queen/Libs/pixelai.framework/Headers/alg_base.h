#include "pixelai_common.hpp"
#include <functional>
#include <istream>
#include <string>
#include <memory>
#include <vector>
#ifndef ALG_BASE_H
#define ALG_BASE_H

class AlgBase
{
public:
    AlgBase();
    ~AlgBase();

    void SetIstreamFunc(pixelai_istream_func istreamFunc);
    pixelai_istream_func GetIstreamFunc();

    void SetOstreamFunc(pixelai_ostream_func ostreamFunc);
    pixelai_ostream_func GetOstreamFunc();
    
    void SetReadBufferFunc(pixelai_read_buffer_func readBufferFunc);
    pixelai_read_buffer_func GetReadBufferFunc();

    std::unique_ptr<std::istream> GetFileIstream(const std::string filePath, std::ios_base::openmode);
    std::unique_ptr<std::ostream> GetFileOstream(const std::string filePath, std::ios_base::openmode);

    std::vector<uint8_t> GetFileData(const std::string filePath);

private:
    pixelai_istream_func mIstreamFunc;
    pixelai_ostream_func mOstreamFunc;
    pixelai_read_buffer_func mReadBufferFunc;
    std::vector<uint8_t> data;     
};

#endif //ALG_BASE_H