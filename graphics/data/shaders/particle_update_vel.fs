#version 120

#define MAX_NUM_ATTRACTORS 5
#define MIN_DIST 0.2
#define NUM_CONSTANTS 7

uniform sampler2D prev_pos_tex;
uniform sampler2D vel_tex;
uniform sampler2D data_tex;

uniform vec3 emitter_direction;

//{dt, lifespan, num_attractors, does_loop, emitter_range, emitter_strength, game_time}
uniform float constants[NUM_CONSTANTS];

uniform vec4 attractors[MAX_NUM_ATTRACTORS];

void main() {

  vec4 prev_pos = texture2D(prev_pos_tex, gl_TexCoord[0].st);
  vec3 velocity = texture2D(vel_tex, gl_TexCoord[0].st).xyz;

  float age = prev_pos.w;
  float dt = constants[0];
  float lifespan = constants[1];
  int num_attractors = int(constants[2]);
  bool does_loop = (constants[3] == 1);
  float emitter_range = constants[4];
  float emitter_strength = constants[5];
  float game_time = constants[6];

  if (age < 0) {
    //Do not update velocity.
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
    float seed = mod(gl_TexCoord[0].s + game_time * dt, 1.f);

    vec4 randVec4 = texture2D(data_tex, vec2(seed, 0.f));

    float randFloat = randVec4.w;

    vec3 velOffset = normalize(randVec4.xyz) * emitter_range * randFloat;

    vec3 respawn_vel = normalize(emitter_direction + velOffset) * emitter_strength;

    gl_FragColor = vec4(respawn_vel, 1.0);
  }
  else {
    //Dead
    gl_FragColor = vec4(0.f, 0.f, 0.f, 1.f);
  }
}
