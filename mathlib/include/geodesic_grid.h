#ifndef __GEODESIC_GRID_H__
#define __GEODESIC_GRID_H__

#include <assert.h>
#include "math_utility.h"
#include "geodesic_cell.h"

//a subdividible geodesic grid on the surface of the sphere
//we can use this data structure to simulate cellular automata

enum CellIndex {
  CELL_ERROR = -1,
  CELL_N = 0, //N pole
  CELL_A = 0,
  CELL_B,
  CELL_C,
  CELL_D,
  CELL_E,
  CELL_F,
  CELL_G,
  CELL_H,
  CELL_I,
  CELL_J,
  CELL_K,
  CELL_L = 11,
  CELL_S = 11, //S pole
};

template <class T>
class GeodesicEdge
{
public:
  GeodesicEdge() { a = NULL; b = NULL; }
  ~GeodesicEdge() {}

  bool operator==(const GeodesicEdge<T> &r)
  {
    if(r.a == this->a && r.b == this->b)
    {
      return true;
    }
    if(r.a == this->b && r.b == this->a)
    {
      return true;
    }

    return false;
  }

  GeodesicCell<T> *a;
  GeodesicCell<T> *b;
};


template <class T>
class GeodesicFace
{
public:
  GeodesicFace() {a = b = c = NULL; }
  ~GeodesicFace() {}

  void set_faces(GeodesicCell<T> *_a, GeodesicCell<T> *_b, GeodesicCell<T> *_c)
  {
    a = _a; b = _b; c = _c;
  }

  GeodesicCell<T> *a, *b, *c;
};

template <class T>
class GeodesicGrid
{
  public:
    GeodesicGrid()
    {
      // start with an icosahedron
      // E = F + V - 2 (Euler)
      // E = 30
      // V = 12
      // F = 20

      // each vertex of the icosahedron is a cell. So we have 12 cells @ the
      // first subdivision level.
      selected_cell = 0;

      subdivision_levels = 0;
      num_cells = 12;
      num_edges = 30;
      num_faces = 20;

      cells = new GeodesicCell<T>[12];
      edges = new GeodesicEdge<T>[30];
      faces = new GeodesicFace<T>[20];

      // fill the 12 vertices with the cartesian coordinates of the icosahedron
      // (0, ±1, ± φ)
       // (±1, ± φ, 0)
      // (± φ, 0, ±1)

      /*
      cells[0].pos =  Float3(-1.0f,  M_PHI, 0.0f);
      cells[1].pos =  Float3( 1.0f,  M_PHI, 0.0f);
      cells[2].pos =  Float3(-1.0f, -M_PHI, 0.0f);
      cells[3].pos =  Float3( 1.0f, -M_PHI, 0.0f);

      cells[4].pos =  Float3( 0.0f, -1.0f,  M_PHI);
      cells[5].pos =  Float3( 0.0f,  1.0f,  M_PHI);
      cells[6].pos =  Float3( 0.0f, -1.0f, -M_PHI);
      cells[7].pos =  Float3( 0.0f,  1.0f, -M_PHI);

      cells[8].pos =  Float3( M_PHI, 0.0f, -1.0f);
      cells[9].pos =  Float3( M_PHI, 0.0f,  1.0f);
      cells[10].pos = Float3(-M_PHI, 0.0f, -1.0f);
      cells[11].pos = Float3(-M_PHI, 0.0f,  1.0f);
      */

      // But... actually, the above coordinates, while mathematically attractive,
      // give me an icosahedron that is rotated oddly for my use (I want vertical poles)
      // so, here are vertex positions from Blender:
      //
      // P.S. I could also fairly easily take the above pretty vertices and rotate them by some
      //      amount no doubt related to PHI, but it seems easier to do this the brute-force
      //      way right now. (also, I don't have a matrix class in my math library yet)
      //

      cells[CELL_N].pos = Float3( 0.00000f,  1.00000f,  0.00000f);  // A (N pole)

      cells[CELL_B].pos = Float3( 0.89442f,  0.44721f, 0.00000f);
      cells[CELL_C].pos = Float3( 0.27639f, 0.44721f, -0.85064f);
      cells[CELL_D].pos = Float3(-0.72360f, 0.44721f, -0.52572f);
      cells[CELL_E].pos = Float3(-0.72360f,  0.44721f, 0.52572f);
      cells[CELL_F].pos = Float3( 0.27639f,  0.44721f, 0.85064f);

      cells[CELL_G].pos = Float3( 0.72360f, -0.44721f, -0.52572f);
      cells[CELL_H].pos = Float3(-0.27639f, -0.44721f, -0.85064f);
      cells[CELL_I].pos = Float3(-0.89442f, -0.44721f, 0.00000f);
      cells[CELL_J].pos = Float3(-0.27639f, -0.44721f, 0.85064f);
      cells[CELL_K].pos = Float3( 0.72360f, -0.44721f, 0.52572f);

      cells[CELL_S].pos = Float3( 0.00000f, -1.00000f,  0.00000f);  // F (S pole)

      //add edges
      edges[0].a = &cells[CELL_A];    edges[0].b = &cells[CELL_B];
      edges[1].a = &cells[CELL_A];    edges[1].b = &cells[CELL_C];
      edges[2].a = &cells[CELL_A];    edges[2].b = &cells[CELL_D];
      edges[3].a = &cells[CELL_A];    edges[3].b = &cells[CELL_E];
      edges[4].a = &cells[CELL_A];    edges[4].b = &cells[CELL_F];

      edges[5].a = &cells[CELL_B];    edges[5].b = &cells[CELL_F];
      edges[6].a = &cells[CELL_B];    edges[6].b = &cells[CELL_K];
      edges[7].a = &cells[CELL_B];    edges[7].b = &cells[CELL_G];
      edges[8].a = &cells[CELL_B];    edges[8].b = &cells[CELL_C];

      edges[9].a = &cells[CELL_C];    edges[9].b = &cells[CELL_G];
      edges[10].a = &cells[CELL_C];    edges[10].b = &cells[CELL_H];
      edges[11].a = &cells[CELL_C];    edges[11].b = &cells[CELL_D];

      edges[12].a = &cells[CELL_D];    edges[12].b = &cells[CELL_H];
      edges[13].a = &cells[CELL_D];    edges[13].b = &cells[CELL_I];
      edges[14].a = &cells[CELL_D];    edges[14].b = &cells[CELL_E];

      edges[15].a = &cells[CELL_E];    edges[15].b = &cells[CELL_I];
      edges[16].a = &cells[CELL_E];    edges[16].b = &cells[CELL_J];
      edges[17].a = &cells[CELL_E];    edges[17].b = &cells[CELL_F];

      edges[18].a = &cells[CELL_F];    edges[18].b = &cells[CELL_J];
      edges[19].a = &cells[CELL_F];    edges[19].b = &cells[CELL_K];

      edges[20].a = &cells[CELL_G];    edges[20].b = &cells[CELL_K];
      edges[21].a = &cells[CELL_G];    edges[21].b = &cells[CELL_L];
      edges[22].a = &cells[CELL_G];    edges[22].b = &cells[CELL_H];

      edges[23].a = &cells[CELL_H];    edges[23].b = &cells[CELL_L];
      edges[24].a = &cells[CELL_H];    edges[24].b = &cells[CELL_I];

      edges[25].a = &cells[CELL_I];    edges[25].b = &cells[CELL_L];
      edges[26].a = &cells[CELL_I];    edges[26].b = &cells[CELL_J];

      edges[27].a = &cells[CELL_J];    edges[27].b = &cells[CELL_L];
      edges[28].a = &cells[CELL_J];    edges[28].b = &cells[CELL_K];

      edges[29].a = &cells[CELL_K];    edges[29].b = &cells[CELL_L];

      //set up faces
      faces[0].set_faces(&cells[CELL_A], &cells[CELL_B], &cells[CELL_C]);      //ABC
      faces[1].set_faces(&cells[CELL_A], &cells[CELL_C], &cells[CELL_D]);      //ACD
      faces[2].set_faces(&cells[CELL_A], &cells[CELL_D], &cells[CELL_E]);      //ADE
      faces[3].set_faces(&cells[CELL_A], &cells[CELL_E], &cells[CELL_F]);      //AEF
      faces[4].set_faces(&cells[CELL_A], &cells[CELL_F], &cells[CELL_B]);      //AFB

      faces[5].set_faces(&cells[CELL_B], &cells[CELL_G], &cells[CELL_C]);      //BGC
      faces[6].set_faces(&cells[CELL_C], &cells[CELL_G], &cells[CELL_H]);      //CGH
      faces[7].set_faces(&cells[CELL_C], &cells[CELL_H], &cells[CELL_D]);      //CHD
      faces[8].set_faces(&cells[CELL_D], &cells[CELL_H], &cells[CELL_I]);      //DHI
      faces[9].set_faces(&cells[CELL_D], &cells[CELL_I], &cells[CELL_E]);      //DIE
      faces[10].set_faces(&cells[CELL_E], &cells[CELL_I], &cells[CELL_J]);    //EIJ
      faces[11].set_faces(&cells[CELL_E], &cells[CELL_J], &cells[CELL_F]);    //EJF
      faces[12].set_faces(&cells[CELL_F], &cells[CELL_J], &cells[CELL_K]);    //FJK
      faces[13].set_faces(&cells[CELL_F], &cells[CELL_K], &cells[CELL_B]);    //FKB
      faces[14].set_faces(&cells[CELL_B], &cells[CELL_K], &cells[CELL_G]);    //BKG

      faces[15].set_faces(&cells[CELL_L], &cells[CELL_G], &cells[CELL_K]);    //LGK
      faces[16].set_faces(&cells[CELL_L], &cells[CELL_K], &cells[CELL_J]);    //LKJ
      faces[17].set_faces(&cells[CELL_L], &cells[CELL_J], &cells[CELL_I]);    //LJI
      faces[18].set_faces(&cells[CELL_L], &cells[CELL_I], &cells[CELL_H]);    //LIH
      faces[19].set_faces(&cells[CELL_L], &cells[CELL_H], &cells[CELL_G]);    //LHG


      // set up adjacency (graph version)
      // IMPORTANT: THESE MUST BE CLOCKWISE FOR RENDERING TO WORK PROPERLY!!!
      // N - BCDEF
      cells[CELL_N].add_neighbor(&cells[CELL_B]);
      cells[CELL_N].add_neighbor(&cells[CELL_C]);
      cells[CELL_N].add_neighbor(&cells[CELL_D]);
      cells[CELL_N].add_neighbor(&cells[CELL_E]);
      cells[CELL_N].add_neighbor(&cells[CELL_F]);

      // B - AFKGC
      cells[CELL_B].add_neighbor(&cells[CELL_N]);
      cells[CELL_B].add_neighbor(&cells[CELL_F]);
      cells[CELL_B].add_neighbor(&cells[CELL_K]);
      cells[CELL_B].add_neighbor(&cells[CELL_G]);
      cells[CELL_B].add_neighbor(&cells[CELL_C]);

      // C - ABGHD
      cells[CELL_C].add_neighbor(&cells[CELL_N]);
      cells[CELL_C].add_neighbor(&cells[CELL_B]);
      cells[CELL_C].add_neighbor(&cells[CELL_G]);
      cells[CELL_C].add_neighbor(&cells[CELL_H]);
      cells[CELL_C].add_neighbor(&cells[CELL_D]);

      // D - ACHIE
      cells[CELL_D].add_neighbor(&cells[CELL_N]);
      cells[CELL_D].add_neighbor(&cells[CELL_C]);
      cells[CELL_D].add_neighbor(&cells[CELL_H]);
      cells[CELL_D].add_neighbor(&cells[CELL_I]);
      cells[CELL_D].add_neighbor(&cells[CELL_E]);

      // E - ADIJF
      cells[CELL_E].add_neighbor(&cells[CELL_N]);
      cells[CELL_E].add_neighbor(&cells[CELL_D]);
      cells[CELL_E].add_neighbor(&cells[CELL_I]);
      cells[CELL_E].add_neighbor(&cells[CELL_J]);
      cells[CELL_E].add_neighbor(&cells[CELL_F]);

      // F - AEJKB
      cells[CELL_F].add_neighbor(&cells[CELL_N]);
      cells[CELL_F].add_neighbor(&cells[CELL_E]);
      cells[CELL_F].add_neighbor(&cells[CELL_J]);
      cells[CELL_F].add_neighbor(&cells[CELL_K]);
      cells[CELL_F].add_neighbor(&cells[CELL_B]);

      // G - BKLHC
      cells[CELL_G].add_neighbor(&cells[CELL_B]);
      cells[CELL_G].add_neighbor(&cells[CELL_K]);
      cells[CELL_G].add_neighbor(&cells[CELL_S]);
      cells[CELL_G].add_neighbor(&cells[CELL_H]);
      cells[CELL_G].add_neighbor(&cells[CELL_C]);

      // H - CGLID
      cells[CELL_H].add_neighbor(&cells[CELL_C]);
      cells[CELL_H].add_neighbor(&cells[CELL_G]);
      cells[CELL_H].add_neighbor(&cells[CELL_S]);
      cells[CELL_H].add_neighbor(&cells[CELL_I]);
      cells[CELL_H].add_neighbor(&cells[CELL_D]);

      // I - DHLJE
      cells[CELL_I].add_neighbor(&cells[CELL_D]);
      cells[CELL_I].add_neighbor(&cells[CELL_H]);
      cells[CELL_I].add_neighbor(&cells[CELL_S]);
      cells[CELL_I].add_neighbor(&cells[CELL_J]);
      cells[CELL_I].add_neighbor(&cells[CELL_E]);

      // J - EILKF
      cells[CELL_J].add_neighbor(&cells[CELL_E]);
      cells[CELL_J].add_neighbor(&cells[CELL_I]);
      cells[CELL_J].add_neighbor(&cells[CELL_S]);
      cells[CELL_J].add_neighbor(&cells[CELL_K]);
      cells[CELL_J].add_neighbor(&cells[CELL_F]);

      // K - BFJLG
      cells[CELL_K].add_neighbor(&cells[CELL_B]);
      cells[CELL_K].add_neighbor(&cells[CELL_F]);
      cells[CELL_K].add_neighbor(&cells[CELL_J]);
      cells[CELL_K].add_neighbor(&cells[CELL_S]);
      cells[CELL_K].add_neighbor(&cells[CELL_G]);

      // S - GKJIH
      cells[CELL_S].add_neighbor(&cells[CELL_G]);
      cells[CELL_S].add_neighbor(&cells[CELL_K]);
      cells[CELL_S].add_neighbor(&cells[CELL_J]);
      cells[CELL_S].add_neighbor(&cells[CELL_I]);
      cells[CELL_S].add_neighbor(&cells[CELL_H]);

      //assign random colors to each vertex for now
      for(int i = 0; i < 12; i++)
      {
        cells[i].color = Float3(random(0.0f, 1.0f), random(0.0f, 1.0f), random(0.0f, 1.0f));
        //cells[i].make_neighbors_clockwise();
      }

      //create 6 rows of adjacency data
      adjacency_grid = new GeodesicCell<T> **[7];
      for(int i = 0; i < 7; i++)
      {
        //longest rows have 4 entries
        adjacency_grid[i] = new GeodesicCell<T> *[4];
        for(int j = 0; j < 4; j++)
        {
          //start off pointing to nothing (we'll fix this later)
          adjacency_grid[i][j] = NULL;
        }
      }

      //                       dU
      //             A-B-C     4
      //             |/|/|
      //           A-K-L-F     3
      //           |/|/|
      //         A-I-J-F       2
      //         |/|/|                =>
      //       A-G-H-F         1
      //       |/|/|                       .-.-A-A-A-.-.
      //     A-D-E-F           0           .-A-G-I-K-B-.
      //     |/|/|                         A-D-E-H-J-L-C
      //     B-C-F             0           B-C-F-F-F-F-F
      // dV: 0-0-0-1-2-3-4                 0-0-0-1-2-3-4

      /*adjacency_grid[0][0] = &cells[CELL_B];    //B
      adjacency_grid[1][0] = &cells[CELL_C];    //C
      adjacency_grid[2][0] = &cells[CELL_F];    //F
      adjacency_grid[3][0] = &cells[CELL_F];    //F
      adjacency_grid[4][0] = &cells[CELL_F];    //F
      adjacency_grid[5][0] = &cells[CELL_F];    //F
      adjacency_grid[6][0] = &cells[CELL_F];    //F

      adjacency_grid[0][1] = &cells[CELL_A];    //A
      adjacency_grid[1][1] = &cells[CELL_D];    //D
      adjacency_grid[2][1] = &cells[CELL_E];    //E
      adjacency_grid[3][1] = &cells[CELL_H];    //H
      adjacency_grid[4][1] = &cells[CELL_J];    //J
      adjacency_grid[5][1] = &cells[CELL_L];    //L
      //adjacency_grid[6][1] = &cells[2];    //C

      adjacency_grid[1][2] = &cells[CELL_A];    //A
      adjacency_grid[2][2] = &cells[CELL_G];    //G
      adjacency_grid[3][2] = &cells[CELL_I];    //I
      adjacency_grid[4][2] = &cells[CELL_K];    //K
      //adjacency_grid[5][2] = &cells[1];    //B

      adjacency_grid[2][3] = &cells[CELL_A];    //A
      adjacency_grid[3][3] = &cells[CELL_A];    //A
      //adjacency_grid[4][3] = &cells[0];    //A

      adj_dv = new int[7];
      adj_dv[0] = 0;
      adj_dv[1] = 0;
      adj_dv[2] = 0;
      adj_dv[3] = 1;
      adj_dv[4] = 2;
      adj_dv[5] = 3;
      adj_dv[6] = 4;*/
    }

    ~GeodesicGrid()
    {
      delete[] cells;
      delete[] edges;
      delete[] faces;

      delete[] adjacency_grid;
    }

    void subdivide(int levels)
    {
      for(int i = 0; i < levels; i++)
      {
        sub();
      }
    }

    int get_subdivision_levels() const { return subdivision_levels; }

    void select_next_cell()
    {
      selected_cell++;
      if(selected_cell >= num_cells)
      {
        selected_cell = 0;
      }
      std::cout<<"selected_cell: "<<selected_cell<<std::endl;
    }

    void select_prev_cell()
    {
      selected_cell--;
      if(selected_cell < 0)
      {
        selected_cell = num_cells - 1;
      }
    }

    void make_selected_neighbors_clockwise()
    {
      GeodesicCell<T> *c = &cells[selected_cell];
      c->make_neighbors_clockwise();
    }

    GeodesicCell<T> *get_cell_array() { return cells; }
    int get_num_cells() const { return num_cells; }
    int get_num_edges() const { return num_edges; }
    int get_num_faces() const { return num_faces; }

    GeodesicCell<T> *get_cell_by_uv(const Float2 uv, GeodesicCell<T> *start)
    {
      //
      // greedy walk
      //
      GeodesicCell<T> *closest = NULL;//start;
      if(closest == NULL) { closest = &cells[num_cells / 2]; }
      float min_d2 = dist_squared(closest->uv, uv);

      while(true)
      {
        //examine each neighbor
        bool found_one_closer = false;
        GeodesicCell<T> *next_c = closest;
        for(int i = 0; i < closest->num_neighbors; i++)
        {
          GeodesicCell<T> *n = closest->neighbors[i];
          float n_d2 = dist_squared(uv, n->uv);
          if(n_d2 < min_d2 || min_d2 < 0.0f)
          {
            min_d2 = n_d2;
            next_c = n;
            found_one_closer = true;
          }
        }

        if(!found_one_closer)
        {
          return closest;
        }
        assert(next_c);
        closest = next_c;
      }
      assert(false);
      return NULL;
    }

  private:

    //helper functions
    GeodesicCell<T> *add_unique_cell_at_point(Float3 &p, GeodesicCell<T> *_cells, int &cell_count, GeodesicCell<T> *a, GeodesicCell<T> *b)
    {
      float weld_threshold = 0.0001f;
      float weld_threshold_2 = weld_threshold * weld_threshold;
      //maybe add ab to the new cell list (if there are no other verts *super* close)

      // thoughts for optimization:
      // 1. Use a KD tree (other space partition tree) to find the closest cell to a
      //    particular point
      // 2. compare x, y and z distances first before doing the dist_squared calculation
      //
      // 3. For our purposes, I'm pretty sure we just need to check the neighbors
      //    of our neighbors, and not the entire grid.
      //
      // 4. This all goes away if we get the adjacency grid working correctly
      //
      for(int i = 0; i < a->num_neighbors; i++)
      {
        GeodesicCell<T> *c = a->neighbors[i];
        float d2 = dist_squared(p, c->pos);
        if(d2 < weld_threshold_2)
        {
          return c;
        }
      }

      for(int i = 0; i < b->num_neighbors; i++)
      {
        GeodesicCell<T> *c = b->neighbors[i];
        float d2 = dist_squared(p, c->pos);
        if(d2 < weld_threshold_2)
        {
          return c;
        }
      }

      //didn't find any... "allocate" a new one off the end of the array
      _cells[cell_count].init();
      _cells[cell_count].pos = p;
      _cells[cell_count].color = Float3(1.0f, 0.0f, 0.0f);
      return &_cells[cell_count++];
    }
    GeodesicCell<T> *add_cell_at_point(Float3 &p, GeodesicCell<T> *_cells, int &cell_count)
    {
      _cells[cell_count].init();
      _cells[cell_count].pos = p;
      _cells[cell_count].color = Float3(1.0f, 0.0f, 0.0f);
      return &_cells[cell_count++];
    }

    void sub()
    {
      subdivision_levels++;

      // E = F + V - 2 (Euler)
      int old_num_edges = num_edges;
      int new_num_edges = num_edges * 2 * 2;
      GeodesicEdge<T> *new_edges = new GeodesicEdge<T>[new_num_edges];

      int old_num_verts = num_cells;
      int new_num_verts = old_num_verts + old_num_edges;
      GeodesicCell<T> *new_cells = new GeodesicCell<T>[new_num_verts];

      int old_num_faces = num_faces;
      int new_num_faces = new_num_edges + 2 - new_num_verts; //E + 2 - V (Euler)
      GeodesicFace<T> *new_faces = new GeodesicFace<T>[new_num_faces];

      //first, add all existing vertices to the new vertex list
      //push all the existing vertices into the new vertex list
      int i;
      //int new_vert_idx = 0;
      int new_vert_idx = 0;
      for(i = 0; i < old_num_verts; i++)
      {
        //copy the old vert to the temp list (operator=)
        cells[i].pos.normalize();
        new_cells[new_vert_idx] = cells[i];

        GeodesicCell<T> *c = &new_cells[new_vert_idx];
        c->num_neighbors = 0;

        //fix up edge list
        for(int j = 0; j < old_num_edges; j++)
        {
          if(edges[j].a == &cells[i]) { edges[j].a = c; }
          if(edges[j].b == &cells[i]) { edges[j].b = c; }
        }

        //fix up face list
        for(int j = 0; j < old_num_faces; j++)
        {
          if(faces[j].a == &cells[i]) { faces[j].a = c; }
          if(faces[j].b == &cells[i]) { faces[j].b = c; }
          if(faces[j].c == &cells[i]) { faces[j].c = c; }
        }
        new_vert_idx++;
      }

      int new_face_idx = 0;
      for(i = 0; i < old_num_faces; i++)
      {
        //cout<<"old face idx: "<<i<<endl;
        //find the midpoints of the three edges in each face
        GeodesicCell<T> *ab = NULL;
        GeodesicCell<T> *bc = NULL;
        GeodesicCell<T> *ca = NULL;

        GeodesicFace<T> *f = &faces[i];
        Float3 mid_ab = midpoint(f->a->pos, f->b->pos);
        Float3 mid_bc = midpoint(f->b->pos, f->c->pos);
        Float3 mid_ca = midpoint(f->c->pos, f->a->pos);

        ab = add_unique_cell_at_point(mid_ab, new_cells, new_vert_idx, f->a, f->b);
        //ab->color = 0.5f * (f->a->color + f->b->color);
        bc = add_unique_cell_at_point(mid_bc, new_cells, new_vert_idx, f->b, f->c);
        //bc->color = 0.5f * (f->b->color + f->c->color);
        ca = add_unique_cell_at_point(mid_ca, new_cells, new_vert_idx, f->c, f->a);
        //ca->color = 0.5f * (f->c->color + f->a->color);
        assert(new_vert_idx <= new_num_verts);

        ab->add_unique_neighbor(f->b);
        ab->add_unique_neighbor(bc);
        ab->add_unique_neighbor(ca);
        ab->add_unique_neighbor(f->a);

        bc->add_unique_neighbor(f->c);
        bc->add_unique_neighbor(ca);
        bc->add_unique_neighbor(ab);
        bc->add_unique_neighbor(f->b);

        ca->add_unique_neighbor(f->a);
        ca->add_unique_neighbor(ab);
        ca->add_unique_neighbor(bc);
        ca->add_unique_neighbor(f->c);

        //questionable
        f->a->add_unique_neighbor(ab);
        f->a->add_unique_neighbor(ca);

        f->b->add_unique_neighbor(bc);
        f->b->add_unique_neighbor(ab);

        f->c->add_unique_neighbor(ca);
        f->c->add_unique_neighbor(bc);

        f->a->make_neighbors_clockwise();
        f->b->make_neighbors_clockwise();
        f->c->make_neighbors_clockwise();

        //so now, we are going to ditch the original face and add 4 new ones
        assert(new_face_idx + 4 <= new_num_faces);
        GeodesicFace<T> *a_ab_ca = &new_faces[new_face_idx++];
        GeodesicFace<T> *ab_b_bc = &new_faces[new_face_idx++];
        GeodesicFace<T> *bc_c_ca = &new_faces[new_face_idx++];
        GeodesicFace<T> *ab_bc_ca = &new_faces[new_face_idx++];

        a_ab_ca->a = f->a;    a_ab_ca->b = ab;    a_ab_ca->c = ca;
        ab_b_bc->a = ab;      ab_b_bc->b = f->b;  ab_b_bc->c = bc;
        bc_c_ca->a = bc;      bc_c_ca->b = f->c;  bc_c_ca->c = ca;
        ab_bc_ca->a = ab;     ab_bc_ca->b = bc;   ab_bc_ca->c = ca;
      }

      for(int i = 0; i < new_num_verts; i++)
      {
        //push the vertex out to the unit vector
        GeodesicCell<T> *c = &new_cells[i];
        c->pos.normalize();
        c->generate_uvh();

        assert(c->num_neighbors > 4);
      }

      delete[] cells;
      cells = new_cells;
      num_cells = new_num_verts;

      delete[] faces;
      faces = new_faces;
      num_faces = new_num_faces;

      delete[] edges;
      edges = new_edges;
      num_edges = new_num_edges;
    }

    //TODO: migrate this out of here
    int selected_cell;                  //index of currently selected cell

    int subdivision_levels;             //current subdivision level
    int num_cells;                      //number of geodesic cells
    int num_edges;                      //number of edges
    int num_faces;                      //number of faces

    GeodesicCell<T>  *cells;              //the array of cells that store the actual data
    GeodesicEdge<T>    *edges;              //the array of cell pairs (edges)
    GeodesicFace<T>    *faces;              //the array of edge triangles (faces)

    //obsolete? (could be useful if we ever want to move this to the GPU)
    GeodesicCell<T> ***adjacency_grid;    //store adjacency information
    int *adj_dv;                        //vertical offset for adjacency grid
};

#endif //__GEODESIC_GRID_H__
