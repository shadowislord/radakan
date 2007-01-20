#ifndef GUI_ENGINE_HPP
#define GUI_ENGINE_HPP

#include "gui.hpp"
#include "state_machine.hpp"
#include "singleton.hpp"
#include "gui_listener.hpp"

using namespace std;

namespace tsl
{
	class GUI_Engine :
		public Singleton <GUI_Engine>,
		public State_Machine <GUI_Engine>
	{
		public :
			GUI_Engine
			(
				Ogre :: RenderWindow & window,
				string log_file_name,
				GUI_Listener & new_gui_listener
			);
			virtual ~GUI_Engine ();
			virtual bool is_initialized () const;
			static string get_class_name ();
			
			void set_scene_manager (Ogre :: SceneManager & new_scene_manager);
			void set_mouse_position (pair <float, float> new_position);
			void left_mouse_button_click ();
			bool render ();	//	!!!	should be const
			void activate (GUI & gui);
			GUI & create_gui (string configuration_file);
			string run ();	//	does nothing

		private :
			CEGUI :: OgreCEGUIRenderer * renderer;
			CEGUI :: System * system;
			CEGUI :: WindowManager * window_manager;
			
			GUI_Listener & gui_listener;
	};
}

#endif	//	GUI_ENGINE_HPP
