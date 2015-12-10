#version 120

uniform sampler2D prev_pos_tex;
uniform sampler2D force_tex;
uniform vec4 constants;

uniform float texel_size;

// F = -k(x - d) (Hooke's Spring Law)
void main()
{
  float dt = constants.x;
  float k = 50.0;//constants.y;
  float texel_size = constants.z;
  float spring_length = constants.w;

  vec4 prev_pos = texture2D(prev_pos_tex, gl_TexCoord[0].st);
  vec4 parent_pos = texture2D(prev_pos_tex, gl_TexCoord[0].st - vec2(0.0, texel_size));
  vec3 external_force = texture2D(force_tex, vec2(prev_pos.x, prev_pos.z)).rgb;

  vec3 v_spring = prev_pos.xyz - parent_pos.xyz;
  float r = length(v_spring);

	vec3 up_force = vec3(0.0, 1.0, 0.0);
  vec3 spring_force = -k * (v_spring / r) * (r - spring_length);
	vec3 final_force = dt * (external_force + spring_force + up_force);

	if(gl_TexCoord[0].t < 0.1)
	{
		final_force = vec3(0.0, 0.0, 0.0);
	}

  gl_FragColor = prev_pos + 1.0 * vec4(final_force, 0.0);// + offset;
}
