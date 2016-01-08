#version 120

uniform sampler2D particle_tex;

varying vec4 vertex_color;

void main() {
  
  vec3 pos_center = texture2D(particle_tex, gl_MultiTexCoord0.st).xyz;
  
  mat4 ModelMatrix = gl_ModelViewMatrix;
  
  ModelMatrix[0].xyz = vec3(1.0, 0.0, 0.0);
  ModelMatrix[1].xyz = vec3(0.0, 1.0, 0.0);
  ModelMatrix[2].xyz = vec3(0.0, 0.0, 1.0);
  
  gl_Position = gl_ProjectionMatrix * ModelMatrix * (gl_Vertex + vec4(pos_center, 1.0));
  //gl_Position = gl_ModelViewProjectionMatrix * vec4(pos_center, 1.0);

  vertex_color = gl_Color;
  //vertex_color = vec4(1.0, 1.0, 0.0, 1.0);
}