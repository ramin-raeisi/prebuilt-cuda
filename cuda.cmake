set(CUDA_TOOLKIT_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})

macro(AziCuda)
  find_package(CUDA 4.0 REQUIRED) #TODO: This is finding 32 bit and that is why the second next line is looking again in 64 bit folder
  include_directories(${CUDA_INCLUDE_DIRS})
  find_library(CudaDEV_LIBRARIES cuda PATHS ${CUDA_TOOLKIT_ROOT_DIR}/lib/x64)
endmacro(AziCuda)

