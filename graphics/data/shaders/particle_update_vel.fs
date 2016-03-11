#version 120

#define MAX_NUM_ATTRACTORS 5
#define MIN_DIST 0.2
#define NUM_CONSTANTS 9


uniform sampler2D prev_pos_tex;
uniform sampler2D vel_tex;
uniform sampler2D data_tex;

//{dt, lifespan, num_attractors, emitter_range, emitter_strength, sim_time, rand_curve_id, emitter_dir_id, data_tex_height}
uniform float constants[NUM_CONSTANTS];

uniform vec4 attractors[MAX_NUM_ATTRACTORS];

void main() {

  vec4 prev_pos = texture2D(prev_pos_tex, gl_TexCoord[0].st);
  vec3 velocity = texture2D(vel_tex, gl_TexCoord[0].st).xyz;

  float age = prev_pos.w;
  float dt = constants[0];
  float lifespan = constants[1];
  int num_attractors = int(constants[2]);
  float emitter_range = constants[3];
  float emitter_strength = constants[4];
  float sim_time = constants[5];

  if (age < 0) {
    //Unborn. Do not update velocity.
    gl_FragColor = vec4(velocity, 1.f);
  }
  else if (age == 0) {
    //Spawn or Respawn.
    int rand_curve_id = int(constants[6]);
    int emitter_dir_id = int(constants[7]);
    int data_tex_height = int(constants[8]);

    float seed = mod(gl_TexCoord[0].s + sim_time, (rand_curve_id + 0.5f) / data_tex_height);

    vec4 randVec4 = texture2D(data_tex, vec2(seed, 0.f));

    vec2 emitter_dir_uv = vec2(gl_TexCoord[0].s, (emitter_dir_id + 0.5f) / data_tex_height);
    vec3 emitter_direction = texture2D(data_tex, emitter_dir_uv).xyz;
    //Curve values are [0,1]. But we need values [-1,1].
    emitter_direction = emitter_direction + vec3(-0.5f, -0.5f, -0.5f);

    float randFloat = randVec4.w;

    vec3 velOffset = normalize(randVec4.xyz) * emitter_range * randFloat;

    vec3 respawn_vel = normalize(emitter_direction + velOffset) * emitter_strength;

    gl_FragColor = vec4(respawn_vel, 1.0);
  }
  else if (age < lifespan) {
    //Update velocities
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
  else {
    //Dead
    gl_FragColor = vec4(0.f, 0.f, 0.f, 1.f);
  }
}
