#version 120

uniform vec2 texel_size;
uniform sampler2D surface_tex;

void main()
{
  vec3 downsampled = vec3(0.0, 0.0, 0.0);


  //anamorphic flare kernel
  float horiz_streak_size = 10.0f;
  downsampled += texture2D(surface_tex, gl_TexCoord[0].st - horiz_streak_size * vec2(texel_size.x, 0.0)).rgb;
  downsampled += texture2D(surface_tex, gl_TexCoord[0].st + horiz_streak_size * vec2(texel_size.x, 0.0)).rgb;
  downsampled += texture2D(surface_tex, gl_TexCoord[0].st - vec2(0.0, texel_size.y)).rgb;
  downsampled += texture2D(surface_tex, gl_TexCoord[0].st + vec2(0.0, texel_size.y)).rgb;
  downsampled /= 4.0;


  /*
  float blur_size = 2.5f;
  downsampled += texture2D(surface_tex, gl_TexCoord[0].st + blur_size * vec2(-texel_size.x, -texel_size.y)).rgb;
  downsampled += texture2D(surface_tex, gl_TexCoord[0].st + blur_size * vec2(texel_size.x, -texel_size.y)).rgb;
  downsampled += texture2D(surface_tex, gl_TexCoord[0].st + blur_size * vec2(texel_size.x, texel_size.y)).rgb;
  downsampled += texture2D(surface_tex, gl_TexCoord[0].st + blur_size * vec2(-texel_size.x, texel_size.y)).rgb;
  downsampled /= 4.0;
  */


  gl_FragColor = vec4(downsampled.rgb, 1.0);
}
