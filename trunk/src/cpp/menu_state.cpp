#include "menu_state.hpp"
#include "play_state.hpp"

using namespace std;
using namespace tsl;

//  constructor
Menu_State ::
	Menu_State (TSL & new_owner) :
	Object (new_owner + "'s menu state"),
	Singleton <Menu_State> (new_owner + "'s menu state"),
	State <TSL> (new_owner),
	gui (GUI_Engine :: get () . create_gui ("menu.cfg"))
{
	trace () << "Menu_State ()" << endl;
	assert (State <TSL> :: is_initialized ());

	assert (Menu_State :: is_initialized ());
}

//  destructor
Menu_State ::
	~Menu_State ()
{
	trace () << "~Menu_State ()" << endl;

	assert (State <TSL> :: is_initialized ());
}

//	virtual
bool Menu_State ::
	is_initialized ()
	const
{
	return State <TSL> :: is_initialized ();
}

//	static
string Menu_State ::
	get_class_name ()
{
	return "Menu_State";
}

//	virtual
string Menu_State ::
	run ()
{
	assert (is_initialized ());

	//	un-pause
	if (Input_Engine :: get () . get_key ("Escape", true)
		|| Input_Engine :: get () . get_gui_button ("Return", true))
	{
		owner . change_active_state <Play_State> ();
		gui . show ("Game resumed");
	}
	
	//	quit
	if (Input_Engine :: get () . get_gui_button ("Quit", true))
	{
		return owner . quit;
	}
	
	//	FPS
	if (Input_Engine :: get () . get_gui_button ("Statistics", true))
	{
		gui . show (owner . get_FPS ());
	}

	GUI_Engine :: get () . activate (gui);

	return owner . go_on;
}
