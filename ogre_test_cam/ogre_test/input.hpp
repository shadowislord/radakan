#ifndef __INPUT_HEADER__
#define __INPUT_HEADER__

using namespace Ogre;
using namespace OIS;
using namespace std;

class Input : public Ogre::FrameListener, public OIS::KeyListener, public OIS::MouseListener
{

public:

	Input (RenderWindow* window, Camera* camera, Player_Character* player, Extended_Camera* ex_cam);
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

	// OIS
	Keyboard* ois_keyboard;
	Mouse* ois_mouse;

	// OGRE
	RenderWindow* ogre_window;
	Camera* ogre_camera;

	SceneManager* ogre_scene_mgr;

	// 3rd person camera stuff
	Player_Character * player_char;
	Extended_Camera * ex_camera;

};

#endif