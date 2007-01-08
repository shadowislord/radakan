#ifndef GUI_ENGINE_HPP
#define GUI_ENGINE_HPP

#include <OgreRenderWindow.h>
#include <OgreCEGUIRenderer.h>
#include <CEGUI.h>

#include "object.hpp"

using namespace std;

namespace tsl
{
	class GUI_Engine :
		public Object,
		public Ogre :: Singleton <GUI_Engine>
	{
		public :
			GUI_Engine (Ogre :: RenderWindow & window, string log_file_name);
			virtual ~GUI_Engine ();
			virtual bool is_initialized () const;
			static string get_type_name ();
			void set_scene_manager (Ogre :: SceneManager & new_scene_manager);
			void set_mouse_position (pair <float, float> new_position);
			bool render () const;

		private :
			CEGUI :: OgreCEGUIRenderer * renderer;
			CEGUI :: Window * root_window;
			CEGUI :: System * system;
			CEGUI :: WindowManager * window_manager;
	};
}

#endif	//	GUI_ENGINE_HPP
