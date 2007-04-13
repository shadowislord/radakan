#ifndef TSL_INPUT_LISTENER_HPP
#define TSL_INPUT_LISTENER_HPP

#include <CEGUIWindow.h>
#include "singleton.hpp"

using namespace std;

namespace tsl
{
	class GUI;
	
	class GUI_Listener :
		public Singleton <GUI_Listener>
	{
		public :
			GUI_Listener ();
			virtual ~GUI_Listener ();
			virtual bool is_initialized () const;
			static string get_class_name ();

			///	searches recursively for button windows
			void subscribe (CEGUI :: Window & window);
			
			virtual bool handle_gui_button (const CEGUI :: EventArgs & arguments) = 0;

		private :
			CEGUI :: Event :: Subscriber subscriber;
	};
}

#endif	//	TSL_INPUT_LISTENER_HPP
