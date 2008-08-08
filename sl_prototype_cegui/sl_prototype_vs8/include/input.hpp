#ifndef __INPUT_HEADER__
#define __INPUT_HEADER__

#include <OgreNoMemoryMacros.h>
#include <CEGUI/CEGUIImageset.h>
#include <CEGUI/CEGUISystem.h>
#include <CEGUI/CEGUILogger.h>
#include <CEGUI/CEGUISchemeManager.h>
#include <CEGUI/CEGUIWindowManager.h>
#include <CEGUI/CEGUIWindow.h>
#include "OgreCEGUIRenderer.h"
#include "OgreCEGUIResourceProvider.h"
#include <OgreMemoryMacros.h> 

#include <Ogre.h>
#include <OIS.h>

using namespace Ogre;
using namespace OIS;
using namespace std;

class Input : public Ogre::FrameListener, public OIS::KeyListener, public OIS::MouseListener
{

public:

	Input (RenderWindow* window, Camera* camera, CEGUI::OgreCEGUIRenderer* renderer);
	~Input ();

	// OIS related events
	virtual bool keyPressed (const OIS::KeyEvent& arg);
	virtual bool keyReleased (const OIS::KeyEvent& arg);

	virtual bool mouseMoved (const OIS::MouseEvent& arg);
	virtual bool mousePressed (const OIS::MouseEvent& arg, OIS::MouseButtonID id);
	virtual bool mouseReleased (const OIS::MouseEvent& arg, OIS::MouseButtonID id);

	// Ogre related events (from FrameListener)
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

	// CEGUI stuff
	CEGUI::OgreCEGUIRenderer* cegui_renderer;

};

#endif