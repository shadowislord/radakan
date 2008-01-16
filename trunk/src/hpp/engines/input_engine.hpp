#ifndef RADAKAN_INPUT_ENGINE_HPP
#define RADAKAN_INPUT_ENGINE_HPP

#include <OISKeyboard.h>
#include <OISMouse.h>

#include "observer.hpp"
#include "pointer.hpp"
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
	class GUI;

	namespace Items
	{
		namespace Characters
		{
			class Character;
		}
	}
	
	namespace Messages
	{
		class Conversation_Message;
		template <class T> class Message;
	}
	
	namespace Engines
	{

		///	Input_Engine records all player input.
		class Input_Engine :
			public Singleton <Input_Engine>,
			public Observer <Messages :: Message <Items :: Characters :: Character> >,
			public Observer <Object>,
			public OIS :: KeyListener,
			public OIS :: MouseListener
		{
			public :
				static const bool absolute;
				static const bool relative;
				static const string left_mouse_button;
				static const string middle_mouse_button;
				static const string right_mouse_button;
				
				static string get_class_name ();
				
				static bool is_mouse_button (string button);
				
				Input_Engine (boost :: shared_ptr <Ogre :: RenderWindow> window);
				virtual ~Input_Engine ();
				virtual bool is_initialized () const;

				void capture ();
				
				const Reference <Messages :: Message <Items :: Characters :: Character> >
					get_conversation_option ();
				bool is_mouse_button_pressed (string button, bool reset);
				const Ogre :: Vector3 & get_mouse_position (bool relative = absolute) const;
				bool has_command (string command_name, bool reset = true);

				virtual void call
					(const Reference <Messages :: Message <Items :: Characters :: Character> > & message);
				virtual void call (const Reference <Object> & message);
				
			private :
				virtual bool keyPressed (const OIS :: KeyEvent & key_event);
				virtual bool keyReleased (const OIS :: KeyEvent & key_event);

				virtual bool mouseMoved (const OIS :: MouseEvent & mouse_event);
				virtual bool mousePressed
					(const OIS :: MouseEvent & mouse_event, OIS :: MouseButtonID id);
				virtual bool mouseReleased
					(const OIS :: MouseEvent & mouse_event, OIS :: MouseButtonID id);

				///	Multiple keys can be pressed at once.
				set <string> pressed_keys;
				
				///	Multiple mouse buttons can be pressed at once.
				set <string> pressed_mouse_buttons;
				
				///	Only one gui button can be clicked at once.
				string gui_command;
				
				///	Only one conversation option can be clicked at once.
				///	Note: 'conversation_option' isn't a really good name.
				Pointer <Messages :: Message <Items :: Characters :: Character> > conversation_option;
				
				Ogre :: Vector3 relative_mouse_position;
				Ogre :: Vector3 absolute_mouse_position;
				string to_string (OIS :: MouseButtonID id);
				string convert (string key_value);
				
				boost :: scoped_ptr <OIS :: Mouse> mouse;
				boost :: scoped_ptr <OIS :: Keyboard> keyboard;
				boost :: shared_ptr <OIS :: InputManager> input_manager;
		};
	}
}

#endif	//	RADAKAN_INPUT_ENGINE_HPP
