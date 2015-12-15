#version 120

uniform sampler2D hair_tex;
uniform sampler2D uv_tex;
uniform sampler2D color_tex;
uniform vec3 sun_pos;
uniform vec3 sun_diff_rgb;
uniform vec3 light_amb_rgb;

varying vec4 vertex_color;
varying vec3 view_dir;
varying vec3 light_dir;

void main()
{
  gl_TexCoord[0] = gl_MultiTexCoord0;

  view_dir = -vec3(gl_ProjectionMatrix[2][0], gl_ProjectionMatrix[2][1], gl_ProjectionMatrix[2][2]);

  vec4 pos_offset = texture2D(hair_tex, gl_TexCoord[0].st);
  vec3 uvs = texture2D(uv_tex, gl_TexCoord[0].st).rgb;

  vec4 pos = gl_Vertex + pos_offset;
  vec3 n = normalize(gl_NormalMatrix * pos.xyz);

  light_dir = normalize(sun_pos - pos.xyz);
  float n_dot_l = clamp(dot(n, light_dir), 0.0, 1.0);
  vec3 diffuse_term = n_dot_l * sun_diff_rgb;
  vec3 tint_color = texture2D(color_tex, uvs.xy).rgb;
  vec3 ambient_term = light_amb_rgb;
  vertex_color = vec4(diffuse_term * tint_color * gl_Color.rgb + ambient_term * tint_color, 1.0);

  gl_Position = gl_ModelViewProjectionMatrix * pos;
}
