#ifndef _FLUID2_D_H_
#define _FLUID2_D_H_

#define FLUID_DIM_DEFAULT   64
//#define N	32

//"Fluxel" = "fluid pixel"

//helper macro for indexing the fluxel array
//#define IX(i ,j) 	((i)+(N+2)*(j))
#define SWAP(x0,x) { float *tmp = x0; x0 = x; x = tmp;}

#include <vector>
#include "fluid2d_interactor.h"
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
	float get_x_dim() const { return dim[0]; }
	float get_y_dim() const { return dim[1]; }

	float get_viscosity() const { return viscosity; }
	float get_diffusion_rate() const { return diffusion_rate; }
	int   get_project_steps() const { return project_steps; }

	void set_viscosity(const float visc) { viscosity = visc; }
	void set_diffusion_rate(const float diff) { diffusion_rate = diff; }
	void set_project_steps(const int p) { project_steps = p; }

	void simulate(const float t);

	void add_density_at_point(const Float2 pt, const float density, const float radius);
	void add_velocity_at_point(const Float2 pt, const Float2 vel, const float radius);
	void add_interactor(Fluid2DInteractor *fi);

protected:
	void init_helper();		//helper function for the constructor

	void add_source(float *x, float *s, float dt);

	void density_step(float *x, float *x0, float *u, float *v, float diff, float dt);
	void velocity_step(float *u, float *v, float *u0, float *v0, float visc, float dt);
	void project(float *u, float *v, float *p, float *div);

	void advect(int b, float *d, float *d0, float *u, float *v, float dt);			//advection step
	void diffuse(int b, float *x, float *x0, float dif, float dt);							//diffusion step

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

	int project_steps;							//number of steps to use in the project (and diffuse) functions
	Float2 density_allowable_range;	//min/max density allowed in the simulation
	float diffusion_rate;						//rate of diffusion
	float viscosity;								//viscosity of the fluid

	//list of objects that can interact w/ the fluid sim

	//int num_interactors;
	//Fluid2DInteractor *interactors;
	std::vector<Fluid2DInteractor *> interactors;
};

#endif // _FLUID2_D_H_
