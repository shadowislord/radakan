#include "play_state.hpp"

using namespace std;
using namespace tsl;

//  constructor
Play_State ::
	Play_State (TSL & new_owner) :
	Object (new_owner + "'s play state"),
	Singleton <Play_State> (new_owner + "'s play state"),
	State_Machine <TSL> (new_owner),
	gui (GUI_Engine :: get () . create_gui ("sector.cfg"))
{
	assert (State_Machine <TSL> :: is_initialized ());

	add (* (new Sector ("Sector 1", owner, gui)));
	add (* (new Sector ("Sector 2", owner, gui)));

	assert (Play_State :: is_initialized ());
}

//  destructor
Play_State ::
	~Play_State ()
{
	trace () << "~Play_State ()" << endl;
	
	assert (State_Machine <TSL> :: is_initialized ());
}

//	virtual
bool Play_State ::
	is_initialized ()
	const
{
	return State_Machine <TSL> :: is_initialized ();
}

//	static
string Play_State ::
	get_class_name ()
{
	return "Play_State";
}

//	virtual
void Play_State ::
	change_active_state (State <TSL> & state)
{
	assert (state . is_type <Sector> ());
	
	if (state != get_active_state ())
	{
		Sector & old_active_sector = get_active_state () . to_type <Sector> ();
		Sector & new_active_sector = state . to_type <Sector> ();
	
		//	Update player position:
		old_active_sector . move_to (Player :: get (), new_active_sector);
		Player :: get () . node = & new_active_sector . copy_node (* Player :: get() . node);
		
		State_Machine <TSL> :: change_active_state (state);

		//	Update camera & scene manager:
		owner . set_camera (new_active_sector . get_camera ());
		GUI_Engine :: get () . set_scene_manager (new_active_sector . get_scene_manager ());
	}
}
