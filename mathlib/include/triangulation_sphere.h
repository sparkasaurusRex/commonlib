#ifndef __TRIANGULATION_SPHERE_H__
#define __TRIANGULATION_SPHERE_H__

//
// Methods for triangulating a point cloud constrained to the surface of
// the unit sphere
//

#include <vector>
#include <queue>
#include "math_utility.h"
#include "shapes.h"

namespace Math
{
  class TriangulationSphere
  {
  public:
    TriangulationSphere();
    ~TriangulationSphere();

    void reset() {}
    void set_vertices(std::vector<Float3> *verts);

    void generate_delaunay_triangulation();

    void fortune_step();  //TEMP / visualization only

    float get_beach_line_height() { return beach_line_height; }
    void advance_beach_line(const float dh);

    std::vector<Edge3D> *get_edges() { return &edges; }
  private:
    std::vector<Float3>     *vertices;    //pointer to the list of vertices we want to triangulate
    std::vector<Edge2D>     edges;        //the edge list we will return once triangulated

    //fortune algorithm variables
    float                   beach_line_height;  //temp / for visualization

    class SiteCompare
    {
    public:
      bool operator()(std::pair<Float3, int> a, std::pair<Float3, int> b)
      {
        if(a.first._val[1] < b.first._val[1]) { return true; }
        if(a.first._val[1] > b.first._val[1]) { return false; }

        //y coords are equal, so sort based on longitude
        //this will almost certainly never happen, so it's ok it's a little slow
        //if this proves too slow, we could also sort on x-coord in
        //cartesian space
        float long_a = 0.5f + (atan2(a.first._val[2], a.first._val[0]) / M_PI) * 0.5f;
        float long_b = 0.5f + (atan2(b.first._val[2], b.first._val[0]) / M_PI) * 0.5f;
        return (long_a < long_b);
      }
    };

    typedef std::pair<Float3, int> SiteEvent;
    typedef std::priority_queue<SiteEvent, std::vector<SiteEvent >, SiteCompare> SiteEventQueue;

    void handle_site_event(SiteEvent *e);

    SiteEventQueue circle_events;
    SiteEventQueue site_events;

    class BeachNode
    {
    public:
      BeachNode();

      SiteEvent *site;

      BeachNode *parent;
      BeachNode *left;
      BeachNode *right;
    };

    //use skip list / binary tree to store beach line
    //each node represents an arc (beta) on the beach line
    BeachNode *beach_line;
  };
};

#endif //__TRIANGULATION_SPHERE_H__
