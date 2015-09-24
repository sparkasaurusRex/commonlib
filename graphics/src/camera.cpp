#include "camera.h"

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

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

void Camera::transform(const Matrix3x3 &m)
{
  pos = m * pos;
  up = m * up;
  lookat = m * lookat;
}

void Camera::set_camera_parameters(const float fs, const float fd, const float fl, const float ss)
{
  f_stop = fs;
  focal_distance_m = fd;
  focal_length_mm = fl;
  shutter_speed_s = ss;
}

void Camera::render_setup()
{
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
    glLoadIdentity();
    gluPerspective(32, window_dimensions[0] / window_dimensions[1], 0.05f, 1000.0f);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
      glLoadIdentity();

      Float3 lookat_pos = pos + lookat;
      gluLookAt(pos[0], pos[1], pos[2],
                lookat_pos[0], lookat_pos[1], lookat_pos[2],
                up[0], up[1], up[2]);
}

void Camera::render_cleanup()
{
    glPopMatrix();
  glPopMatrix();
}
