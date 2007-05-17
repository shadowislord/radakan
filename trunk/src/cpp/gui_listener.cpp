#include "gui_listener.hpp"
#include "log.hpp"
#include <elements/CEGUIPushButton.h>

using namespace std;
using namespace TSL;

//	static
const string GUI_Listener ::
	get_class_name ()
{
	return "GUI_Listener";
}

GUI_Listener ::
	GUI_Listener () :
	Object ("gui listener"),
	subscriber (& GUI_Listener :: handle_gui_button, this)
{
	assert (Singleton <GUI_Listener> :: is_initialized ());

	assert (is_initialized ());
}

GUI_Listener ::
	~GUI_Listener ()
{
	Log :: trace <GUI_Listener> (me, "~");
	assert (is_initialized ());
}

//	virtual
bool GUI_Listener ::
	is_initialized ()
	const
{
	return Singleton <GUI_Listener> :: is_initialized ();
}

void GUI_Listener ::
	subscribe (CEGUI :: Window & window)
{
	assert (is_initialized ());
	CEGUI :: Window * temp;
	for (unsigned int i = 0; i < window . getChildCount (); i ++)
	{
		temp = window . getChildAtIdx (i);
		if (temp -> getType () . find ("Button") != string :: npos)
		{
			temp -> subscribeEvent (CEGUI :: PushButton :: EventClicked, subscriber);
		}
		else
		{
			subscribe (* temp);
		}
	}
}
