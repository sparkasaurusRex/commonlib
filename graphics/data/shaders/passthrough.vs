/*#version 330

layout(location = 0) in vec4 in_position;

void main()
{
  gl_Position = in_position;
}
*/

#version 120
void main(void) {
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = ftransform();
}
