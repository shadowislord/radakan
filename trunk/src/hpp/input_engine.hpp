#ifndef RADAKAN_INPUT_ENGINE_HPP
#define RADAKAN_INPUT_ENGINE_HPP

#include <OgreRenderWindow.h>

#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
#include <OISEvents.h>
//	#include <OIS.h>

#include "gui_engine.hpp"

using namespace std;

namespace Radakan
{
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
				Input_Engine (Ogre :: RenderWindow & window);
				virtual ~Input_Engine ();
				virtual bool is_initialized () const;
				
				static const string get_class_name ();
				
				using Singleton <Input_Engine> :: get;
				using Singleton <Input_Engine> :: is_instantiated;

				void capture ();
				bool get_gui_button (string button);
				Messages :: Conversation_Message * get_conversation_option ();
				bool get_key (string key, bool reset);
				bool get_mouse_button (string button, bool reset);
				const Ogre :: Vector3 & get_mouse_position (bool relative = absolute) const;
				static bool is_mouse_button (string button);
				
				static const bool absolute;
				static const bool relative;
				static const string left_mouse_button;
				static const string middle_mouse_button;
				static const string right_mouse_button;

				virtual void call (const Object & message);
				
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
				///	'conversation_option' is 'NULL', when no option is selected.
				Messages :: Conversation_Message * conversation_option;
				
				Ogre :: Vector3 relative_mouse_position;
				Ogre :: Vector3 absolute_mouse_position;
				string to_string (OIS :: MouseButtonID id);
				string convert (string key_value);
				
				OIS :: Mouse * mouse;
				OIS :: Keyboard * keyboard;
				OIS :: InputManager * input_manager;
		};
	}
}

#endif	//	RADAKAN_INPUT_ENGINE_HPP
