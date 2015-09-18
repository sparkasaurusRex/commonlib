
//stuff passed in by the CPU
attribute vec4 position;    //vertex position
attribute vec3 color;       //vertex color
attribute vec2 uvs;          //vertex texture coordinates

//attribute vec4 gl_MultiTexCoord0;

//stuff to be passed onto the fragment shader to be interpolated
varying vec3 color_varying; //vertex color

varying vec2 uvs0_varying;   //texture coordinates
varying vec2 uvs1_varying;      


//the shader entry point is the main method
void main()
{    
    //pass onto the fragment shader (interpolated between verts)
    color_varying = color;
    uvs_varying = uvs;
    
    //transform the vertex position
    gl_TexCoord[0] = gl_MultiTexCoord0;
    gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * position;
}