#version 120

uniform sampler2D prev_pos_tex;
uniform sampler2D vel_tex;

uniform vec4 constants;

void main() {
  
  float dt = clamp(constants.x, 0.0, 0.02);
  
  vec4 prev_pos = texture2D(prev_pos_tex, gl_TexCoord[0].st);
  vec3 velocity = texture2D(vel_tex, gl_TexCoord[0].st).xyz;
  
  //Add forces here
  
  vec3 r = prev_pos.xyz;
  
  vec3 netForce = -0.5 * r / (length(r) * length(r));
  
  //netForce = clamp(netForce, -1, 1);
  
  vec3 new_velocity = velocity + dt * netForce;
  
  gl_FragColor = vec4(new_velocity, 1.0);
}