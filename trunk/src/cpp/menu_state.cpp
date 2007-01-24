#include "menu_state.hpp"
#include "play_state.hpp"
#include "quit_state.hpp"

using namespace std;
using namespace tsl;

//  constructor
Menu_State ::
	Menu_State () :
	Object ("menu state"),
	gui (GUI_Engine :: get () . create_gui ("menu.cfg"))
{
	trace () << "Menu_State ()" << endl;
	assert (Algorithm <TSL> :: is_initialized ());

	assert (Menu_State :: is_initialized ());
}

//  destructor
Menu_State ::
	~Menu_State ()
{
	trace () << "~" << get_class_name () << " ()" << endl;

	assert (Algorithm <TSL> :: is_initialized ());
}

//	virtual
bool Menu_State ::
	is_initialized ()
	const
{
	return Algorithm <TSL> :: is_initialized ();
}

//	static
string Menu_State ::
	get_class_name ()
{
	return "Menu_State";
}

//	virtual
Algorithm <TSL> & Menu_State ::
	transit (TSL & owner)
{
	assert (is_initialized ());

	//	un-pause
	if (Input_Engine :: get () . get_key ("Escape", true)
		|| Input_Engine :: get () . get_gui_button ("Return", true))
	{
		owner . set_active_state <Play_State> ();
		gui . show ("Game resumed");
	}
	
	//	quit
	if (Input_Engine :: get () . get_gui_button ("Quit", true))
	{
		return Quit_State :: get ();
	}
	
	//	FPS
	if (Input_Engine :: get () . get_gui_button ("Statistics", true))
	{
		gui . show (owner . get_FPS ());
	}

	GUI_Engine :: get () . activate (gui);

	return * this;
}
