
#include "input.hpp"

Input::Input (RenderWindow* window, Camera* camera)
{
	ParamList parameters;
	ostringstream window_handle_string;

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

	// Enable unbuffered keyboard and mouse input
	// TODO: Switch this to a more efficient buffered mode.
	ois_keyboard = static_cast<Keyboard*>(ois_input_manager.createInputObject (OISKeyboard, false));
	ois_mouse = static_cast<Mouse*>(ois_input_manager.createInputObject (OISMouse, false));
	// TODO: Joystick support (would be nice)

	// Define the window outline for our mouse
	unsigned int width, height, depth;
	// Possible because this can be negative?
	int left, top;
	window->getMetrics (width, height, depth, left, top);

	const OIS::MouseState& mouse_state = ois_mouse->getMouseState ();
	mouse_state.width = width;
	mouse_state.height = height;

	// Set this class as the callback class
	ois_keyboard->setEventCallback (this);
	ois_mouse->setEventCallback (this);

	ogre_camera = camera;
	ogre_window = window;

	// Create a timer
	timer = PlatformManager::getSingletonPtr ()->createTimer ();

	// Get the scene manager
	scene_mgr = ogre_camera->getSceneManager ();
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


// Ogre related events (from FrameListener)
// frameStarted is executed every loop
bool Input::frameStarted (const FrameEvent& evt)
{

	Vector3 translate = Vector3::ZERO;

	double current_time = timer->getMicroseconds () * 0.0001f;
	double time_elapsed = current_time - last_time;
	last_time = current_time;

	double move_speed = 0.6f * time_elapsed;

	// Ensure that our window has not been closed
	if (ogre_window->isClosed ()) return false;

	ois_keyboard->capture ();
	ois_mouse->capture ();
	const OIS::MouseState& mouse_state = ois_mouse->getMouseState ();

	// Handle input
	if (ois_keyboard->isKeyDown (OIS::KC_W)) translate.z = -move_speed;
	if (ois_keyboard->isKeyDown (OIS::KC_S)) translate.z = move_speed;
	if (ois_keyboard->isKeyDown (OIS::KC_A)) translate.x = -move_speed;
	if (ois_keyboard->isKeyDown (OIS::KC_D)) translate.x = move_speed;
	if (ois_keyboard->isKeyDown (OIS::KC_ESCAPE)) return false;

	ogre_camera->moveRelative (translate);

	// Handle mouse look
	ogre_camera->yaw (Degree (-mouse_state.relX * 0.13));
	ogre_camera->pitch (Degree (-mouse_state.relY * 0.13));

	return true;
}

bool Input::frameEnded (const FrameEvent& evt)
{
	return true;
}

