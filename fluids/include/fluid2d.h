#ifndef _FLUID2_D_H_
#define _FLUID2_D_H_

#define FLUID_DIM_DEFAULT   32
//#define N	32

//"Fluxel" = "fluid pixel"

//helper macro for indexing the fluxel array
#define IX(i ,j) 	((i)+(N+2)*(j))


typedef struct {
	float p;		//pressure
	float t;		//temperature
	
	float u;		//velocity (x)
	float v;		//velocity (y)
	
	float dp;		//inflow / outflow rates
	float dt;		//temperature change
} Fluxel;

class Fluid2D
{
public:
	Fluid2D();
	Fluid2D(const int w, const int h);
	~Fluid2D();

	//static inline int IX(const int i, const int j) const { return (((i)+(N+2)*(j))) }

	const Fluxel *get_fluid_array() { return f; }
	const float get_x_dim() { return dim[0]; }
	const float get_y_dim() { return dim[1]; }

	void simulate(const float t) {};
	
	void render_gl();
	void init_gl();			//init the vertex array (openGL)
	
protected:
	void init_helper();		//helper function for the constructor
	
	void advect(int b,
	            float *d,
	            float *d0,
	            float *u,
	            float *v,
	            float dt);			//advection step
	void diffuse(int b,
	             float *x,
	             float *x0,
	             float dif,
	             float dt);			//diffusion step

	//helper function access our 1-D fluid array with 2 coordinates
	//inline just as fast as a macro? *shrug*
	inline int idx(const int &i, const int &j) const {
		return i + (dim[0] + 2) * j;
	}

	void set_bnd(int b, float *x);

private:
	int N;					//TEMPORARY UNTIL I FIGURE OUT HOW TO HAVE DIFF DIMS
	int dim[2];				//dimensions of the grid (+2 for boundary conditions)
	Fluxel *f;				//fluid array
	Fluxel *f_swap;			//swap array

	float diff_rate;		//rate of diffusion

	//GLfloat *gl_verts;		//the vertices of the grid
	//GLuint gl_texture;
};

#endif // _FLUID2_D_H_
