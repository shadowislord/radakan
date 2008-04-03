
#include "display.h"

CDisplay::CDisplay(int width, int height, bool fullScreen)
{
    // Set the current render system to OpenGL
    globals->getRoot()->restoreConfig();

    // Create the Ogre window
    globals->getRoot()->initialise(true, "H.A.W.K");
    globals->setWindow(globals->getRoot()->getAutoCreatedWindow());

    cout << "Display initialised to:" << width << "x" << height << "\r\n";

    // Create the scene manager
    SceneManager *scene = globals->getRoot()->createSceneManager(ST_EXTERIOR_CLOSE,
                                                                 "Scene");
    globals->setScene(scene);

    // Create the camera
    Camera *camera = globals->getScene()->createCamera("Player");
    camera->setAspectRatio((float)width/height);
    camera->setNearClipDistance(1.0f);
    camera->setFarClipDistance(1000.0f);
    camera->setPosition(Vector3(0, 0, 500));
    camera->lookAt(0, 0, 0);

    Viewport *vp = globals->getWindow()->addViewport(camera);
    vp->setBackgroundColour(ColourValue(0, 0, 255));
}
