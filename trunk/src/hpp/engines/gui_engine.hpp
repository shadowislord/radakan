#ifndef RADAKAN_GUI_ENGINE_HPP
#define RADAKAN_GUI_ENGINE_HPP

#include "state_machine.hpp"
#include "singleton.hpp"

using namespace std;

#if RADAKAN_GUI_MODE == RADAKAN_CEGUI_MODE
	namespace CEGUI
	{
		class OgreCEGUIRenderer;
		class System;
	}
#endif

namespace Radakan
{
	class GUI;

	namespace Engines
	{
		///	GUI_Engine manages the GUIs.
		class GUI_Engine :
			public State_Machine <GUI>,
			public Singleton <GUI_Engine>
		{
			public :
				GUI_Engine ();
				virtual ~GUI_Engine ();
				virtual bool is_initialized () const;
				
				static string get_class_name ();
				
				void set_active_gui (Reference <GUI> gui);

				void update_and_render ();

				Reference <GUI> create_gui (string configuration_file);

			private :
				#if RADAKAN_GUI_MODE == RADAKAN_CEGUI_MODE
					boost :: scoped_ptr <CEGUI :: OgreCEGUIRenderer> renderer;
					boost :: scoped_ptr <CEGUI :: System> system;
				#endif
		};
	}
}

#endif	//	RADAKAN_GUI_ENGINE_HPP
