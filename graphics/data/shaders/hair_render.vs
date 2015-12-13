#version 120

uniform sampler2D hair_tex;
uniform sampler2D uv_tex;
uniform sampler2D color_tex;

varying vec4 vertex_color;

void main()
{
  gl_TexCoord[0] = gl_MultiTexCoord0;

  vec4 pos_offset = texture2D(hair_tex, gl_TexCoord[0].st);
  vec3 uvs = texture2D(uv_tex, gl_TexCoord[0].st).rgb;

  vec4 pos = gl_Vertex + pos_offset;
  vec3 n = pos.xyz;
  vec3 l = vec3(0.707, 0.707, 0.0);
  float n_dot_l = clamp(dot(n, l), 0.0, 1.0);
  vec3 tint_color = texture2D(color_tex, uvs.xy).rgb;
  vec3 ambient_term = vec3(0.1, 0.1, 0.1);
  vertex_color = vec4(n_dot_l * tint_color * gl_Color.rgb + ambient_term, 1.0);

  gl_Position = gl_ModelViewProjectionMatrix * pos;
}
