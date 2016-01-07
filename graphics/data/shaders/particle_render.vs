#version 120

uniform sampler2D particle_tex;

varying vec4 vertex_color;

void main() {
  
  vec4 pos_offset = texture2D(particle_tex, gl_MultiTexCoord0.st);
  
  vec4 pos = gl_Vertex + vec4(pos_offset.xyz, 1.0);
  
  gl_Position = gl_ModelViewProjectionMatrix * pos;
  
  vertex_color = vec4(1.0, 0.0, 0.0, 1.0);
  
  //gl_Position = gl_ModelViewProjectionMatrix * vec4(0.0, 0.0, 0.0, 1.0);
}