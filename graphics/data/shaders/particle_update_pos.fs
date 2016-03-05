#version 120

#define NUM_CONSTANTS 5

uniform sampler2D prev_pos_tex;
uniform sampler2D vel_tex;

// contains rand floats from -1 to 1
uniform sampler2D data_tex;

//constants
//{dt, lifespan, does_loop, emitter_radius, game_time}
uniform float constants[NUM_CONSTANTS];

uniform vec3 emitter_location;

void main() {

  vec4 prev_pos = texture2D(prev_pos_tex, gl_TexCoord[0].st);
  vec3 velocity = texture2D(vel_tex, gl_TexCoord[0].st).xyz;

  float age = prev_pos.w;
  float dt = constants[0];
  float lifespan = constants[1];
  bool does_loop = (constants[2] == 1);
  float emitter_radius = constants[3];
  float game_time = constants[4];

  if (age < 0) {
    //Update age but do not update position
    gl_FragColor = vec4(prev_pos.xyz, age + dt);
  }
  else if (age < lifespan) {
    //Update new position and update particle age.
    gl_FragColor = vec4(prev_pos.xyz + dt * velocity, age + dt);
  }
  else if (does_loop) {
    //Respawn
    float seed = mod(gl_TexCoord[0].s + game_time * dt, 1.f);

    vec4 randVec4 = texture2D(data_tex, vec2(seed, 0.f));

    float randFloat = abs(randVec4.w);

    vec3 randVec3 = normalize(randVec4.xyz);

    vec3 respawn_point = emitter_location.xyz + emitter_radius * randVec3 * randFloat;

    gl_FragColor = vec4(respawn_point, 0.f);
  }
  else {
    //Dead
    gl_FragColor = vec4(0.f, 0.f, 0.f, age);
  }
}
