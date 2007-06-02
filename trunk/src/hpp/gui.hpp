#ifndef TSL_GUI_HPP
#define TSL_GUI_HPP

#include "log.hpp"
#include "observer.hpp"
#include "resident.hpp"
#include <CEGUIWindow.h>
#include <OgreCEGUIRenderer.h>
//	#include <OgreRenderWindow.h>

using namespace std;

namespace TSL
{

	///	I'm a graphical user interface.
	class GUI :
		public Resident <GUI>,
		public Observable <GUI>,
		public Observer <Engines :: Log>
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
			
			CEGUI :: Listbox * log_window;
	};
}

#endif	//	TSL_GUI_HPP
