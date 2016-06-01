#ifndef __RADIO_GROUP_H__
#define __RADIO_GROUP_H__

#include <vector>

#include "check_button.h"

namespace UI
{
  class RadioGroup
  {
  private:
    std::vector<CheckButton *> group;
  public:
    RadioGroup() {}
    ~RadioGroup() {}

    void add(CheckButton *c) { group.push_back(c); }
    void set_active(CheckButton *c)
    {
      c->set_state(true);
      for (unsigned int i = 0; i < group.size(); i++)
      {
        if (group[i] != c)
        {
          group[i]->set_state(false);
        }
      }
    }
  };
};

#endif //__RADIO_GROUP_H__
