#version 120

uniform sampler2D sprite_tex;

varying vec4 vertex_color;

void main() {

  gl_FragColor = texture2D(sprite_tex, gl_TexCoord[1].st) * vertex_color;
  
}
