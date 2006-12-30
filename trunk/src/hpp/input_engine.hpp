#ifndef INPUT_ENGINE_HPP
#define INPUT_ENGINE_HPP

#include <OgreRenderWindow.h>

#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
#include <OISEvents.h>
//	#include <OIS.h>

#include "object.hpp"

using namespace std;

namespace sl
{

	///	Input engine is responsible for recording player input.

	class Input_Engine :
		public Object,
		public Ogre :: Singleton <Input_Engine>,
		private OIS :: KeyListener,
		private OIS :: MouseListener
	{
		public :
			Input_Engine (Ogre :: RenderWindow & window);
			virtual ~Input_Engine ();
			virtual bool is_initialized () const;
			static string get_type_name ();
			void capture ();
			bool get_key (string key, bool reset);
			bool get_mouse_button (string button, bool reset);
			float get_mouse_height (bool relative) const;
			float get_mouse_width (bool relative) const;
			
			static bool is_mouse_button (string button);
			static const string left_mouse_button;
			static const string middle_mouse_button;
			static const string right_mouse_button;

		private :
			virtual bool keyPressed (const OIS :: KeyEvent & key_event);
			virtual bool keyReleased (const OIS :: KeyEvent & key_event);

			virtual bool mouseMoved (const OIS :: MouseEvent & mouse_event);
			virtual bool mousePressed (const OIS :: MouseEvent & mouse_event, OIS :: MouseButtonID id);
			virtual bool mouseReleased (const OIS :: MouseEvent & mouse_event, OIS :: MouseButtonID id);

			map <string, bool> keys;
			map <string, bool> mouse_buttons;
			float relative_mouse_height;
			float relative_mouse_width;
			float absolute_mouse_height;
			float absolute_mouse_width;
			string to_string (OIS :: MouseButtonID id);
			string convert (string key_value);
			
			OIS :: Mouse * mouse;
			OIS :: Keyboard * keyboard;
	};
}

#endif	//	INPUT_ENGINE_HPP
