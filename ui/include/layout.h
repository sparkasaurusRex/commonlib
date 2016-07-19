#pragma once

#include <vector>
#include <stdio.h>

#include "mxml.h"
#include "widget.h"
#include "radio_group.h"

namespace UI
{
  enum WidgetType
  {
    WIDGET_TOOLBOX,
    WIDGET_RADIO_GROUP,
    WIDGET_PUSH_BUTTON,
    WIDGET_CHECK_BUTTON
  };

  //temporary storage to read from data files before creating actual game objects
  class WidgetTemplate
  {
  public:
    WidgetType type;

    std::string tool_tip;
    std::string tex_default;
    std::string tex_active;
    std::string tex_off;

    Float2 dim;
    Float2 offset;

    std::vector<WidgetTemplate *> children;
  };

  class Layout
  {
  protected:
    std::vector<Widget *> widgets;

  public:
    Layout() {}
    ~Layout() {}

    void read_from_xml(mxml_node_t *root, uint32_t version);
    //void read_from_xml_string(char *xml_src);
    //void read_from_bin(void *data);

    void init();

  private:
    void parse_xml_level(mxml_node_t *root, Widget *parent = NULL, RadioGroup *radio_group = NULL);
  };
};
