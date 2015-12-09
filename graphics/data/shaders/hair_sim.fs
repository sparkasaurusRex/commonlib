#version 120

uniform sampler2D prev_pos_tex;
uniform sampler3D force_tex;
uniform sampler3D forces;
uniform float dt;

void main()
{
	vec4 prev_pos = texture2D(prev_pos_tex, gl_TexCoord[0].st);
	vec3 force = texture3D(force_tex, prev_pos.xyz).rgb;
	force = 2.0 * (force - vec3(0.5, 0.5, 0.5));
	//vec4 offset = vec4(sin(dt), 0.0, 0.0, 0.0);
	gl_FragColor = prev_pos + 0.003 * vec4(force.xyz, 0.0);// + offset;
}
