#ifndef RADAKAN_GUI_ENGINE_HPP
#define RADAKAN_GUI_ENGINE_HPP

#include "state_machine.hpp"
#include "singleton.hpp"

using namespace std;

namespace CEGUI
{
	class OgreCEGUIRenderer;
	class System;
}

namespace Ogre
{
	class RenderWindow;
	class SceneManager;
}

namespace Radakan
{
	class GUI;

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
					boost :: shared_ptr <Ogre :: RenderWindow> window,
					boost :: shared_ptr <Ogre :: SceneManager> scene_manager
				);
				virtual ~GUI_Engine ();
				virtual bool is_initialized () const;
				
				static string get_class_name ();
				
				void set_active_gui (Reference <GUI> gui);

				void set_mouse_position (const Ogre :: Vector3 & new_position);
				void left_mouse_button_click ();
				
				void render () const;
				
				Reference <GUI> create_gui (string configuration_file);

			private :
				//	TODO solve the problems that occur when renderer is a plain object
				//	instead of a pointer.
				boost :: shared_ptr <CEGUI :: OgreCEGUIRenderer> renderer;
				
				boost :: shared_ptr <CEGUI :: System> system;
		};
	}
}

#endif	//	RADAKAN_GUI_ENGINE_HPP
