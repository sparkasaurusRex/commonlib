#version 120

#define NUM_CONSTANTS 9

uniform sampler2D prev_pos_tex;
uniform sampler2D vel_tex;

//contains data texture
uniform sampler2D data_tex;

//constants
//{dt, lifespan, does_loop, emitter_radius, emitter_duration, sim_time, rand_curve_id, age_curve_id, data_tex_height}
uniform float constants[NUM_CONSTANTS];

uniform vec3 emitter_location;

float dt = constants[0];
float lifespan = constants[1];
bool does_loop = (constants[2] == 1);
float emitter_radius = constants[3];
float sim_time = constants[5];
int data_tex_height = int(constants[8]);

vec4 spawn();

void main() {

  vec4 prev_pos = texture2D(prev_pos_tex, gl_TexCoord[0].st);

  float age = prev_pos.w;


  if (age < 0) {
    //Unborn particles
    float emitter_duration = constants[4];
    int age_curve_id = int(constants[7]);
    vec2 age_curve_uv = vec2(sim_time / emitter_duration, (age_curve_id + 0.5f) / data_tex_height);
    float particles_to_be_born = texture2D(data_tex, age_curve_uv).x;

    if (gl_TexCoord[0].s < particles_to_be_born) {
      //A particle is born!
      gl_FragColor = spawn();
    }
    else {
      //Still unborn
      gl_FragColor = vec4(prev_pos.xyz, -1);
    }
  }
  else if (age < lifespan) {
    //Update new position and update particle age.
    vec3 velocity = texture2D(vel_tex, gl_TexCoord[0].st).xyz;

    gl_FragColor = vec4(prev_pos.xyz + dt * velocity, age + dt);
  }
  else if (does_loop) {
    //Respawn
    gl_FragColor = spawn();
  }
  else {
    //Dead
    gl_FragColor = vec4(0.f, 0.f, 0.f, age);
  }
}

vec4 spawn() {

  int rand_curve_id = int(constants[6]);

  float seed = mod(gl_TexCoord[0].s + sim_time, 1);

  vec4 randVec4 = texture2D(data_tex, vec2(seed, (rand_curve_id + 0.5f) / data_tex_height));

  float randFloat = abs(randVec4.w);

  vec3 randVec3 = normalize(randVec4.xyz);

  return vec4(emitter_location.xyz + emitter_radius * randVec3 * randFloat, 0.f);

}
