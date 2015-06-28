#ifndef __RENDER_SHEPHERD_H__
#define __RENDER_SHEPHERD_H__

#include "static_mesh.h"
#include "material.h"
#include "texture.h"

//the render shepherd is for all intents and purposes
//the game's render context - it keeps all the renderables
//herded, and is in charge of the render loop

class RenderShepherd
{
    public:
        RenderShepherd();
        ~RenderShepherd();

        void set_render_resolution(Float2 &r);

        void init();

        void register_mesh(StaticMesh *m);
        void register_material(Material *m);
        void register_texture(Texture *t);

        void render();

        //temp
        Float3 eye;
        Float3 look_at_pos;

    protected:
        std::vector<StaticMesh *>   static_meshes;
        std::vector<Texture *>      textures;
        std::vector<Shader *>       shaders;

        Float3 clear_color;

        Float2 resolution;
};

#endif // __RENDER_SHEPHERD_H__
