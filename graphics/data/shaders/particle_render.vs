#version 120

#define NUM_CONSTANTS 4

uniform sampler2D particle_tex;
uniform sampler2D data_tex;

varying vec4 vertex_color;

//constants
//{lifespan, color_curve_id, size_curve_id, data_tex_height}
uniform float constants[NUM_CONSTANTS];


void main() {

  float lifespan = constants[0];

  gl_TexCoord[1] = gl_MultiTexCoord1; //sprite texture

  //Grab the particle's location
  vec4 pos_center = texture2D(particle_tex, gl_MultiTexCoord0.st);

  float age = pos_center.w;

  if (age < 0 || age > lifespan) {
    //Do not render
    vertex_color = vec4(0.0, 0.0, 0.0, 0.0);

    //position it right behind the camera
    gl_Position = gl_ProjectionMatrix * vec4(0.0, 0.0, 1000.0, 1.0);
  }
  else {
    //Render

    int color_curve_id = int(constants[1]);
    int size_curve_id = int(constants[2]);
    int data_tex_height = int(constants[3]);

    float normalized_age = age / lifespan;

    vec2 color_curve_uv = vec2(normalized_age, (color_curve_id + 0.5f) / data_tex_height);
    vec2 size_curve_uv = vec2(normalized_age, (size_curve_id + 0.5f) / data_tex_height);

    //Set particle color
    vertex_color = vec4(texture2D(data_tex, color_curve_uv));


    float particle_size = texture2D(data_tex, size_curve_uv).x;

    //Translate it to world coordinates
    vec4 pos_center_worldcoords = gl_ModelViewMatrix * vec4(pos_center.xyz, 1.0);

    //vec3 camera_pos = gl_ModelViewMatrixInverse[3].xyz / gl_ModelViewMatrixInverse[3].w;

    //Grab the billboard vertex.
    //TODO: Scale the billboard with respect to its distance to the camera.
    vec4 billboard_corner = vec4(gl_Vertex.xy, 0.0, 0.0) * particle_size;// / distance(pos_center_worldcoords.xyz, camera_pos);

    //The final vertex is the particle position plus the billboard corner.
    //Then translate it with the projection matrix.
    gl_Position = gl_ProjectionMatrix * (pos_center_worldcoords + billboard_corner);
  }
}
