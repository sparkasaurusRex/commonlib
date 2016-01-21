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

      void upload_input_array(void *data);

      void set_num_elements(const unsigned int e) { num_elements = e; }
      unsigned int get_num_elements() const { return num_elements; }

      void set_max_elements(const unsigned int e) { max_elements = e; }
      unsigned int get_max_elements() const { return max_elements; }

    private:
      unsigned int              num_elements;
      unsigned int              max_elements;
      cl_context                context;
      cl_device_id              device_id;

      cl_mem                    input_buffer;
      cl_mem                    output_buffer;
  };
};

#endif //__GPU_COMPUTE_H__
