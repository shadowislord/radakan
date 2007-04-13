#include "gui_listener.hpp"
#include <elements/CEGUIPushButton.h>

using namespace std;
using namespace tsl;


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
	log (debugging) << "~" << get_class_name () << " ()" << endl;
	assert (is_initialized ());
}

//	virtual
bool GUI_Listener ::
	is_initialized ()
	const
{
	return Singleton <GUI_Listener> :: is_initialized ();
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
