#ifndef __STATIC_MESH_H__
#define __STATIC_MESH_H__

#include <vector>

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include "math_utility.h"

#include "material.h"

class World;

using namespace Math;

typedef struct
{
    int hash_id;
    char file_name[256];
    int dim[2];

    int img_size;
    unsigned char *image;
} TextureFileData;

typedef struct
{
    Float3 diffuse_rgb;

    int num_textures;
    int *tex_idx;
} MaterialFileData;

typedef struct
{
    Float3 position;
    Float3 normal;
} VertexFileData;

typedef struct
{
    int face_idx;
    int mat_idx;
    Float3 face_normal;

    Float2 uvs[3];
    Float3 color[3];
    int vert_idx[3];
} FaceFileData;

typedef struct
{
    unsigned long       hash_id;
    int                 num_materials;
    int                 num_vertices;
    int                 num_faces;

    MaterialFileData    *materials;
    VertexFileData      *vertices;
    FaceFileData        *faces;
} StaticMeshFileData;


typedef struct {
    Float3 pos;
    Float3 normal;
} MeshVertex;

typedef struct {
    int mat_id;
    std::vector<int> vertex_indices;
    Float3 normal;
} MeshFace;

typedef struct {
    Material *m;

    int num_indices;

    //OpenGL buffer object name
    GLuint ibo;

    //actual index data
    //GLushort *index_buffer;
} DrawCall;

class StaticMesh
{
    public:
        StaticMesh();
        ~StaticMesh();

        bool read_from_file(const char *filename);
        bool read_from_file_data(StaticMeshFileData *smfd);
        bool fixup_materials(World *w);
        bool init();
        void render_gl();

    protected:
        bool make_render_data();

        //actual render data
        std::vector<DrawCall> draw_calls;

        //TODO
        //these should only be temp - and eventually
        //deprecated - they should only be used to
        //build the render data
        std::vector<Material *>     materials;
        std::vector<MeshVertex>     vertices;
        std::vector<MeshFace>       faces;

        GLuint vbo;
        GLuint nbo;

        GLuint display_list;
};

#endif // __STATIC_MESH_H__
