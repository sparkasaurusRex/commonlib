#include <iostream>
#include <assert.h>

#include "gpu_compute.h"

using namespace GPUCompute;
using namespace std;

GPUComputeImp::GPUComputeImp()
{
  num_elements = 0;
}

GPUComputeImp::~GPUComputeImp()
{

}

void GPUComputeImp::init()
{
  bool use_gpu = true;

  //connect to compute device
  int err = clGetDeviceIDs(NULL, use_gpu ? CL_DEVICE_TYPE_GPU : CL_DEVICE_TYPE_CPU, 1, &device_id, NULL);
  assert(err == CL_SUCCESS);

  //create a compute context
  context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);
  assert(context);
}

void GPUComputeImp::load_and_build_kernel(const char *fname, const char *kernel_name)
{
  int err;

  //create command queue
  cl_command_queue commands = clCreateCommandQueue(context, device_id, 0, &err);
  assert(commands);

  //load the file from disk
  FILE *f = fopen(fname, "r");
  if(f)
  {
    fseek(f, 0, SEEK_END);
    int string_size = ftell(f);
    rewind(f);

    char *kernel_src = (char *)malloc(sizeof(char) * (string_size + 1));
    memset(kernel_src, 0, string_size + 1);
    fread(kernel_src, sizeof(char), string_size, f);

    //create and build the kernel program executable and check for errors
    cl_program program = clCreateProgramWithSource(context, 1, (const char **) & kernel_src, NULL, &err);
    assert(program);

    err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
    if(err != CL_SUCCESS)
    {
      size_t len;
      char buffer[2048];
      cerr<<"GPUCompute: Failed to build kernel program executable!"<<endl;
      clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
      cerr<<buffer<<endl;
    }

    cl_kernel kernel = clCreateKernel(program, kernel_name, &err);
    assert(err == CL_SUCCESS && kernel);

    input_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(float) * num_elements, NULL, NULL);
    output_buffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(float) * num_elements, NULL, NULL);

    assert(input_buffer && output_buffer);
  }
  else
  {
    cerr<<"GPUCompute: could not open kernel file! (no file handle)"<<endl;
  }
}
