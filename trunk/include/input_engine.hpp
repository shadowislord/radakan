#ifndef INPUT_ENGINE_HPP
#define INPUT_ENGINE_HPP

#include <Ogre.h>

#include <OIS.h>

#include "exit_event.hpp"
#include "move_event.hpp"
#include "engine.hpp"

using namespace std;

///	Input engine is responsible for translating player input into events and actions.

class Input_Engine :
	public Engine,
	public OIS :: KeyListener,
	public OIS :: MouseListener
{
	public:
		Input_Engine ();
		virtual ~Input_Engine ();

		void start_listening (Ogre :: RenderWindow * window);

		virtual bool is_initialized () const;
		virtual Event * process (Event * event);

	protected:
		virtual bool keyPressed (const OIS :: KeyEvent & key_event);
		virtual bool keyReleased (const OIS :: KeyEvent & key_event);

		virtual bool mouseMoved (const OIS :: MouseEvent & mouse_event);
		virtual bool mousePressed (const OIS :: MouseEvent & mouse_event, OIS :: MouseButtonID id);
		virtual bool mouseReleased (const OIS :: MouseEvent & mouse_event, OIS :: MouseButtonID id);

	private:
		OIS :: InputManager * input_manager;
		OIS :: Mouse * mouse;
		OIS :: Keyboard * keyboard;

		bool going_forward;
		bool going_backward;
		bool going_left;
		bool going_right;
};

#endif	//	INPUT_ENGINE_HPP
