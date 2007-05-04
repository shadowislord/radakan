#ifndef TSL_GUI_HPP
#define TSL_GUI_HPP

#include <OgreRenderWindow.h>
#include "/home/t/opt/ogre-1.4/Samples/Common/CEGUIRenderer/include/OgreCEGUIRenderer.h"
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
				CEGUI :: Window & new_root
			);
			virtual ~GUI ();
			virtual bool is_initialized () const;
			static string get_class_name ();
			void update_message ();	//	updates the message
			CEGUI :: Window & root_window;

		private :
			GUI (const GUI & gui);
			CEGUI :: Window * text_window;
	};
}

#endif	//	TSL_GUI_HPP
