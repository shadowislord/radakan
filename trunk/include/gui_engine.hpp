#ifndef GUI_ENGINE_HPP
#define GUI_ENGINE_HPP

#include <OgreRenderWindow.h>
#include <OgreCEGUIRenderer.h>
#include <CEGUI.h>

#include "engine.hpp"

using namespace std;

class GUI_Engine:
	public Engine
{
	public:
		GUI_Engine (Ogre :: RenderWindow * window, Ogre :: SceneManager * scene_manager);
		virtual ~GUI_Engine ();
		virtual bool is_initialized () const;
		void render () const;

	private:
		CEGUI :: OgreCEGUIRenderer * renderer;
		CEGUI :: Window * root_window;
		CEGUI :: System * system;
		CEGUI :: WindowManager * window_manager;

};

#endif	//	GUI_ENGINE_HPP
