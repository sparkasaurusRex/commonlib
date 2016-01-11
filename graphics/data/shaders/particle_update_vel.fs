#version 120

#define MAX_NUM_ATTRACTORS 5
#define MIN_DIST 0.2

uniform sampler2D prev_pos_tex;
uniform sampler2D vel_tex;

uniform vec4 constants;
//{dt, lifespan, num_attractors, ...}

uniform vec4 attractors[MAX_NUM_ATTRACTORS];

void main() {
  
  float dt = constants.x;
  
  vec4 prev_pos = texture2D(prev_pos_tex, gl_TexCoord[0].st);
  vec3 velocity = texture2D(vel_tex, gl_TexCoord[0].st).xyz;
  
  if (prev_pos.w < 0 || prev_pos.w > constants.y) {
    //Do not update velocity.
    gl_FragColor = vec4(velocity, 1.0);
  }
  else {
    //Add forces here
    
    vec3 netForce = vec3(0.0, 0.0, 0.0);
    
    float r;
    
    vec3 rVector;
    
    for (int i = 0; i < int(constants.z); i++) {
      
      rVector = attractors[i].xyz - prev_pos.xyz;
      
      r = length(rVector);
      
      rVector = normalize(rVector);
      
      if (r < MIN_DIST) {
        r = MIN_DIST;
      }
      
      netForce = netForce + attractors[i].w * rVector / (r * r);
      
    }
    
    vec3 new_velocity = velocity + dt * netForce;
    
    gl_FragColor = vec4(new_velocity, 1.0);
  }
}