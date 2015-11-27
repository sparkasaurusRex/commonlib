#version 120

uniform sampler2D tex_a;
uniform sampler2D tex_b;

void main()
{
  vec3 a_col = texture2D(tex_a, gl_TexCoord[0].st).rgb;
  vec3 b_col = texture2D(tex_b, gl_TexCoord[0].st).rgb;

  gl_FragColor = vec4(b_col, 1.0);//vec4(a_col + b_col, 1.0);
}
