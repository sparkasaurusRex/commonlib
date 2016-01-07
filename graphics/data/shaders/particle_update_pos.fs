#version 120

uniform sampler2D prev_pos_tex;
uniform sampler2D vel_tex;


void main() {
  
  float dt = 0.01;
  
  vec4 prev_pos = texture2D(prev_pos_tex, gl_TexCoord[0].st);
  vec4 velocity = texture2D(vel_tex, gl_TexCoord[1].st);
  
  
  gl_FragColor = vec4(prev_pos.xyz + dt * velocity.xyz, prev_pos.w + dt);
  //update new position and update particle age
}