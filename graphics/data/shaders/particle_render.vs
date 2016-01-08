#version 120

uniform sampler2D particle_tex;

varying vec4 vertex_color;

void main() {
  
  vec3 pos_center = texture2D(particle_tex, gl_MultiTexCoord0.st).xyz;
  
  gl_Position = gl_ModelViewProjectionMatrix * vec4(pos_center, 1.0);

  vertex_color = vec4(1.0, 1.0, 0.0, 1.0);
}