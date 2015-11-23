#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include <math.h>
#include "glutility.h"

void gl_init()
{
    int argc;
    char **argv;
    //glutInit(&argc, argv);
}

void gl_sphere(const float *_pos, const float *_dim, const float _radius)
{
    int num_segments = 16;
    int num_rings = 8;

    glTranslatef(_pos[0], _pos[1], _pos[2]);
    glScalef(_dim[0], _dim[1], _dim[2]);

    //glutSolidSphere(_radius, num_segments, num_rings);
    /*glBegin(GL_POINTS);

      for(int i = 0; i < num_segments; i++)
      {
        float theta = (M_PI * 2.0f) / (float)i;
        for(int j = 0; j < num_rings; j++)
        {

        }
      }

    glEnd();*/
}

//draw a cube
void gl_cube(const float *_pos, const float *_dim)
{
	float pos[3] = { _pos[0], _pos[1], _pos[2] };
	float dim[3] = { _dim[0] / 2.0f, _dim[1] / 2.0f, _dim[2] / 2.0f };
	glTranslatef(pos[0], pos[1], pos[2]);
	glBegin(GL_QUADS);

		//FRONT FACE
		glNormal3f( 0.0f, 0.0f, 1.0f);
		glVertex3f(-dim[0], dim[1], dim[2]);              // Top Left
		glVertex3f( dim[0], dim[1], dim[2]);              // Top Right
		glVertex3f( dim[0],-dim[1], dim[2]);              // Bottom Right
		glVertex3f(-dim[0],-dim[1], dim[2]);              // Bottom Left

		//BACK FACE
		glNormal3f( 0.0f, 0.0f, -1.0f);
		glVertex3f(-dim[0], dim[1], -dim[2]);              // Top Left
		glVertex3f( dim[0], dim[1], -dim[2]);              // Top Right
		glVertex3f( dim[0],-dim[1], -dim[2]);              // Bottom Right
		glVertex3f(-dim[0],-dim[1], -dim[2]);              // Bottom Left

		//TOP FACE
		glNormal3f( 0.0f, 1.0f, 0.0f);
		glVertex3f(-dim[0], dim[1],  dim[2]);              // Top Left
		glVertex3f( dim[0], dim[1],  dim[2]);              // Top Right
		glVertex3f( dim[0], dim[1], -dim[2]);              // Top Right
		glVertex3f(-dim[0], dim[1], -dim[2]);              // Top Left

		//BOTTOM FACE
		glNormal3f( 0.0f, -1.0f, 0.0f);
		glVertex3f( dim[0],-dim[1],  dim[2]);              // Bottom Right
		glVertex3f(-dim[0],-dim[1],  dim[2]);              // Bottom Left
		glVertex3f(-dim[0],-dim[1], -dim[2]);              // Bottom Left
		glVertex3f( dim[0],-dim[1], -dim[2]);              // Bottom Right

		glNormal3f( 1.0f, 0.0f, 0.0f);
		glVertex3f( dim[0], dim[1],  dim[2]);              // Top Right
		glVertex3f( dim[0],-dim[1],  dim[2]);              // Bottom Right
		glVertex3f( dim[0],-dim[1], -dim[2]);              // Bottom Right
		glVertex3f( dim[0], dim[1], -dim[2]);              // Top Right

		glNormal3f( -1.0f, 0.0f, 0.0f);
		glVertex3f(-dim[0], dim[1],  dim[2]);              // Top Left
		glVertex3f(-dim[0],-dim[1],  dim[2]);              // Bottom Left
		glVertex3f(-dim[0],-dim[1], -dim[2]);              // Bottom Left
		glVertex3f(-dim[0], dim[1], -dim[2]);              // Top Left

	glEnd();
}

//draw a cube angled along the z-axis
void gl_cube(const float *_pos, const float *_dim, const float theta_z)
{
	float pos[3] = { _pos[0], _pos[1], _pos[2] };
	float dim[3] = { _dim[0] / 2.0f, _dim[1] / 2.0f, _dim[2] / 2.0f };
	glTranslatef(pos[0], pos[1], pos[2]);
	glRotatef(theta_z, 0.0f, 0.0f, 1.0f);
	glBegin(GL_QUADS);

		//FRONT FACE
		glNormal3f( 0.0f, 0.0f, 1.0f);
		glVertex3f(-dim[0], dim[1], dim[2]);              // Top Left
		glVertex3f( dim[0], dim[1], dim[2]);              // Top Right
		glVertex3f( dim[0],-dim[1], dim[2]);              // Bottom Right
		glVertex3f(-dim[0],-dim[1], dim[2]);              // Bottom Left

		//BACK FACE
		glNormal3f( 0.0f, 0.0f, -1.0f);
		glVertex3f(-dim[0], dim[1], -dim[2]);              // Top Left
		glVertex3f( dim[0], dim[1], -dim[2]);              // Top Right
		glVertex3f( dim[0],-dim[1], -dim[2]);              // Bottom Right
		glVertex3f(-dim[0],-dim[1], -dim[2]);              // Bottom Left

		//TOP FACE
		glNormal3f( 0.0f, 1.0f, 0.0f);
		glVertex3f(-dim[0], dim[1],  dim[2]);              // Top Left
		glVertex3f( dim[0], dim[1],  dim[2]);              // Top Right
		glVertex3f( dim[0], dim[1], -dim[2]);              // Top Right
		glVertex3f(-dim[0], dim[1], -dim[2]);              // Top Left

		//BOTTOM FACE
		glNormal3f( 0.0f, -1.0f, 0.0f);
		glVertex3f( dim[0],-dim[1],  dim[2]);              // Bottom Right
		glVertex3f(-dim[0],-dim[1],  dim[2]);              // Bottom Left
		glVertex3f(-dim[0],-dim[1], -dim[2]);              // Bottom Left
		glVertex3f( dim[0],-dim[1], -dim[2]);              // Bottom Right

		glNormal3f( 1.0f, 0.0f, 0.0f);
		glVertex3f( dim[0], dim[1],  dim[2]);              // Top Right
		glVertex3f( dim[0],-dim[1],  dim[2]);              // Bottom Right
		glVertex3f( dim[0],-dim[1], -dim[2]);              // Bottom Right
		glVertex3f( dim[0], dim[1], -dim[2]);              // Top Right

		glNormal3f( -1.0f, 0.0f, 0.0f);
		glVertex3f(-dim[0], dim[1],  dim[2]);              // Top Left
		glVertex3f(-dim[0],-dim[1],  dim[2]);              // Bottom Left
		glVertex3f(-dim[0],-dim[1], -dim[2]);              // Bottom Left
		glVertex3f(-dim[0], dim[1], -dim[2]);              // Top Left

	glEnd();
}
