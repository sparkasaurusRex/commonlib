#include <GL/glew.h>
#include <GL/gl.h>

#include <assert.h>
#include <iostream>
#include <string.h>

#include "render_shepherd.h"

using namespace std;

RenderShepherd::RenderShepherd()
{
    clear_color[0] = 0.05f;
    clear_color[1] = 0.35f;
    clear_color[2] = 0.5f;

    resolution[0] = 640;
    resolution[1] = 480;
}

RenderShepherd::~RenderShepherd()
{

}

void RenderShepherd::init()
{
    GLenum res = glewInit();
	assert(res == GLEW_OK);

	cout<<"OpenGL version: "<<glGetString(GL_VERSION)<<endl;

	char extensions[4092];
	strcpy(extensions, (char *)glGetString(GL_EXTENSIONS));
	int len = strlen(extensions);
	for(int i = 0; i < len; i++) {
		if(extensions[i] == ' ') {
			extensions[i] = '\n';
		}
	}
	cout<<extensions<<endl;

	glViewport(0, 0, resolution[0], resolution[1]);
    glMatrixMode(GL_PROJECTION);
    glEnable(GL_DEPTH_TEST);

	glEnable(GL_LIGHTING);

	glEnable(GL_FOG);
	glFogi(GL_FOG_MODE, GL_LINEAR);
	glHint(GL_FOG_HINT, GL_DONT_CARE);

	gluPerspective(32, (float)resolution[0] / resolution[1], 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
}

void RenderShepherd::render()
{
    glClearColor(clear_color[0], clear_color[1], clear_color[2], 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glLoadIdentity();

    //setup camera
    gluLookAt(eye[0], eye[1], eye[2],
              look_at_pos[0], look_at_pos[1], look_at_pos[2],
              0.0f, 1.0f, 0.0f);

    //HACKY camera hack
    glTranslatef(0.0f, 0.0f, -22.0f);


    //render all static meshes
    std::vector<StaticMesh *>::iterator smi;
    for(smi = static_meshes.begin(); smi != static_meshes.end(); smi++)
    {
        StaticMesh *m = *smi;
        assert(m);


    }
}

void RenderShepherd::register_mesh(StaticMesh *m)
{

}

void RenderShepherd::register_texture(Texture *t)
{

}
