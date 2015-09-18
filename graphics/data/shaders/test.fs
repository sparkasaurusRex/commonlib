varying vec3 normal;
varying vec3 light_dir;

uniform vec3 diff_rgb;
uniform vec3 amb_rgb;

void main() {
	gl_FragColor = vec4(diff_rgb, 1.0f);
	float n_dot_l = pow(1.0f - clamp(dot(normal, light_dir), 0.0f, 0.2f), 1.5f);
	gl_FragColor = vec4(diff_rgb * n_dot_l, 1.0f);
}
