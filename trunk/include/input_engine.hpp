#ifndef INPUT_ENGINE_HPP
#define INPUT_ENGINE_HPP

#include <OgreRenderWindow.h>

#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
#include <OISEvents.h>
//	#include <OIS.h>

#include "engine.hpp"

using namespace std;

///	Input engine is responsible for translating player input into events and actions.

class Input_Engine :
	public Engine,
	public OIS :: KeyListener,
	public OIS :: MouseListener
{
	public:
		Input_Engine (Ogre :: RenderWindow * window);
		virtual ~Input_Engine ();
		virtual bool is_initialized () const;
		void capture ();
//		bool is_key_down (OIS :: KeyCode);	the interface should be OIS independant
		bool get_key (string key, bool reset);
		bool get_mouse_buttons (int button, bool reset);
		int get_mouse_height () const;
		int get_mouse_width () const;

	protected:
		virtual bool keyPressed (const OIS :: KeyEvent & key_event);
		virtual bool keyReleased (const OIS :: KeyEvent & key_event);

		virtual bool mouseMoved (const OIS :: MouseEvent & mouse_event);
		virtual bool mousePressed (const OIS :: MouseEvent & mouse_event, OIS :: MouseButtonID id);
		virtual bool mouseReleased (const OIS :: MouseEvent & mouse_event, OIS :: MouseButtonID id);

	private:
		map <string, bool> keys;
		map <int, bool> mouse_buttons;
		int mouse_height;
		int mouse_width;
		
		OIS :: Mouse * mouse;
		OIS :: Keyboard * keyboard;
};

#endif	//	INPUT_ENGINE_HPP
