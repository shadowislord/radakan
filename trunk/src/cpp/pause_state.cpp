#include "pause_state.hpp"
#include "play_state.hpp"

using namespace std;
using namespace tsl;

//  constructor
Pause_State ::
	Pause_State (TSL & new_owner) :
	Object (new_owner + "'s pause state"),
	State <TSL> (new_owner)
{
	trace () << "Pause_State ()" << endl;
	assert (State <TSL> :: is_initialized ());

	assert (Pause_State :: is_initialized ());
}

//  destructor
Pause_State ::
	~Pause_State ()
{
	trace () << "~Pause_State ()" << endl;

	assert (State <TSL> :: is_initialized ());
}

//	virtual
bool Pause_State ::
	is_initialized ()
	const
{
	return State <TSL> :: is_initialized ();
}

//	static
string Pause_State ::
	get_type_name ()
{
	return "pause state";
}

//	virtual
string Pause_State ::
	run ()
{
	assert (is_initialized ());

	//	un-pause
	if (Input_Engine :: get () . get_key ("p", true))
	{
		owner . change_active_state <Play_State> ();
		GUI_Engine :: get () . show ("Game unpaused");
	}

	if (Input_Engine :: get () . get_mouse_button
					(Input_Engine :: get () . left_mouse_button, true))
	{
		GUI_Engine :: get () . left_mouse_button_click ();
	}

	if (Input_Engine :: get () . get_mouse_button
					(Input_Engine :: get () . right_mouse_button, true))
	{
		if (0.1 < Ogre :: Math :: RangeRandom (0, 1))
		{
			GUI_Engine :: get () . show
				("FPS: " + to_string (owner . get_FPS ()));
		}
		else
		{
			GUI_Engine :: get () . show
				("Trivia: there are 1961 trees in each forest.");
		}
	}
	
	return "continue";
}
