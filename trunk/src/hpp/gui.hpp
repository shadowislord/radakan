#ifndef GUI_HPP
#define GUI_HPP

#include "gui_listener.hpp"
#include "state.hpp"
#include <OgreRenderWindow.h>
#include <OgreCEGUIRenderer.h>

using namespace std;

namespace tsl
{
	class GUI_Engine;

	class GUI :
		public State <GUI_Engine>
	{
		public :
			GUI
			(
				GUI_Engine & owner,
				CEGUI :: Window & new_root,
				GUI_Listener & gui_listener
			);
			virtual ~GUI ();
			virtual bool is_initialized () const;
			static string get_class_name ();
			void show (string new_message);
			string run ();	//	updates the message
			CEGUI :: Window & root_window;

		private :
			CEGUI :: Window * text_window;
			static string message;
	};
}

#endif	//	GUI_HPP
