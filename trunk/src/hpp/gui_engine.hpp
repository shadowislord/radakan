#ifndef RADAKAN_GUI_ENGINE_HPP
#define RADAKAN_GUI_ENGINE_HPP

#include "gui.hpp"
#include "state_machine.hpp"
#include "singleton.hpp"

using namespace std;

namespace Radakan
{
	namespace Engines
	{
		///	GUI_Engine manages the GUIs.
		class GUI_Engine :
			public Singleton <GUI_Engine>,
			public State_Machine <GUI>
		{
			public :
				GUI_Engine
				(
					Ogre :: RenderWindow & window,
					Ogre :: SceneManager & scene_manager
				);
				virtual ~GUI_Engine ();
				virtual bool is_initialized () const;
				
				static const string get_class_name ();
				
				void set_active_gui (GUI & gui);

				void set_mouse_position (const Ogre :: Vector3 & new_position);
				void left_mouse_button_click ();
				
				void render () const;
				
				GUI & create_gui (string configuration_file);

			private :
				//	TODO solve the problems that occur when renderer is a plain object
				//	instead of a pointer.
				CEGUI :: OgreCEGUIRenderer * renderer;
				
				CEGUI :: System * system;
		};
	}
}

#endif	//	RADAKAN_GUI_ENGINE_HPP
