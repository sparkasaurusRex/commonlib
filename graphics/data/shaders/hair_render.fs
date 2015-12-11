#version 120

varying vec4 vertex_color;

void main()
{
  //vec4 tint = texture2D(color_tex, gl_TexCoord[0].st);
  gl_FragColor = vertex_color;
}
