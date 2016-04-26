#ifndef __VR_H__
#define __VR_H__

namespace VR
{
  class VRContext
  {
  private:

  public:
    VRContext() {}
    ~VRContext() {}

    void init();
    void deinit();

    void simulate(const double game_time, const double frame_time);
  };
};

#endif //__VR_H__