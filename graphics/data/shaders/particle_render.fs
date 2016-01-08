#version 120

varying vec4 vertex_color;

void main() {
  //gl_FragColor = gl_Color;
  //gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
  
  gl_FragColor = vertex_color;
}