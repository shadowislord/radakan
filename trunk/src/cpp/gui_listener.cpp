#include "gui_listener.hpp"

using namespace std;
using namespace tsl;


GUI_Listener ::
	GUI_Listener () :
	Object ("gui listener"),
	event_suscriber (& GUI_Listener :: handle_gui_button, this)
{
	assert (Object :: is_initialized ());

	assert (is_initialized ());
}

GUI_Listener ::
	~GUI_Listener ()
{
	assert (is_initialized ());

}

//	virtual
bool GUI_Listener ::
	is_initialized ()
	const
{
	return warn <GUI_Listener> (Object :: is_initialized ());
}

//	static
string GUI_Listener ::
	get_class_name ()
{
	return "GUI_Listener";
}

void GUI_Listener ::
	subscribe (CEGUI :: Window & window)
{
	CEGUI :: Window * temp;
	for (unsigned int i = 0; i < window . getChildCount (); i ++)
	{
		temp = window . getChildAtIdx (i);
		if (temp -> getType () . find ("Button") != string :: npos)
		{
			temp -> subscribeEvent (CEGUI :: PushButton :: EventClicked, event_suscriber);
		}
		else
		{
			subscribe (* temp);
		}
	}
}
