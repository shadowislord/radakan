
#include "input.hpp"

Input::Input (RenderWindow* window, Camera* camera, CEGUI::OgreCEGUIRenderer* renderer)
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

	// 
	ogre_camera = camera;
	ogre_window = window;
	cegui_renderer = renderer;

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

	// Move the cursor
	CEGUI::System::getSingleton ().injectMouseMove (arg.state.relX, arg.state.relY);

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

	// Ensure that our window has not been closed
	if (ogre_window->isClosed ()) return false;

	// Enable widget automation
	CEGUI::System::getSingleton ().injectTimePulse (time_elapsed);

	// Update the keyboard and mouse state
	ois_keyboard->capture ();
	ois_mouse->capture ();
	const OIS::MouseState& mouse_state = ois_mouse->getMouseState ();

	// Handle input
	if (ois_keyboard->isKeyDown (OIS::KC_ESCAPE)) return false;

	return true;
}

bool Input::frameEnded (const FrameEvent& evt)
{
	return true;
}

