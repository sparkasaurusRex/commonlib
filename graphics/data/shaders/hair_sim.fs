#version 120

uniform sampler2D prev_pos_tex;
uniform sampler2D force_tex;
uniform float dt;

void main()
{
	vec4 prev_pos = texture2D(prev_pos_tex, gl_TexCoord[0].st);
	//vec3 force = texture3D(force_tex, ).rgb;
	vec4 offset = vec4(0.02 * sin(3 * dt) * gl_TexCoord[0].t, 0.0, 0.0, 0.0);
	gl_FragColor = prev_pos + offset;// + 1.0 * vec4(force.xyz, 0.0);// + offset;
}
