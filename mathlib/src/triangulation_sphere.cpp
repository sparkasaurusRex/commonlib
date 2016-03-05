#include <assert.h>
#include <iostream>
#include "triangulation_sphere.h"

using namespace Math;
using namespace std;


TriangulationSphere::TriangulationSphere()
{
  vertices = NULL;
}

TriangulationSphere::~TriangulationSphere() {}

void TriangulationSphere::set_vertices(std::vector<Float3> *verts)
{
  vertices = verts;
}

//Delaunay triangulation via Fortune's algorithm adapted to the surface
//of a sphere
void TriangulationSphere::generate_delaunay_triangulation()
{
  cout<<"TriangulationSphere::generate_delaunay_triangulation()"<<endl;
  beach_line_height = 1.0f;

  //put all the points into a priority_queue sorted by y-coordinate
  assert(vertices);
  for(int i = 0; i < vertices->size(); i++)
  {
    Float3 p = (*vertices)[i];
    site_events.push(std::pair<Float3, int>(p, i));
  }

  while(!site_events.empty() || !circle_events.empty())
  {
    SiteEvent p = site_events.top();
    //cout<<p.second<<": "<<p.first<<endl;

    //if p.y > sites in circle_events
    if(!circle_events.empty())
    {
      SiteEvent ce = circle_events.top();
      if(ce.first._val[1] < p.first._val[1])
      {
        handle_site_event(&p);
        site_events.pop();
      }
    }

    //site_events.pop();
  }
}

void TriangulationSphere::handle_site_event(SiteEvent *p)
{
  //1. if skiplist is empty, insert p into it and set the previous
  //and next pointers to point to itself
  //if the skiplist only contains one node, then simply append the p
  //to it and reset pointer correspondingly, otherwise, continue with steps 2-4

  //2. Search the skiplist from the reference position for the arc alpha
  // which will intersect with the great circle through p and north pole n
  // Assume that the two end points of alpha are (theta1, phi1) and
  // (theta2, phi2) such that points on alpha with azimuthal angle which lies between
  // phi1 and phi2
  //
}

void TriangulationSphere::fortune_step()
{
  cout<<"beach-line height: "<<beach_line_height<<endl;
}

void TriangulationSphere::advance_beach_line(const float dh)
{
  beach_line_height -= dh;
  if(beach_line_height < -1.0f)
  {
    beach_line_height = 1.0f;
  }
}
