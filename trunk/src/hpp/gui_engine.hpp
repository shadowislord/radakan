#ifndef TSL_GUI_ENGINE_HPP
#define TSL_GUI_ENGINE_HPP

#include "gui.hpp"
#include "data_state_machine.hpp"
#include "singleton.hpp"

using namespace std;

namespace tsl
{
	class Input_Engine;

	class GUI_Engine :
		public Singleton <GUI_Engine>,
		public Data_State_Machine <GUI>
	{
		public :
			GUI_Engine
			(
				Ogre :: RenderWindow & window,
				Ogre :: SceneManager & scene_manager,
				string log_file_name
			);
			virtual ~GUI_Engine ();
			virtual bool is_initialized () const;
			static string get_class_name ();
			
			void set_mouse_position (const Ogre :: Vector3 & new_position);
			void left_mouse_button_click ();
			
			//	TODO: Make render () const.
			bool render ();
			
			void activate (GUI & gui);
			GUI & create_gui (string configuration_file);

		private :
			CEGUI :: OgreCEGUIRenderer * renderer;
			CEGUI :: System * system;
	};
}

#endif	//	TSL_GUI_ENGINE_HPP
