#version 120

uniform sampler2D prev_pos_tex;
uniform sampler2D vel_tex;

uniform vec4 constants;


void main() {
  
  float dt = clamp(constants.x, 0.0, 0.02);
    
  vec4 prev_pos = texture2D(prev_pos_tex, gl_TexCoord[0].st);
  vec3 velocity = texture2D(vel_tex, gl_TexCoord[0].st).xyz;
  
  
  gl_FragColor = vec4(prev_pos.xyz + dt * velocity, prev_pos.w + dt);
  //update new position and update particle age
}