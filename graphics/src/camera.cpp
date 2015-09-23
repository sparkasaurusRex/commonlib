#include "camera.h"

using namespace Math;

Camera::Camera()
{
  pos = Float3(0.0f, 0.0f, -10.0f);
  up = Float3(0.0f, 1.0f, 0.0f);
  lookat = Float3(0.0f, 0.0f, 1.0f);

  f_stop = 16.0f;
  focal_distance_m = 10000.0f;
  focal_length_mm = 35.0f;
  shutter_speed_s = 0.01f;
}

void Camera::set_camera_parameters(const float fs, const float fd, const float fl, const float ss)
{
  f_stop = fs;
  focal_distance_m = fd;
  focal_length_mm = fl;
  shutter_speed_s = ss;
}
