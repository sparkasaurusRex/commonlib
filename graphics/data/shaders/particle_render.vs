#version 120

uniform sampler2D particle_tex;

void main() {

  gl_TexCoord[0] = gl_MultiTexCoord0;
  
  vec4 pos_offset = texture2D(particle_tex, gl_TexCoord[0].st);
  
  vec4 pos = gl_Vertex + vec4(pos_offset.xyz, 0.0);

  gl_FrontColor = gl_Color
  
  gl_Position = gl_ModelViewProjectionMatrix * pos;
}