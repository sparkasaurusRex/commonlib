#include <iostream>
#include "label.h"

using namespace UI;
using namespace std;

void Label::render()
{
  if(visible)
  {
    //cout<<text.c_str()<<endl;
    render_prep();
    glColor3f(1.0f, 1.0f, 1.0f);
    font->print(pos[0], pos[1], text.c_str());
    render_cleanup();
  }
}
