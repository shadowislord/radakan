
#ifndef INPUT_ENGINE_HPP
#define INPUT_ENGINE_HPP

#include <ogre.h>

#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
#include <OISEvents.h>

#include "event.hpp"
#include "engine.hpp"

using namespace std;

///	Input engine is responsible for translating player input into events and actions.

class Input_Engine : public Engine,
                     public OIS::KeyListener,
                     public OIS::MouseListener
{
	public:
		Input_Engine ();
		virtual ~Input_Engine ();

		void start_listening(Ogre::RenderWindow *window);

		virtual bool is_initialized () const;
		virtual Event * process (Event * event);

        bool keyPressed(const OIS::KeyEvent& arg);
        bool keyReleased(const OIS::KeyEvent&);

        bool mouseMoved(const OIS::MouseEvent& arg);
        bool mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID);
        bool mouseReleased(const OIS::MouseEvent&, OIS::MouseButtonID);

	private:
        OIS::InputManager *input_manager;

        OIS::Mouse* mouse;
        OIS::Keyboard* keyboard;
};

#endif	//	INPUT_ENGINE_HPP
