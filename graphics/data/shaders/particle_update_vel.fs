#version 120

uniform sampler2D prev_pos_tex;
uniform sampler2D vel_tex;

void main() {
  
  float dt = 0.01;
  
  vec4 prev_pos = texture2D(prev_pos_tex, gl_TexCoord[0].st);
  vec3 velocity = texture2D(vel_tex, gl_TexCoord[1].st).xyz;
  
  //Add forces here
  
  vec3 netForce = vec3(0.0, 0.0, 0.0);
  
  vec3 gravity = vec3(0.0, 1.0, 0.0);
  
  netForce = netForce + gravity;
  
  vec3 new_velocity = velocity + dt * netForce;
  
  gl_FragColor = vec4(new_velocity, 0.0);
}