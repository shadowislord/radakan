#ifndef GUI_ENGINE_HPP
#define GUI_ENGINE_HPP

#include <OgreRenderWindow.h>
#include <OgreCEGUIRenderer.h>
#include <CEGUI.h>

#include "object.hpp"

using namespace std;

namespace sl
{
	class GUI_Engine :
		public Object,
		public Ogre :: Singleton <GUI_Engine>
	{
		public :
			GUI_Engine (Ogre :: RenderWindow * window);
			virtual ~GUI_Engine ();
			virtual bool is_initialized () const;
			bool render () const;

		private :
			CEGUI :: OgreCEGUIRenderer * renderer;
			CEGUI :: Window * root_window;
			CEGUI :: System * system;
			CEGUI :: WindowManager * window_manager;
	};
}

#endif	//	GUI_ENGINE_HPP
