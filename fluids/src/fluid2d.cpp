//#include <GL/gl.h>
#include <assert.h>

//#include "graphics/glutility.h"
#include "fluid2d.h"
#include "perlin.h"

Fluid2D::Fluid2D()
{
	N = FLUID_DIM_DEFAULT;
	dim[0] = dim[1] = FLUID_DIM_DEFAULT;
	init_helper();
}

Fluid2D::Fluid2D(const int w, const int h)
{
	N = w;
	dim[0] = w;
	dim[1] = h;
	init_helper();
}

//helper function for the constructor
void Fluid2D::init_helper()
{
	f = new Fluxel[(dim[0] + 2) * (dim[1] + 2)];
	f_swap = new Fluxel[(dim[0] + 2) * (dim[1] + 2)];
	
	for(int i = 0; i < dim[0] + 1; i++) {
		for(int j = 0; j < dim[1] + 2; j++) {
			//f[j * (dim[0] + 2) + i].p
			f[idx(i, j)].p = PerlinNoise::octave_noise_2d(1, 1.0f, 1.0f, (float)i * 0.1f, (float)j * 0.1f);
			f[idx(i, j)].t = PerlinNoise::octave_noise_2d(1, 1.0f, 1.0f, (float)i * 0.12f + 0.715f, (float)j * 0.12f + 0.9142f);

			//initial velocities
			f[idx(i, j)].u = PerlinNoise::octave_noise_2d(1, 1.0f, 1.0f, (float)i * 0.417f + 0.832f, (float)j * 0.417f + 1.424f);
			f[idx(i, j)].v = PerlinNoise::octave_noise_2d(1, 1.0f, 1.0f, (float)i * 0.185f + 0.329f, (float)j * 0.185f + 0.192f);

			f[idx(i, j)].dp = PerlinNoise::octave_noise_2d(1, 1.0f, 1.0f, (float)i * 0.075f + 0.025f, (float)j * 0.075f + 2.723f);
			f[idx(i, j)].dt = PerlinNoise::octave_noise_2d(1, 1.0f, 1.0f, (float)i * 0.13f + 0.1653f, (float)j * 0.13f + 7.127f);
		}
	}
}

Fluid2D::~Fluid2D()
{
	delete f;
}

void Fluid2D::diffuse(int b, float *x, float *x0, float diff, float dt)
{
	int i, j, k;
	float a = dt * diff * N * N;

	for(k=0; k < 20; k++) {
		for (i = 1 ; i <= N; i++) {
			for(j = 1; j <= N; j++) {
				x[IX(i,j)] = (x0[IX(i,j)] +
					          a * (x[IX(i - 1, j)] +
					               x[IX(i + 1, j)] +
					               x[IX(i, j - 1)] +
					               x[IX(i, j+1)])) / (1 + 4 * a);
			}
		}
		set_bnd(b, x);
	}
}

//advection step
void Fluid2D::advect(int b, float *d, float *d0, float *u, float *v, float dt)
{
	int i, j, i0, j0, i1, j1;
	float x, y, s0, t0,s1, t1, dt0, dt1;

	dt0 = dt*N;
	//dt1 = dt * dim[1];
	for(i = 1; i <= N; i++) {
		for(j = 1; j <= N; j++) {
			
			x = i - dt0 * u[IX(i,j)];
			y = j - dt0 * v[IX(i,j)];

			if(x < 0.5)
				x = 0.5;
			if(x > N + 0.5)
				x = N + 0.5;

			i0 = (int)x;
			i1 = i0+ 1;

			if(y < 0.5)
				y = 0.5;
			if(y > N + 0.5)
				y = N + 0.5;

			j0 = (int)y;
			j1 = j0+1;
			s1 = x - i0;
			s0 = 1 - s1;
			t1 = y - j0;
			t0 = 1 - t1;
			d[IX(i,j)] = s0 * (t0 * d0[IX(i0, j0)] +
			                   t1 * d0[IX(i0, j1)]) +
							   s1 * (t0 * d0[IX(i1,j0)] + 
							         t1 * d0[IX(i1,j1)]);
		}
	}
	set_bnd(b, d);
}


void Fluid2D::set_bnd(int b, float *x)
{
	/*for(int i = 0; i <= N; i++) {
		x[IX(0 ,i)] = 		b == 1 ? –x[IX(1,i)] : x[IX(1,i)];
		x[IX(N + 1, i)] = 	b == 1 ? –x[IX(N,i)] : x[IX(N,i)];

		//...
	}*/
	assert(false);
}

void Fluid2D::init_gl()
{
	//glGenTextures(1, &gl_texture);
}

void Fluid2D::render_gl()
{
	//update the texture
	/*glBindTexture(GL_TEXTURE_2D, gl_texture);

	float pos[3] = { 0.0f, 0.0f, 0.0f };
	float dim[3] = { 2.0, 2.0f, 2.0f };
	glColor3f(1.0f, 0.0f, 0.0f);
	gl_cube(pos, dim);
	*/
	
	//render a textured quad
	//glBegin(GL_QUADS);
	//	glTexCoord2d(0.0, 0.0); glVertex2d(0.0, 0.0);
	//	glTexCoord2d(1.0, 0.0); glVertex2d(1.0, 0.0);
	//	glTexCoord2d(1.0, 1.0); glVertex2d(1.0, 1.0);
	//	glTexCoord2d(0.0, 1.0); glVertex2d(0.0, 1.0);
	//glEnd();
}