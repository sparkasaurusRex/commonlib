#pragma once

#include <vector>
#include <stdio.h>

#include "mxml.h"
#include "widget.h"
#include "radio_group.h"

#define UI_LAYOUT_FLAG_VERTICAL       1<<0
#define UI_LAYOUT_FLAG_ALIGN_CENTER   1<<1

namespace UI
{
  //temporary storage to read from data files before creating actual game objects
  class LayoutWidgetTemplate
  {
  public:
    LayoutWidgetTemplate() { real_widget = NULL; flags = 0; }
    ~LayoutWidgetTemplate() {}

    WidgetType type;
    uint32_t flags;

    std::string tool_tip;
    std::string tex_default;
    std::string tex_active;
    std::string tex_off;

    std::string click_callback;

    Math::Float2 dim;
    Math::Float2 offset;

    std::vector<LayoutWidgetTemplate *> children;
    LayoutWidgetTemplate *parent;

    Widget *real_widget; //used later to fix everything up
  };

  class Layout
  {
  public:
    Layout() {}
    ~Layout() {}

    void read_from_xml(mxml_node_t *root, uint32_t version);

    std::vector<LayoutWidgetTemplate *> templates;
  private:
    void parse_xml_level(mxml_node_t *root, LayoutWidgetTemplate *parent = NULL, LayoutWidgetTemplate *radio_group = NULL);
  };
};
