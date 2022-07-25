cmake_minimum_required(VERSION 3.10)

set(CMAKE_CUDA_ARCHITECTURES "35;50;52;60;61;70;75;80;86")

# Set the project name
project(third_party_include LANGUAGES CUDA CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 11)

set(CMAKE_CUDA_STANDARD 14)
set(CMAKE_CUDA_STANDARD_REQUIRED ON)

# CUDA
include_directories(SYSTEM "${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES}")
find_package(CUDA  REQUIRED)
include_directories("${CUDA_INCLUDE_DIRS}")

# DALI
execute_process(
        COMMAND python -c "import nvidia.dali as dali; print(dali.sysconfig.get_lib_dir())"
        OUTPUT_VARIABLE DALI_LIB_DIR)
string(STRIP ${DALI_LIB_DIR} DALI_LIB_DIR)

execute_process(
        COMMAND python -c "import nvidia.dali as dali; print(\" \".join(dali.sysconfig.get_compile_flags()))"
        OUTPUT_VARIABLE DALI_COMPILE_FLAGS)
string(STRIP ${DALI_COMPILE_FLAGS} DALI_COMPILE_FLAGS)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${DALI_COMPILE_FLAGS}")
set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} ${DALI_COMPILE_FLAGS}")
link_directories("${DALI_LIB_DIR}")

# Add a .so
add_library(op SHARED myoperation.cu)
target_link_libraries(op PUBLIC ${CUDA_LIBRARIES} dali)

# Add an executable
add_executable(main main.cpp)
target_link_libraries(main PUBLIC ${CUDA_LIBRARIES} dali op "${DALI_LIB_DIR}/libdali_core.so")

