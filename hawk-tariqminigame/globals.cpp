
#include "globals.h"

CGlobals::CGlobals()
{
    root = new Root("", "config/screen.cfg", "logs/log.txt");

    // Initialise Ogre
    #ifdef _DEBUG
        root->loadPlugin("plugins/RenderSystem_GL_d.dll");
        root->loadPlugin("plugins/Plugin_CgProgramManager_d.dll");
        root->loadPlugin("plugins/Plugin_ParticleFX_d.dll");
    #else
        root->loadPlugin("plugins/RenderSystem_GL.dll");
        root->loadPlugin("plugins/Plugin_CgProgramManager.dll");
        root->loadPlugin("plugins/Plugin_ParticleFX.dll");
    #endif
}

CGlobals::~CGlobals()
{
    delete root;
}


CGlobals* CGlobals::Instance()
{
    static CGlobals instance;
    return &instance;
}

Root* CGlobals::getRoot()
{
    return root;
}

void CGlobals::setWindow(RenderWindow *argWindow)
{
    window = argWindow;
}

RenderWindow* CGlobals::getWindow()
{
    return window;
}

void CGlobals::setScene(SceneManager *argScene)
{
    scene = argScene;
}

SceneManager* CGlobals::getScene()
{
    return scene;
}

void CGlobals::setCamera(Camera *argCamera)
{
    camera = argCamera;
}

Camera* CGlobals::getCamera()
{
    return camera;
}


