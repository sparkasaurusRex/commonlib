#version 120

uniform sampler2D particle_tex;

varying vec4 vertex_color;

void main() {
  
  vec4 pos_center = texture2D(particle_tex, gl_MultiTexCoord0.st);
  
  mat4 identityModelView = gl_ModelViewMatrix;
  
  identityModelView[0][0] = 1.0;
  identityModelView[0][1] = 0.0;
  identityModelView[0][2] = 0.0;
  identityModelView[1][0] = 0.0;
  identityModelView[1][1] = 1.0;
  identityModelView[1][2] = 0.0;
  identityModelView[2][0] = 0.0;
  identityModelView[2][1] = 0.0;
  identityModelView[2][2] = 1.0;
  
  gl_Position = gl_ProjectionMatrix * identityModelView * gl_Position;
  
  vertex_color = vec4(1.0, 0.0, 0.0, 1.0);
}