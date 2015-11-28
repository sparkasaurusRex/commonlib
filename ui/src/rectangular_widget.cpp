#include "rectangular_widget.h"

using namespace UI;

bool RectangularWidget::hit_test(const int x, const int y) const
{
  return ((x > pos[0]) && (x < (pos[0] + dim[0])) &&
          (y < pos[1]) && (y > (pos[1] - dim[1])));
}
