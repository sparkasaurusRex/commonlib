#include "radial_widget.h"

using namespace UI;
using namespace Math;
using namespace std;

bool RadialWidget::hit_test(const int x, const int y) const
{
  Float2 dpos = Float2(x, y) - center;
  float r = dpos.magnitude();
  dpos = dpos / r; //normalize

  float theta = atan2(dpos[1], dpos[0]);
  if(theta < 0.0f) { theta = 2.0f * M_PI + theta; }

  //if(arc[0] < arc[1])
  {
    return ((r > radii[0]) && (r < radii[1]) &&
          (theta > arc[0]) && (theta < arc[1]));
  }
  // else
  // {
  //   return ((r > radii[0]) && (r < radii[1]) &&
  //         (theta < arc[0]) && (theta > arc[1]));
  // }
}
