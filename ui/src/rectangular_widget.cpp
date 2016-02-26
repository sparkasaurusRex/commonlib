#include "rectangular_widget.h"

#include <iostream>

using namespace UI;
using namespace std;

bool RectangularWidget::hit_test(const int x, const int y) const
{
  bool hit =  ((x > pos[0]) && (x < (pos[0] + dim[0])) &&
          (y < pos[1]) && (y > (pos[1] - dim[1])));

  if(hit)
  {
    cout<<"hit: "<<pos<<endl;
  }
  return hit;
}
