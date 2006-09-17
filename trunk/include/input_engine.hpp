
#ifndef INPUT_ENGINE_HPP
#define INPUT_ENGINE_HPP

#include <Ogre.h>

#include <OIS.h>

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

    protected:
        virtual bool keyPressed(const OIS::KeyEvent& arg);
        virtual bool keyReleased(const OIS::KeyEvent&);

        virtual bool mouseMoved(const OIS::MouseEvent& arg);
        virtual bool mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID);
        virtual bool mouseReleased(const OIS::MouseEvent&, OIS::MouseButtonID);

	private:
        OIS::InputManager *input_manager;

        OIS::Mouse* mouse;
        OIS::Keyboard* keyboard;
};

#endif	//	INPUT_ENGINE_HPP
