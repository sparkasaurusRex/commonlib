#version 120

uniform sampler2D sprite_tex;

varying vec4 vertex_color;

void main() {
  
  gl_FragColor = texture2D(sprite_tex, gl_TexCoord[3].st) * vertex_color;
  
  //gl_FragColor = vertex_color;
  //gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}