#ifndef __C_GLOBALS_CLASS__
#define __C_GLOBALS_CLASS__

#include <ogre.h>

using namespace Ogre;

class CGlobals
{
    public:
    ~CGlobals();
    static CGlobals* Instance();

    Root* getRoot();

    void setWindow(RenderWindow *argWindow);
    RenderWindow* getWindow();

    void setScene(SceneManager *argScene);
    SceneManager* getScene();

    void setCamera(Camera *argCamera);
    Camera* getCamera();

    private:
    CGlobals();
    CGlobals(const CGlobals &);
    CGlobals& operator=(const CGlobals &);

    Root *root;
    RenderWindow *window;
    SceneManager *scene;
    Camera *camera;
};

#define globals CGlobals::Instance()

#endif
