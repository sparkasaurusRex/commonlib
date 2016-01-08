#version 120

uniform sampler2D particle_tex;

varying vec4 vertex_color;

void main() {
  
  vec4 pos_center = texture2D(particle_tex, gl_MultiTexCoord0.st);
  
  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
  
  vertex_color = vec4(1.0, 1.0, 1.0, 1.0);
}