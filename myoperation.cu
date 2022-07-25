#include "myoperation.h"
#include <iostream>

namespace other_ns {
  void MyOp::Run(::dali::MixedWorkspace &ws) {

    auto &input = ws.Input<::dali::CPUBackend>(0);
    int num_samples = input.shape().num_samples();
    auto &output = ws.Output<::dali::GPUBackend>(0);
    output.set_type<size_t>();
    size_t tmp_size[2] = {output.capacity(), input.capacity()};
    output.Resize(::dali::uniform_list_shape(num_samples, std::vector<int64_t>{2}));
    for (int sample_idx = 0; sample_idx < num_samples; sample_idx++) {
      auto *out = output.mutable_tensor<size_t>(sample_idx);
      CUDA_CALL(cudaStreamSynchronize(ws.stream()));
      CUDA_CALL(cudaMemcpy(out, &tmp_size, sizeof(size_t) * 2, cudaMemcpyHostToDevice));
    }
    std::cout << "MyOp::Run is done." << std::endl;
  }
}  // namespace other_ns


DALI_REGISTER_OPERATOR(MyOperation, ::other_ns::MyOp, ::dali::Mixed);
DALI_SCHEMA(MyOperation)
  .DocStr("MyOp")
  .NumInput(1)
  .NumOutput(1);

