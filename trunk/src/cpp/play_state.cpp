#include "play_state.hpp"

using namespace std;
using namespace tsl;

//  constructor
Play_State ::
	Play_State (TSL & new_owner) :
	Object (new_owner + "'s play state"),
	State_Machine <TSL> (new_owner)
{
	assert (State_Machine <TSL> :: is_initialized ());

	add (* (new Sector ("Sector 1", owner,
				* owner . root -> createSceneManager (Ogre :: ST_GENERIC))) );

	add (* (new Sector ("Sector 2", owner,
				* owner . root -> createSceneManager (Ogre :: ST_GENERIC))) );


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
	get_type_name ()
{
	return "play state";
}

void Play_State ::
	change_active_state (State <TSL> & state)
{
	assert (state . is_type <Sector> ());
	
	if (& state != get_active_state ())
	{
		Sector * old_active_sector = owner . get_active_sector ();
		assert (old_active_sector != NULL);
		Sector & new_active_sector = state . to_type <Sector> ();
	
		//	Update player position:
		old_active_sector -> move_to (Player :: getSingleton (), new_active_sector);
		Player :: getSingleton () . node = & new_active_sector . copy_node (* Player :: getSingleton () . node);
		
		State_Machine <TSL> :: change_active_state (state);
		assert (owner . get_active_sector () == & state);

		//	Update camera & scene manager:
		owner . root -> getRenderSystem () -> _getViewport () -> setCamera (& new_active_sector . get_camera ());
		owner . gui_engine -> set_scene_manager (new_active_sector . get_scene_manager ());
	}
}
