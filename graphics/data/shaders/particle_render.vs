#version 120

uniform sampler2D particle_tex;
uniform sampler2D data_tex;

uniform float lifespan;

uniform int color_curve_id;
uniform int data_tex_height;

varying vec4 vertex_color;


void main() {

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

    //Translate it to world coordinates
    vec4 pos_center_worldcoords = gl_ModelViewMatrix * vec4(pos_center.xyz, 1.0);

    //vec3 camera_pos = gl_ModelViewMatrixInverse[3].xyz / gl_ModelViewMatrixInverse[3].w;

    //Grab the billboard vertex.
    //TODO: Scale the billboard with respect to its distance to the camera.
    vec4 billboard_corner = vec4(gl_Vertex.xy, 0.0, 0.0);// / distance(pos_center_worldcoords.xyz, camera_pos);

    //The final vertex is the particle position plus the billboard corner.
    //Then translate it with the projection matrix.
    gl_Position = gl_ProjectionMatrix * (pos_center_worldcoords + billboard_corner);

    //vertex_color = gl_Color;

    //Animate particle age: Green => young, Red => old
    //vertex_color = vec4(pos_center.w / lifespan, 1.0 - (pos_center.w / lifespan), 0.f, 1.f);

    float normalized_age = age / lifespan;

    //CHECK FOR ACCURACY
    vec2 color_curve_uv = vec2(age / lifespan, color_curve_id / (data_tex_height - 1));
    vertex_color = vec4(texture2D(data_tex, color_curve_uv));

  }
}
