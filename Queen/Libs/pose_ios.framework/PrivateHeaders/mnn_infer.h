#include <MNN/ImageProcess.hpp>
#include <MNN/Interpreter.hpp>
#include <MNN/AutoTime.hpp>
#include <memory>
#include <vector>
#include <iostream>
#include <fstream>


using namespace MNN;
using namespace MNN::CV;
class MnnInfer
{
public:
    MnnInfer();
    ~MnnInfer();

    void init_model(const char *model_path, const void* buffer, size_t size, const char *write_path,
            const char *write_name, bool use_GPU, int w, int h, std::fstream &loger);

    void resize_engine(int net_in_w, int net_in_h);

    void get_in_shape(std::vector<int> &in_shape);

    void get_out_shape(std::vector<int> &out_shape);

    void *get_inputtensor();

    void *get_outputtensor();

    void processImage(unsigned char *inputRGB, int w, int h);

    void run_model();

    void release();
    

private:

    bool m_use_GPU;
    std::string m_write_path;
    std::string m_write_name;

    int m_net_in_w;
    int m_net_in_h;

    const float means[3] = {127.5f, 127.5f, 127.5f};
    const float norms[3] = {1.0f / 128.0f, 1.0f / 128.0f, 1.0f / 128.0f};
    std::shared_ptr<MNN::CV::ImageProcess> pretreat;

    std::shared_ptr<Interpreter> seg_interpreter_;
    Session *seg_session_;
    Tensor *input_tensor_;
    Tensor *output_tensor_;
    Tensor *result_tensor_;
    Tensor *in_tensor_;
};