#include <Ogre.h>
#include <OIS.h>

#include "character.hpp"
#include "camera.hpp"

#include "input.hpp"

Input::Input (RenderWindow* window, Camera* camera, Player_Character* player, Extended_Camera* ex_cam)
{
	ParamList parameters;
	ostringstream window_handle_string;

	ogre_window = window;
	ogre_camera = camera;
	player_char = player;
	ex_camera = ex_cam;
	ogre_scene_mgr = ogre_camera->getSceneManager ();

	// Get a handle to the window (OS specific)
	#if defined OIS_WIN32_PLATFORM
		window->getCustomAttribute ("HWND", &window_handle);
	#elif defined OIS_LINUX_PLATFORM
		window->getCustomAttribute ("GLXWINDOW", &window_handle);
	#endif

	window_handle_string << window_handle;
	parameters.insert (make_pair (string ("WINDOW"), window_handle_string.str ()));

	// Initialise the OIS input manager
	InputManager& ois_input_manager = *InputManager::createInputSystem (parameters);

	// Enable unbuffered keyboard support
	ois_keyboard = static_cast<Keyboard*>(ois_input_manager.createInputObject (OISKeyboard, false));

	// Enable buffered mouse support
	ois_mouse = static_cast<Mouse*>(ois_input_manager.createInputObject (OISMouse, true));

	// Define the window outline for our mouse
	unsigned int width, height, depth;
	int left, top;
	window->getMetrics (width, height, depth, left, top);

	const OIS::MouseState& mouse_state = ois_mouse->getMouseState ();
	mouse_state.width = width;
	mouse_state.height = height;

	// Set this class as the callback class
	ois_keyboard->setEventCallback (this);
	ois_mouse->setEventCallback (this);
}

Input::~Input ()
{
}

// OIS related events
bool Input::keyPressed (const OIS::KeyEvent& arg)
{
	return true;
}

bool Input::keyReleased (const OIS::KeyEvent& arg)
{
		return true;
}

bool Input::mouseMoved (const OIS::MouseEvent& arg)
{
	return true;
}

bool Input::mousePressed (const OIS::MouseEvent& arg, OIS::MouseButtonID id)
{
	return true;
}

bool Input::mouseReleased (const OIS::MouseEvent& arg, OIS::MouseButtonID id)
{
	return true;
}


// Ogre related events
// frameStarted is executed every loop
bool Input::frameStarted (const FrameEvent& evt)
{

	float time_elapsed = static_cast<float>(evt.timeSinceLastFrame);

	// Update the keyboard and mouse state
	ois_keyboard->capture ();
	ois_mouse->capture ();
	const OIS::MouseState& mouse_state = ois_mouse->getMouseState ();

	// Handle input
	if (ois_keyboard->isKeyDown (OIS::KC_ESCAPE)) return false;

	// Update the player
	player_char->update (time_elapsed, ois_keyboard);

	// Update the camera
	ex_camera->update (time_elapsed,
		player_char->get_camera_node ()->getWorldPosition (),
		player_char->get_sight_node ()->getWorldPosition ());

	return true;
}

bool Input::frameEnded (const FrameEvent& evt)
{
	return true;
}

