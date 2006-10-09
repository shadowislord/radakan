#ifndef __INPUT_HEADER__
#define __INPUT_HEADER__

#include <Ogre.h>
#include <OIS.h>

using namespace Ogre;
using namespace OIS;
using namespace std;

class Input : public Ogre::FrameListener, public OIS::KeyListener, public OIS::MouseListener
{

public:

	Input (RenderWindow* window, Camera* camera);
	~Input ();

	// OIS related events
	virtual bool keyPressed (const OIS::KeyEvent& arg);
	virtual bool keyReleased (const OIS::KeyEvent& arg);

	virtual bool mouseMoved (const OIS::MouseEvent& arg);
	virtual bool mousePressed (const OIS::MouseEvent& arg, OIS::MouseButtonID id);
	virtual bool mouseReleased (const OIS::MouseEvent& arg, OIS::MouseButtonID id);

	// Ogre related events
	virtual bool frameStarted (const FrameEvent& evt);
	virtual bool frameEnded (const FrameEvent& evt);

private:

	size_t window_handle;

	Keyboard* ois_keyboard;
	Mouse* ois_mouse;

	// Reference to some of our ogre stuff
	Camera* ogre_camera;
	RenderWindow* ogre_window;

	SceneManager* scene_mgr;

	Timer* timer;

	// used for the PLSM
	RaySceneQuery* ray_query;
	Ray ray;

};

#endif