#pragma once

#include <vector>
#include <stdio.h>

#include "mxml.h"
#include "widget.h"
#include "radio_group.h"

namespace UI
{
  //temporary storage to read from data files before creating actual game objects
  class LayoutWidgetTemplate
  {
  public:
    LayoutWidgetTemplate() {}
    ~LayoutWidgetTemplate() {}

    WidgetType type;

    std::string tool_tip;
    std::string tex_default;
    std::string tex_active;
    std::string tex_off;

    Math::Float2 dim;
    Math::Float2 offset;

    std::vector<LayoutWidgetTemplate *> children;
  };

  class Layout
  {
  protected:
    //std::vector<Widget *> widgets;
    std::vector<LayoutWidgetTemplate *> templates;

  public:
    Layout() {}
    ~Layout() {}

    void read_from_xml(mxml_node_t *root, uint32_t version);
    //void read_from_xml_string(char *xml_src);
    //void read_from_bin(void *data);

    void init();

  private:
    void parse_xml_level(mxml_node_t *root, LayoutWidgetTemplate *parent = NULL);
  };
};
