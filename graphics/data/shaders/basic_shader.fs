varying vec3 color_varying;
varying vec2 uvs_varying;

uniform sampler2D tex;

void main()
{    
    vec4 new_col = vec4(color_varying.rgb, 1.0) * texture2D(tex, uvs_varying);
    gl_FragColor = new_col;
}