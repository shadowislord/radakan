#ifndef TSL_GUI_HPP
#define TSL_GUI_HPP

#include "log.hpp"
#include "observer.hpp"
#include <OgreRenderWindow.h>
#include <OgreCEGUIRenderer.h>
#include <CEGUIWindow.h>

using namespace std;

namespace TSL
{

	///	GUI is a graphical user interface.
	class GUI :
		public Observer <Engines :: Log>,
		public Observable <GUI>
	{
		public :
			GUI
			(
				string new_name,
				CEGUI :: Window & new_root
			);
			virtual ~GUI ();
			virtual bool is_initialized () const;
			
			static const string get_class_name ();
			
			CEGUI :: Window & root_window;

			virtual void call (const Object & message);

		private :
			void subscribe (CEGUI :: Window & window);

			virtual bool handle_event (const CEGUI :: EventArgs & arguments);

			CEGUI :: Event :: Subscriber subscriber;
			
			CEGUI :: Window * text_window;
	};
}

#endif	//	TSL_GUI_HPP
