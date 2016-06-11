#include <sstream>
#include <iomanip>
#include "tool.h"

std::string Tool::mxml_read_text(mxml_node_t *node)
{
  assert(node);

  std::string text_string;
  const char *buff;
  mxml_node_t *text_node = mxmlGetFirstChild(node);
  while (text_node)
  {
    buff = mxmlGetText(text_node, NULL);
    if (buff)
    {
      text_string = text_string + std::string(" ") + buff;
    }
    text_node = mxmlGetNextSibling(text_node);
  }

  return text_string;
}

Math::Float2 Tool::mxml_read_float2(const mxml_node_t *node)
{
  assert(node);
  assert(node->next);

  Math::Float2 val((float)atof(node->value.text.string),
                   (float)atof(node->next->value.text.string));

  return val;
}

Math::Float3 Tool::mxml_read_float3(const mxml_node_t *node)
{
  assert(node);
  assert(node->next);
  assert(node->next->next);

  Math::Float3 val((float)atof(node->value.text.string),
                   (float)atof(node->next->value.text.string),
                   (float)atof(node->next->next->value.text.string));

  return val;
}

std::string Tool::make_nice_filename(std::string prefix, std::string suffix, int counter)
{
  std::ostringstream oss;
  //oss.setfill(0);
  //oss.setw(4);
  oss<<prefix<<std::setfill('0')<<std::setw(5)<<counter<<suffix;
  return oss.str();
}
