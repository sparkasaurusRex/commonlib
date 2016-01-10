#version 120

void main() {
  
  gl_TexCoord[0] = gl_MultiTexCoord0; //position data
  //gl_TexCoord[1] = gl_MultiTexCoord1; //velocity data
  
  gl_Position = ftransform();
}