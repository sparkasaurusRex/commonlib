#version 120

uniform sampler2D prev_pos_tex;
uniform sampler2D force_tex;
uniform float dt;

void main()
{
	vec4 prev_pos = texture2D(prev_pos_tex, gl_TexCoord[0].st);
	//TEMP
	vec2 uvs = vec2(prev_pos.x, prev_pos.z);
	vec3 force = texture2D(force_tex, uvs).rgb;
	vec4 offset = vec4(0.02 * sin(3 * dt) * gl_TexCoord[0].t * gl_TexCoord[0].t, 0.0, 0.0, 0.0);
	gl_FragColor = prev_pos + 0.003 * vec4(gl_TexCoord[0].t * gl_TexCoord[0].t * force.xyz, 0.0);// + offset;
}
