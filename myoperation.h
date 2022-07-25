#ifndef MYOPERATION_H_
#define MYOPERATION_H_

#include <vector>
#include <cuda_runtime_api.h>
#include "dali/pipeline/operator/operator.h"

void LogFunc(const char* arg0);

namespace other_ns {
  class InitParams {
  public:
  float x = 0.f;
  std::vector<float> y;

  InitParams(){}
  InitParams(float x_, std::vector<float> y_): x(x_), y(y_){}

  dali::DaliProtoPriv * SerializeToProtobuf(dali::DaliProtoPriv *arg) const {
    arg->set_type("InitParams");
    arg->set_is_vector(false);

    auto x_arg = arg->add_extra_args();
    x_arg.set_name("x");
    dali::SerializeToProtobuf(x, &x_arg);

    auto y_arg = arg->add_extra_args();
    y_arg.set_name("y");
    y_arg.set_is_vector(true);
    for (size_t i = 0; i < y.size(); ++i) {
      dali::SerializeToProtobuf(y[i], &y_arg);
    }

    return arg;
  }

  static InitParams DeserializeFromProtobuf(const dali::DaliProtoPriv& arg) {
    return InitParams(
      arg.extra_args(0).floats(0),
      arg.extra_args(1).floats()
    );
  }

  std::string ToString() const {
    std::string ret = "InitParams {";

    ret += "x " + std::to_string(x) + ", ";

    ret += "y [";
    for(int i = 0; i < y.size(); i++){
      ret += std::to_string(y[i]) + ", ";
    }
    ret += "], ";

    ret += "}";
    return ret;
  }
};

class MyOp : public ::dali::Operator<::dali::MixedBackend> {
 public:
  explicit MyOp(const ::dali::OpSpec &spec)
      : Operator<::dali::MixedBackend>(spec)
      , dummy_params(spec.GetArgument<InitParams>("init_params"))
      {}

  bool SetupImpl(std::vector<::dali::OutputDesc> &output_desc, const ::dali::MixedWorkspace &ws) override {
    return false;
  }

  using ::dali::Operator<::dali::MixedBackend>::Run;
  void Run(::dali::MixedWorkspace &ws) override;
  
  InitParams dummy_params;
};


}  // namespace other_ns


#endif  // MYOPERATION_H_
