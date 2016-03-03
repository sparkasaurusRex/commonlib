#version 120

#define MAX_NUM_ATTRACTORS 5
#define MIN_DIST 0.2

uniform sampler2D prev_pos_tex;
uniform sampler2D vel_tex;
uniform sampler2D rand_tex;

uniform vec4 constants;
//{dt, lifespan, num_attractors, does_loop}
//uniform vec4 more_constants;
//{emitter_range, emitter_strength, ..., ...}

uniform vec3 emitter_direction;

uniform vec4 attractors[MAX_NUM_ATTRACTORS];

void main() {

  //vec3 emitter_direction = vec3(0.f, 1.f, 0.f);
  vec4 more_constants = vec4(0.3f, 1.2f, 0.f, 0.f);


  vec4 prev_pos = texture2D(prev_pos_tex, gl_TexCoord[0].st);
  vec3 velocity = texture2D(vel_tex, gl_TexCoord[0].st).xyz;

  float dt = constants.x;
  float lifespan = constants.y;
  int num_attractors = int(constants.z);
  bool does_loop = (constants.w == 1);
  float age = prev_pos.w;
  float emitter_range = more_constants.x;
  float emitter_strength = more_constants.y;

  if (age < 0) {
    //Do not update velocity.
    //velocity = vec3(1.f, 1.f, 0.f);
    gl_FragColor = vec4(velocity, 1.f);
  }
  else if (age < lifespan) {
    //Add forces here

    vec3 netForce = vec3(0.0, 0.0, 0.0);

    float r;

    vec3 rVector;

    for (int i = 0; i < num_attractors; i++) {

      rVector = attractors[i].xyz - prev_pos.xyz;

      r = length(rVector);

      rVector = normalize(rVector);

      if (r < MIN_DIST) {
        r = MIN_DIST;
      }

      netForce = netForce + attractors[i].w * rVector / (r * r);

    }

    vec3 new_velocity = velocity + dt * netForce;

    gl_FragColor = vec4(new_velocity, 1.f);
  }
  else if (does_loop) {
    //Respawn

    vec4 randVec4 = texture2D(rand_tex, gl_TexCoord[0].st);

    float randFloat = randVec4.w;

    vec3 velOffset = normalize(randVec4.xyz) * emitter_range * randFloat;

    //vec3 respawn_vel = normalize(emitter_direction + velOffset) * emitter_strength;

    vec3 respawn_vel;
    //respawn_vel = emitter_direction + velOffset;
    respawn_vel = velOffset;
    //respawn_vel = normalize(respawn_vel);
    //respawn_vel = respawn_vel + emitter_strength;

    gl_FragColor = vec4(respawn_vel, 1.0);
  }
  else {
    //Dead
    gl_FragColor = vec4(0.f, 0.f, 0.f, 1.f);
  }
}
