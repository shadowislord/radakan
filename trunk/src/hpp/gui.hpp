#ifndef TSL_GUI_HPP
#define TSL_GUI_HPP

#include <OgreRenderWindow.h>
#include "/home/t/opt/ogre-1.4/Samples/Common/CEGUIRenderer/include/OgreCEGUIRenderer.h"
#include "gui_listener.hpp"
#include "log.hpp"
#include "observer.hpp"

using namespace std;

namespace TSL
{
	class GUI :
		public Observer <Log>
	{
		public :
			GUI
			(
				string new_name,
				CEGUI :: Window & new_root
			);
			virtual ~GUI ();
			virtual bool is_initialized () const;
			
			static const string class_name;
			
			CEGUI :: Window & root_window;

			virtual void call (const string & type, const string & message);

		private :
			CEGUI :: Window * text_window;
	};
}

#endif	//	TSL_GUI_HPP
