//#include <GL/gl.h>
#include <assert.h>
#include <iostream>
#include <cstring>

//#include "graphics/glutility.h"
#include "fluid2d.h"
#include "perlin.h"

using namespace std;

Fluid2D::Fluid2D()
{
	N = FLUID_DIM_DEFAULT;
	dim[0] = dim[1] = FLUID_DIM_DEFAULT;

	init_helper();
}

Fluid2D::Fluid2D(const int w, const int h)
{
	N = w;
	assert(w == h);
	dim[0] = w;
	dim[1] = h;

	init_helper();
}

//helper function for the constructor
void Fluid2D::init_helper()
{
	density_allowable_range = Float2(-1.0f, 1.0f);
	viscosity = 0.0075f;
	diffusion_rate = 0.001f;
	project_steps = 20;

	//Ellis' values:
	//viscosity = 0.0001f;
	//diffusion_rate = 0.001f;

	int size = (N + 2) * (N + 2);
	u = new float[size];
	v = new float[size];
	u_prev = new float[size];
	v_prev = new float[size];
	dens = new float[size];
	dens_prev = new float[size];

	float perlin_scale = 0.1f;

	cout<<"initializing fluid of size "<<N<<endl;
	for(int i = 0; i < dim[0]; i++)
	{
		for(int j = 0; j < dim[1]; j++)
		{
			u[idx(i, j)] = 0.0f;
			u_prev[idx(i, j)] = 0.0f;

			v[idx(i, j)] = 0.0f;
			v_prev[idx(i, j)] = 0.0f;

			dens[idx(i, j)] = 0.0f;
			dens_prev[idx(i, j)] = 0.0f;
		}
	}
}

Fluid2D::~Fluid2D()
{
	delete u;
	delete u_prev;
	delete v;
	delete v_prev;
	delete dens;
	delete dens_prev;
}

void Fluid2D::add_density_at_point(const Float2 pt, const float density, const float radius)
{
	float *src = new float[(N + 2) * (N + 2)];
	memset(src, 0, (N + 2) * (N + 2) * sizeof(float));
	for(int i = 0; i < dim[0]; i++)
	{
		for(int j = 0; j < dim[1]; j++)
		{
				Float2 p0((float)i / (float)dim[0], (float)j / (float)dim[1]);
				Float2 dir(pt[0] - p0[0], pt[1] - p0[1]);
				float d2 = dir.mag_squared();

				if(d2 < radius * radius)
				{
						src[idx(i, j)] = density * (1.0f - d2 / (radius * radius));
				}
		}
	}

	add_source(dens_prev, src, 1.0f);
	delete src;
}

void Fluid2D::add_velocity_at_point(const Float2 pt, const Float2 vel, const float radius)
{
	int array_size = (N + 2) * (N + 2);

	float *src_u = new float[array_size];
	float *src_v = new float[array_size];

	memset(src_u, 0, array_size * sizeof(float));
	memset(src_v, 0, array_size * sizeof(float));

	for(int i = 0; i < dim[0]; i++)
	{
		for(int j = 0; j < dim[1]; j++)
		{
			Float2 p0((float)i / (float)dim[0], (float)j / (float)dim[1]);
			Float2 v(pt[0] - p0[0], pt[1] - p0[1]);
			float dist = v.magnitude();

			if(dist < radius)
			{
				src_u[idx(i, j)] = vel[0] * (1.0f - dist / radius);
				src_v[idx(i, j)] = vel[1] * (1.0f - dist / radius);
			}

			//HACK - add artificial turbulence
			src_u[idx(i, j)] += 0.5f * PerlinNoise::scaled_octave_noise_2d(1, 1.0f, 5.0f, -1.0f, 1.0f, (float)i / (float)dim[0], (float)j / (float)dim[1]);
			src_v[idx(i, j)] += 0.5f * PerlinNoise::scaled_octave_noise_2d(1, 1.0f, 5.0f, -1.0f, 1.0f, (float)i / (float)dim[0] + 1.432f, (float)j / (float)dim[1] + 6.1235f);

			//src_v[idx(i, j)] += 0.5f * sin((float)i / (float)dim[0] * M_PI * 5.0f);

			//crazy vortex shit
			/*float pull_strength = 1.0f;
			float rot_strength = 3.0f;
			src_u[idx(i, j)] += rot_strength * ((float)j / (float)dim[1] - 0.5f);
			src_v[idx(i, j)] += -rot_strength * ((float)i / (float)dim[0] - 0.5f);
			src_u[idx(i, j)] += pull_strength * ((float)i / (float)dim[0] - 0.5f);
			src_u[idx(i, j)] += pull_strength * ((float)j / (float)dim[0] - 0.5f);
			*/

			src_v[idx(i, j)] += -5.0f * dens[idx(i, j)]; //lame temp differential code

		}
	}

	add_source(u_prev, src_u, 1.0f);
	add_source(v_prev, src_v, 1.0f);

	delete src_u;
	delete src_v;
}

void Fluid2D::add_interactor(Fluid2DInteractor *fi)
{
	assert(fi);
	assert(dens);
	assert(u);
	assert(v);

	fi->set_density_grid(dens);
	fi->set_velocity_grid(u, v);
	fi->set_grid_dimensions(Float2(dim[0], dim[1]));

	interactors.push_back(fi);
}

void Fluid2D::simulate(const float dt)
{
	for(int i = 0; i < interactors.size(); i++)
	{
		Fluid2DInteractor *fi = interactors[i];
		fi->simulate(dt);
	}
	velocity_step(u, v, u_prev, v_prev, viscosity, dt);
	density_step(dens, dens_prev, u, v, diffusion_rate, dt);
}

void Fluid2D::add_source(float *_x, float *_s, float dt)
{
	int i, size = (N + 2) * (N + 2);
	for(i = 0; i < size; i++)
	{
		_x[i] += dt * _s[i];

		//cap density
		if(_x == dens)
		{
				if(_x[i] < density_allowable_range[0])
				{
					_x[i] = density_allowable_range[0];
				}
				if(_x[i] > density_allowable_range[1])
				{
					_x[i] = density_allowable_range[1];
				}
		}
	}
}

void Fluid2D::density_step(float *_x, float *_x0, float *_u, float *_v, float diff, float dt)
{
	add_source(_x, _x0, dt);
	SWAP(_x0, _x);
	diffuse(0, _x, _x0, diff, dt);
	SWAP(_x0, _x);
	advect(0, _x, _x0, _u, _v, dt);
}

void Fluid2D::velocity_step(float *_u, float *_v, float *_u0, float *_v0, float visc, float dt)
{
	add_source(_u, _u0, dt);
	add_source(_v, _v0, dt);

	SWAP(_u0, _u);
	diffuse(1, _u, _u0, visc, dt);

	SWAP(_v0, _v);
	diffuse(2, _v, _v0, visc, dt);

	project(_u, _v, _u0, _v0);

	SWAP(_u0, _u);
	SWAP(_v0, _v);

	advect(1, _u, _u0, _u0, _v0, dt);
	advect(2, _v, _v0, _u0, _v0, dt);

	project(_u, _v, _u0, _v0);
}

void Fluid2D::project(float *_u, float *_v, float *_p, float *_div)
{
	int i, j, k;
	float h;
	h = 1.0 / N;
	for(i = 1; i <= N; i++) {
		for(j = 1; j <= N; j++) {
			_div[idx(i,j)] = -0.5 * h * (_u[idx(i + 1, j)] - _u[idx(i - 1, j)] +
																	 _v[idx(i, j + 1)] - _v[idx(i, j - 1)]);
			_p[idx(i,j)] = 0;
		}
	}
	set_bnd(0, _div);
	set_bnd(0, _p);

	for(k = 0; k < project_steps; k++)
	{
		for (i = 1; i <= N ; i++ )
		{
			for (j = 1; j <= N ; j++ )
			{
				_p[idx(i,j)] = (_div[idx(i, j)] + _p[idx(i - 1, j)] + _p[idx(i + 1, j)] + _p[idx(i, j - 1)] + _p[idx(i, j + 1)]) / 4;
			}
		}
		set_bnd(0, _p);
	}
	for(i = 1; i <= N; i++)
	{
		for(j = 1; j <= N; j++)
		{
			_u[idx(i,j)] -= 0.5 * (_p[idx(i + 1, j)] - _p[idx(i - 1, j)]) / h;
			_v[idx(i,j)] -= 0.5 * (_p[idx(i, j + 1)] - _p[idx(i, j - 1)]) / h;
		}
	}
	set_bnd(1, _u);
	set_bnd(2, _v);
}

//Gauss-Seidel relaxation
void Fluid2D::diffuse(int b, float *_x, float *_x0, float diff, float dt)
{
	int i, j, k;
	float a = dt * diff * N * N;

	for(k = 0; k < project_steps; k++) {
		for(i = 1 ; i <= N; i++) {
			for(j = 1; j <= N; j++) {
				_x[idx(i,j)] = (_x0[idx(i,j)] +
												a * (_x[idx(i - 1, j)] +
															_x[idx(i + 1, j)] +
															_x[idx(i, j - 1)] +
															_x[idx(i, j + 1)])) / (1 + 4 * a);
			}
		}
		set_bnd(b, _x);
	}
}

//advection step
void Fluid2D::advect(int b, float *_d, float *_d0, float *_u, float *_v, float dt)
{
	int i, j, i0, j0, i1, j1;
	float x, y, s0, t0, s1, t1, dt0;

	dt0 = dt * N;
	//dt1 = dt * dim[1];
	for(i = 1; i <= N; i++) {
		for(j = 1; j <= N; j++) {

			x = i - dt0 * _u[idx(i,j)];
			y = j - dt0 * _v[idx(i,j)];

			if(x < 0.5)
				x = 0.5;
			if(x > N + 0.5)
				x = N + 0.5;

			i0 = (int)x;
			i1 = i0 + 1;

			if(y < 0.5)
				y = 0.5;
			if(y > N + 0.5)
				y = N + 0.5;

			j0 = (int)y;
			j1 = j0 + 1;
			s1 = x - i0;
			s0 = 1 - s1;
			t1 = y - j0;
			t0 = 1 - t1;
			_d[idx(i, j)] = s0 * (t0 * _d0[idx(i0, j0)] +
			                      t1 * _d0[idx(i0, j1)]) +
							        s1 * (t0 * _d0[idx(i1, j0)] +
							              t1 * _d0[idx(i1, j1)]);
		}
	}
	set_bnd(b, _d);
}


void Fluid2D::set_bnd(int b, float *x)
{
	int i;
	for (i = 1; i <= N; i++)
	{
		x[idx(0,     i)] =	b == 1 ? -x[idx(1, i)] : x[idx(1, i)];
		x[idx(N + 1, i)] =	b == 1 ? -x[idx(N, i)] : x[idx(N, i)];
		x[idx(i,     0)] =	b == 2 ? -x[idx(i, 1)] : x[idx(i, 1)];
		x[idx(i, N + 1)] =	b == 2 ? -x[idx(i, N)] : x[idx(i, N)];
	}
	x[idx(0, 0)] =					0.5 * (x[idx(1, 0)] +     x[idx(0, 1)]);
	x[idx(0, N + 1)] =			0.5 * (x[idx(1, N + 1)] + x[idx(0, N)]);
	x[idx(N + 1, 0)] =			0.5 * (x[idx(N, 0)] +     x[idx(N + 1, 1)]);
	x[idx(N + 1, N + 1)] =	0.5*(x[idx(N, N + 1)] +   x[idx(N + 1, N)]);
}
