#version 120

uniform sampler2D prev_pos_tex;
uniform sampler2D force_tex;
uniform sampler2D uv_tex;
uniform vec4 constants;

uniform float texel_size;

// F = -k(x - d) (Hooke's Spring Law)
void main()
{
  float dt = constants.x;
  float k = constants.y;
  float texel_size = constants.z;
  float spring_length = constants.w;
  float wind_strength = 0.5;
  float drag = 0.1;

  //use the position of root segment's xy coordinates as our uvs (TEMP)
  //vec2 force_uvs = texture2D(prev_pos_tex, vec2(gl_TexCoord[0].s, 0.0)).rb;
  //force_uvs = vec2(0.5 * force_uvs.x + 0.5, 0.5 * force_uvs.y + 0.5);
  vec3 force_uvs = texture2D(uv_tex, gl_TexCoord[0].st).rgb;
  vec4 prev_pos = texture2D(prev_pos_tex, gl_TexCoord[0].st);
  vec4 parent_pos = texture2D(prev_pos_tex, gl_TexCoord[0].st - vec2(0.0, texel_size));
  vec4 external_force = wind_strength * texture2D(force_tex, force_uvs.xy);

  spring_length = spring_length * external_force.w * force_uvs.z; //scale by height

  vec3 v_spring = prev_pos.xyz - parent_pos.xyz;
  float r = length(v_spring);

  vec3 up_force = vec3(0.0, 0.2, 0.0);  //TODO - hair surface normal?
  vec3 spring_force = -k * (v_spring / r) * (r - spring_length);
  vec3 final_force = dt * (external_force.xyz + spring_force + up_force);

  if(gl_TexCoord[0].t < texel_size)
  {
    final_force = vec3(0.0, 0.0, 0.0);
  }

  gl_FragColor = prev_pos + 1.0 * vec4(final_force, 0.0);// + offset;
}
