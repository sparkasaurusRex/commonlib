#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "math_utility.h"

class Camera
{
public:
  Camera();
  ~Camera() {}

  void set_pos(const Math::Float3 p) { pos = p; }
  void set_up(const Math::Float3 u) { up = u; }
  void set_lookat(const Math::Float3 l) { lookat = l; }

  void set_window_dimensions(const Math::Float2 d) { window_dimensions = d; }
  void set_f_stop(const float f) { f_stop = f; }
  void set_focal_distance(const float d) { focal_distance_m = d; }
  void set_focal_length(const float l) { focal_length_mm = l; }
  void set_shutter_speed(const float s) { shutter_speed_s = s; }

  void set_camera_parameters(const float fs, const float fd, const float fl, const float ss);

  void render_setup();
  void render_cleanup();
private:
  Math::Float3 pos;
  Math::Float3 up;
  Math::Float3 lookat;

  Math::Float2 window_dimensions;
  float f_stop;             //aperture size
  float focal_distance_m;   //distance that we are focused on
  float focal_length_mm;    //focal length (zoom)
  float shutter_speed_s;    //how long the shutter is open (motion_blur)

  //std::vector<ScreenEffect *> effects;
  //float spherical_aberration;
  //float vignetting;
  //Float3 tint;
};

#endif //_CAMERA_H__
