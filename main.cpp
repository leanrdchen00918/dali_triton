#include "myoperation.h"
#include "dali/pipeline/pipeline.h"


#include "dali/core/common.h"
#include "dali/pipeline/data/backend.h"
#include "dali/pipeline/data/buffer.h"
#include "dali/pipeline/data/tensor.h"
#include "dali/pipeline/operator/operator.h"
#include "dali/pipeline/operator/builtin/copy.h"
#include "dali/util/image.h"
#include "dali/plugin/plugin_manager.h"

void daliLoadLibrary(const char* lib_path) {
    dali::PluginManager::LoadLibrary(lib_path);
}

void WriteProtobufAsFile(std::string serialized, std::string filename){
  std::ofstream out;
  out.open(filename, std::ios::out|std::ios::binary);
  out << serialized;
  out.close();
}

int main(){
    const int batch_size = 1;
    const int num_thread = 1;
    const int device_id = 0;
    const cudaStream_t stream = 0;

    daliLoadLibrary("./libop.so");
    
    // create pipeline instance
    ::dali::Pipeline pipe(batch_size, num_thread, device_id);
    
    // construct pipeline
    pipe.AddExternalInput("DALI_INPUT_0");
    pipe.AddOperator(
        ::dali::OpSpec("MyOperation")
        .AddArg("device", "mixed")
        .AddArg("init_params", other_ns::InitParams(1.0f, std::vector<float>{2.0f, 3.0f}))
        .AddInput("DALI_INPUT_0", "cpu")
        .AddOutput("DALI_OUTPUT_0", "gpu"));

    // build pipeline
    std::vector<std::pair<std::string, std::string>> outputs = {{"DALI_OUTPUT_0", "gpu"}};
    pipe.Build(outputs);

    // serialize pipeline
    auto serialized = pipe.SerializeToProtobuf();
    WriteProtobufAsFile(serialized, "./model.dali");
    
    // run pipeline
    // dali::TensorList<dali::CPUBackend> begin_data;
    // auto shape = dali::uniform_list_shape(batch_size, {2});
    // begin_data.Resize(shape, dali::DALI_UINT64);
    // size_t crop_x = 1, crop_y = 2;
    // for (int k = 0; k < batch_size; k++) {
    //     begin_data.mutable_tensor<size_t>(k)[0] = crop_x;
    //     begin_data.mutable_tensor<size_t>(k)[1] = crop_y;
    // }
    // pipe.SetExternalInput("DALI_INPUT_0", begin_data);
    // ::dali::DeviceWorkspace ws;
    // ws.set_stream(stream);
    // pipe.RunCPU();
    // pipe.RunGPU();
    // pipe.Outputs(&ws);
    return 0;
}


