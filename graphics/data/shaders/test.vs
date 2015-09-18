varying vec3 normal;
varying vec3 light_dir;

void main() {
	normal = gl_NormalMatrix * gl_Normal;
	light_dir = gl_NormalMatrix * vec3(0.0f, -1.0f, 0.0f);
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
