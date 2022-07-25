#!/bin/bash
mkdir -p build
cd build && cmake .. && make && ./main && cd ..

mkdir -p model_repo/mydali/1
cp build/libop.so model_repo/libop.so
cp build/model.dali model_repo/mydali/1/model.dali
cp config.pbtxt model_repo/mydali/config.pbtxt

sudo docker run --gpus=1 --rm -p8000:8000 -p8001:8001 -p8002:8002 -v /model_repo:/models \
nvcr.io/nvidia/tritonserver:22.06-py3 \
tritonserver --model-repository=/models --backend-config dali,plugin_libs=/models/libop.so