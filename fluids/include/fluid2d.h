#ifndef _FLUID2_D_H_
#define _FLUID2_D_H_

#define FLUID_DIM_DEFAULT   64
//#define N	32

//"Fluxel" = "fluid pixel"

//helper macro for indexing the fluxel array
//#define IX(i ,j) 	((i)+(N+2)*(j))
#define SWAP(x0,x) { float *tmp = x0; x0 = x; x = tmp;}

#include "math_utility.h"

using namespace Math;


/*
typedef struct {
	float p;		//pressure
	float t;		//temperature

	float u;		//velocity (x)
	float v;		//velocity (y)

	float dp;		//inflow / outflow rates
	float dt;		//temperature change
} Fluxel;*/

class Fluid2D
{
public:
	Fluid2D();
	Fluid2D(const int w, const int h);
	~Fluid2D();

	//static inline int IX(const int i, const int j) const { return (((i)+(N+2)*(j))) }

	const float *get_density_array() { return dens; }
	const float get_x_dim() { return dim[0]; }
	const float get_y_dim() { return dim[1]; }

	void simulate(const float t);

	//void render_gl();
	//void init_gl();			//init the vertex array (openGL)

	void add_density_at_point(const Float2 pt, const float density, const float radius);
	void add_velocity_at_point(const Float2 pt, const Float2 vel, const float radius);

protected:
	void init_helper();		//helper function for the constructor

	void add_source(float *x, float *s, float dt);

	void density_step(float *x, float *x0, float *u, float *v, float diff, float dt);
	void velocity_step(float *u, float *v, float *u0, float *v0, float visc, float dt);
	void project(float *u, float *v, float *p, float *div);

	void advect(int b, float *d, float *d0, float *u, float *v, float dt);			//advection step
	void diffuse(int b, float *x, float *x0, float dif, float dt);			//diffusion step

	//helper function access our 1-D fluid array with 2 coordinates
	//inline just as fast as a macro? *shrug*
	inline int idx(const int &i, const int &j) const
	{
		return i + (dim[0] + 2) * j;
	}

	void set_bnd(int b, float *x);

private:
	int N;								//TEMPORARY UNTIL I FIGURE OUT HOW TO HAVE DIFF DIMS
	int dim[2];						//dimensions of the grid (+2 for boundary conditions)
	//Fluxel *f;						//fluid array
	//Fluxel *f_swap;				//swap array

	float *u, *v, *u_prev, *v_prev;
	float *dens, *dens_prev;

	float diff_rate;			//rate of diffusion

	//list of objects that can interact w/ the fluid sim

	//int num_interactors;
	//Fluid2d_Interactor *interactors;
};

#endif // _FLUID2_D_H_
