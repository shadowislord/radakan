#ifndef INPUT_LISTENER_HPP
#define INPUT_LISTENER_HPP

#include "object.hpp"
#include <CEGUI.h>

using namespace std;

namespace tsl
{
	class GUI_Listener :
		public virtual Object
	{
		public :
			GUI_Listener ();
			virtual ~GUI_Listener ();
			virtual bool is_initialized () const;
			static string get_class_name ();

			///	searches recursively for button windows
			void subscribe (CEGUI :: Window & window);
			
			virtual bool handle_gui_button (const CEGUI :: EventArgs & arguments) = 0;

		private:
			CEGUI :: Event :: Subscriber event_suscriber;
	};
}

#endif	//	INPUT_LISTENER_HPP
