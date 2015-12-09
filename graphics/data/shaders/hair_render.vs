#version 120

uniform sampler2D hair_tex;
varying vec4 vertex_color;

void main()
{
  float displace_amt = 0.1;
  vertex_color = gl_Color;
  vec4 pos = gl_ModelViewMatrix * gl_Vertex;

  gl_TexCoord[0] = gl_MultiTexCoord0;

  vec4 pos_offset = texture2D(hair_tex, gl_TexCoord[0].st);

  vec4 displaced_pos = vec4(gl_Vertex.xyz + displace_amt * pos_offset.rgb, gl_Vertex.w);
  gl_Position = gl_ModelViewProjectionMatrix * displaced_pos;
}
