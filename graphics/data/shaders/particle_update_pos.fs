#version 120

uniform sampler2D prev_pos_tex;
uniform sampler2D vel_tex;

uniform vec4 constants;
//{dt, lifespan, ..., ...}

uniform vec3 emitterLocation;

void main() {
  
  float dt = constants.x;
    
  vec4 prev_pos = texture2D(prev_pos_tex, gl_TexCoord[0].st);
  vec3 velocity = texture2D(vel_tex, gl_TexCoord[0].st).xyz;
  
  if (prev_pos.w < 0) {
    //Update age but do not update position
    gl_FragColor = vec4(prev_pos.xyz, prev_pos.w + dt);
  }
  else if (prev_pos.w > constants.y) {
    gl_FragColor = vec4(emitterLocation.xyz, 0);
  }
  else {
    //Update new position and update particle age.
    gl_FragColor = vec4(prev_pos.xyz + dt * velocity, prev_pos.w + dt);
  }
}