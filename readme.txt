1. First, please run ./run.sh and you may see the problem I'm experiencing: 
    UNAVAILABLE: Unknown: DALI Backend error: [/opt/dali/dali/pipeline/operator/argument.cc:68] Assert on "it != fn_map.end()" failed: Invalid argument "type" in protobuf
2. Remove line 41 in main.cpp and line 64 in myoperation.h, and then run.sh won't exit with an error. 
3. client.py is for checking the model running ok.
4. Thanks for helping me out of this!