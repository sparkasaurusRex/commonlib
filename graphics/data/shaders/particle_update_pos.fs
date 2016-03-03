#version 120

uniform sampler2D prev_pos_tex;
uniform sampler2D vel_tex;
// contains rand floats from -1 to 1
uniform sampler2D rand_tex;

uniform vec4 constants;
//{dt, lifespan, does_loop, emitter_radius}

uniform vec3 emitterLocation;

void main() {

  float dt = constants.x;
  float lifespan = constants.y;
  bool does_loop = (constants.z == 1);

  vec4 prev_pos = texture2D(prev_pos_tex, gl_TexCoord[0].st);
  vec3 velocity = texture2D(vel_tex, gl_TexCoord[0].st).xyz;

  float age = prev_pos.w;

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
    //float emitterRadius = 0.15f;
    float emitter_radius = constants.w;

    vec4 randVec4 = texture2D(rand_tex, gl_TexCoord[0].st);

    float randFloat = abs(randVec4.w);

    vec3 randVec3 = normalize(randVec4.xyz);

    vec3 respawn_point = emitterLocation.xyz + emitter_radius * randVec3 * randFloat;

    gl_FragColor = vec4(respawn_point, 0.f);
  }
  else {
    //Dead
    gl_FragColor = vec4(0.f, 0.f, 0.f, age);
  }
}
