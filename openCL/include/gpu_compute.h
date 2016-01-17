#ifndef __GPU_COMPUTE_H__
#define __GPU_COMPUTE_H__

#if defined(__APPLE__)
#include <OpenCL/opencl.h>
#else
#include <CL/opencl.h>
#endif //__APPLE__

namespace GPUCompute
{
  class GPUComputeImp
  {
    public:
      GPUComputeImp();
      ~GPUComputeImp();

      void init();
      void deinit();

      void load_and_build_kernel(const char *fname, const char *kernel_name);
    private:
      int                       num_elements;
      cl_context                context;
      cl_device_id              device_id;

      cl_mem                    input_buffer;
      cl_mem                    output_buffer;
  };
};

#endif //__GPU_COMPUTE_H__
