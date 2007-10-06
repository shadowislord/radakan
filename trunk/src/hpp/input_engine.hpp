#ifndef RADAKAN_INPUT_ENGINE_HPP
#define RADAKAN_INPUT_ENGINE_HPP

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
	class GUI;

	namespace Messages
	{
		class Conversation_Message;
	}
	
	namespace Engines
	{

		///	Input_Engine records all player input.
		class Input_Engine :
			public Singleton <Input_Engine>,
			public Observer <GUI>,
			public OIS :: KeyListener,
			public OIS :: MouseListener
		{
			public :
				static string get_class_name ();
				
				Input_Engine (boost :: shared_ptr <Ogre :: RenderWindow> window);
				virtual ~Input_Engine ();
				virtual bool is_initialized () const;
				
				using Singleton <Input_Engine> :: get;
				using Singleton <Input_Engine> :: is_instantiated;

				void capture ();
				bool get_gui_button (string button);
				const Reference <Messages :: Conversation_Message> get_conversation_option ();
				bool get_key (string key, bool reset);
				bool get_mouse_button (string button, bool reset);
				const Ogre :: Vector3 & get_mouse_position (bool relative = absolute) const;
				static bool is_mouse_button (string button);
				
				static const bool absolute;
				static const bool relative;
				static const string left_mouse_button;
				static const string middle_mouse_button;
				static const string right_mouse_button;

				virtual void call (const Reference <Object> message = update);
				
			private :
				virtual bool keyPressed (const OIS :: KeyEvent & key_event);
				virtual bool keyReleased (const OIS :: KeyEvent & key_event);

				virtual bool mouseMoved (const OIS :: MouseEvent & mouse_event);
				virtual bool mousePressed
					(const OIS :: MouseEvent & mouse_event, OIS :: MouseButtonID id);
				virtual bool mouseReleased
					(const OIS :: MouseEvent & mouse_event, OIS :: MouseButtonID id);

				///	Multiple keys can be pressed at once.
				map <string, bool> keys;
				
				///	Multiple mouse buttons can be pressed at once.
				map <string, bool> mouse_buttons;
				
				///	Only one gui button can be clicked at once.
				string gui_button;
				
				///	Only one conversation option can be clicked at once.
				Reference <Messages :: Conversation_Message> conversation_option;
				
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
