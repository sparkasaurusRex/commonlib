#version 120

uniform sampler2D hair_tex;
uniform sampler2D uv_tex;
uniform sampler2D color_tex;

varying vec4 vertex_color;

void main()
{
  gl_TexCoord[0] = gl_MultiTexCoord0;

  vec4 pos_offset = texture2D(hair_tex, gl_TexCoord[0].st);
  vec4 pos = gl_Vertex + pos_offset;

  vec2 uvs = texture2D(uv_tex, gl_TexCoord[0].st).rg;
  vec4 tint_color = texture2D(color_tex, uvs);
  vertex_color = tint_color * gl_Color;

  gl_Position = gl_ModelViewProjectionMatrix * pos;
}
