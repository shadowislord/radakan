#ifndef RADAKAN_ENGINES_INPUT_REGISTRATOR_HPP
#define RADAKAN_ENGINES_INPUT_REGISTRATOR_HPP

#include <OISKeyboard.h>
#include <OISMouse.h>

#include "observer.hpp"
#include "singleton.hpp"

using namespace std;

namespace OIS
{
	class InputManager;
	class Keyboard;
	class KeyEvent;
	class Mouse;
	class MouseEvent;
}

namespace Radakan
{
	namespace Messages
	{
		class Button_Event;
		class List_Event;
		class Player_Input;
	}
	
	namespace Engines
	{
		namespace Input
		{
		
			///	Registrator records all player input.
			class Registrator :
				public Observer <Messages :: Button_Event>,	//	I observe GUIs.
				public Observer <Messages :: List_Event>,	//	I observe GUIs.
				public Singleton <Registrator>,
				public OIS :: KeyListener,
				public OIS :: MouseListener
			{
				private :
					static string to_string (OIS :: MouseButtonID id);
					static string convert (string key_value);

				public :
					static string get_class_name ();
					
					Registrator ();
					virtual ~Registrator ();
					virtual bool is_initialized () const;

					void capture ();
					
					virtual void call (Reference <Messages :: Button_Event> message);
					virtual void call (Reference <Messages :: List_Event> message);
					
				private :
					virtual bool keyPressed (const OIS :: KeyEvent & key_event);
					virtual bool keyReleased (const OIS :: KeyEvent & key_event);

					virtual bool mouseMoved (const OIS :: MouseEvent & mouse_event);
					virtual bool mousePressed
						(const OIS :: MouseEvent & mouse_event, OIS :: MouseButtonID id);
					virtual bool mouseReleased
						(const OIS :: MouseEvent & mouse_event, OIS :: MouseButtonID id);

					boost :: scoped_ptr <OIS :: Mouse> mouse;
					boost :: scoped_ptr <OIS :: Keyboard> keyboard;
					boost :: shared_ptr <OIS :: InputManager> input_manager;
			};
		}
	}
}

#endif	//	RADAKAN_ENGINES_INPUT_REGISTRATOR_HPP
