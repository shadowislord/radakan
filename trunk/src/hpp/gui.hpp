#ifndef TSL_GUI_HPP
#define TSL_GUI_HPP

#include <OgreRenderWindow.h>
#include <OgreCEGUIRenderer.h>
#include "gui_listener.hpp"

using namespace std;

namespace tsl
{
	class GUI :
		public virtual Object
	{
		public :
			GUI
			(
				string new_name,
				CEGUI :: Window & new_root,
				GUI_Listener & gui_listener
			);
			virtual ~GUI ();
			virtual bool is_initialized () const;
			static string get_class_name ();
			void show (string new_message);
			string update_message ();	//	updates the message
			CEGUI :: Window & root_window;

		private :
			CEGUI :: Window * text_window;
			static string message;
	};
}

#endif	//	TSL_GUI_HPP
