#include <iostream>
#include "layout.h"
#include "platform.h"
#include "toolbox.h"
#include "check_button.h"
#include "radio_group.h"

using namespace UI;
using namespace std;
using namespace Math;

void Layout::parse_xml_level(mxml_node_t *root, LayoutWidgetTemplate *parent)
{
  mxml_node_t *start_node = root->child;
  const char *buffer = NULL;

  //parse toolboxes
  mxml_node_t *toolbar_node = NULL;
  do
  {
    //use MXML_NO_DESCEND so we stay at the top level
    toolbar_node = mxmlFindElement(start_node, root, "toolbar", NULL, NULL, MXML_NO_DESCEND);
    if (toolbar_node)
    {
      cout << "adding toolbar..." << endl;
      //ToolBox *tb = new ToolBox;
      LayoutWidgetTemplate *tb = new LayoutWidgetTemplate;
      tb->type = WIDGET_TOOLBOX;

      buffer = mxmlElementGetAttr(toolbar_node, "name");
      cout << "\tname: " << buffer << endl;

      buffer = mxmlElementGetAttr(toolbar_node, "orientation");
      cout << "\torientation: " << buffer << endl;

      buffer = mxmlElementGetAttr(toolbar_node, "x");
      uint32_t x_offset = atoi(buffer);
      buffer = mxmlElementGetAttr(toolbar_node, "y");
      uint32_t y_offset = atoi(buffer);

      cout << "\toffset: (" << x_offset << ", " << y_offset << ")" << endl;

      //tb->translate(Float2((float)x_offset, (float)y_offset));
      tb->offset = Float2((float)x_offset, (float)y_offset);
      templates.push_back(tb);

      //widgets.push_back(tb);

      parse_xml_level(toolbar_node, tb);
    }
    start_node = toolbar_node;
  } while (toolbar_node);

  //parse radio groups
  start_node = root->child;
  mxml_node_t *radio_group_node = NULL;
  do
  {
    radio_group_node = mxmlFindElement(start_node, root, "radio_group", NULL, NULL, MXML_NO_DESCEND);
    if (radio_group_node)
    {
      cout << "adding radio group" << endl;
      //RadioGroup *rg = new RadioGroup;
      LayoutWidgetTemplate *rg = new LayoutWidgetTemplate;
      rg->type = WIDGET_RADIO_GROUP;

      if (parent)
      {
        cout << "parent detected..." << endl;
        parent->children.push_back(rg);
      }

      parse_xml_level(radio_group_node, parent);
    }
    start_node = radio_group_node;
  } while (radio_group_node);

  //parse check button widgets
  start_node = root->child;
  mxml_node_t *check_button_node = NULL;
  do
  {
    check_button_node = mxmlFindElement(start_node, root, "check_button", NULL, NULL, MXML_NO_DESCEND);
    if (check_button_node)
    {
      cout << "adding checkbutton..." << endl;
      //CheckButton *cb = new CheckButton;
      LayoutWidgetTemplate *cb = new LayoutWidgetTemplate;
      cb->type = WIDGET_CHECK_BUTTON;

      buffer = mxmlElementGetAttr(check_button_node, "name");
      cout << "\tname: " << buffer << endl;

      buffer = mxmlElementGetAttr(check_button_node, "width");
      uint32_t cb_width = atoi(buffer);
      buffer = mxmlElementGetAttr(check_button_node, "height");
      uint32_t cb_height = atoi(buffer);
      cout << "\tdim: (" << cb_width << ", " << cb_height << ")" << endl;
      cb->dim = Float2((float)cb_width, (float)cb_height);

      if (check_button_node->child)
      {

        //tool tip
        mxml_node_t *child_node = mxmlFindElement(check_button_node->child, root, "tool_tip", NULL, NULL, MXML_NO_DESCEND);
        if (child_node)
        {
          buffer = mxmlGetOpaque(child_node);
          if (buffer)
          {
            cout << "\ttool tip: " << buffer << endl;
            cb->tool_tip = buffer;//set_tooltip(buffer);
          }
        }

        //default texture
        child_node = mxmlFindElement(check_button_node->child, root, "tex_default", NULL, NULL, MXML_NO_DESCEND);
        if (child_node)
        {
          buffer = mxmlGetOpaque(child_node);
          if (buffer)
          {
            cout << "\tdefault texture: " << buffer << endl;
          }
        }

        //active texture
        child_node = mxmlFindElement(check_button_node->child, root, "tex_active", NULL, NULL, MXML_NO_DESCEND);
        if (child_node)
        {
          buffer = mxmlGetOpaque(child_node);
          if (buffer)
          {
            cout << "\tactive texture: " << buffer << endl;
          }
        }

        //"off" texture
        child_node = mxmlFindElement(check_button_node->child, root, "tex_off", NULL, NULL, MXML_NO_DESCEND);
        if (child_node)
        {
          buffer = mxmlGetOpaque(child_node);
          if (buffer)
          {
            cout << "\toff texture: " << buffer << endl;
          }
        }

        //callback function
        child_node = mxmlFindElement(check_button_node->child, root, "callback", NULL, NULL, MXML_NO_DESCEND);
        if (child_node)
        {
          buffer = mxmlGetOpaque(child_node);
          if (buffer)
          {
            cout << "\tcallback_function: " << buffer << endl;
          }
        }
      }

      if (parent)
      {
        cout << "\tparent detected..." << endl;
        parent->children.push_back(cb);
      }

      templates.push_back(cb);
    }
    start_node = check_button_node;
  } while (check_button_node);
}

void Layout::read_from_xml(mxml_node_t *root, uint32_t version)
{
  cout << "loading UI layout from xml..." << endl;
  cout << "\tversion: " << version << endl;

  mxml_node_t *layout_node = mxmlFindElement(root, root, "ui_layout", NULL, NULL, MXML_DESCEND);
  if (layout_node)
  {
    parse_xml_level(layout_node);
  }
  else
  {
    SET_TEXT_COLOR(CONSOLE_COLOR_RED);
    cout << "Could not find root ui_layout node!" << endl;
    SET_TEXT_COLOR(CONSOLE_COLOR_RED);
  }
}