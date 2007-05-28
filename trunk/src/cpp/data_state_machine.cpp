#include "data_state_machine.hpp"
#include "log.hpp"

using namespace std;
using namespace TSL;
using namespace TSL :: State_Machines;

//	static
template <class T> const string Data_State_Machine <T> ::
	get_class_name ()
{
	return "Data_State_Machine <" + T :: get_class_name () + ">";
}

//  constructor
template <class T> Data_State_Machine <T> ::
	Data_State_Machine () :
	Object ("The nathis -> me doesn't matter as this class is an abstact class.")
{
	Engines :: Log :: trace (this -> me, Data_State_Machine <T> :: get_class_name (), "");

	//	Do nothing.

	assert (Data_State_Machine <T> :: is_initialized ());
}

//  destructor
template <class T> Data_State_Machine <T> ::
	~Data_State_Machine ()
{
	Engines :: Log :: trace (this -> me, Data_State_Machine <T> :: get_class_name (), "~");
	assert (Data_State_Machine <T> :: is_initialized ());

	State_Machine <T> :: unset_active_state ();
}

//	virtual
template <class T> bool Data_State_Machine <T> ::
	is_initialized ()
	const
{
	//	Engines :: Log :: trace (this -> me, Data_State_Machine <T> :: get_class_name (), "is_initialized");
	assert (State_Machine <T> :: is_initialized ());
	assert (Set <T> :: is_initialized ());

	return true;
}

//	virtual
template <class T> bool Data_State_Machine <T> ::
	add (T & t)
{
	Engines :: Log :: trace (this -> me, Data_State_Machine <T> :: get_class_name (), "add", t);
	assert (is_initialized ());
	assert (t . is_initialized ());

	bool check = Location <T> :: add (t);
	assert (check);

	if (! State_Machine <T> :: has_active_state ())
	{
		State_Machine <T> :: set_active_state (t);
	}

	return true;
}

//	virtual
template <class T> void Data_State_Machine <T> ::
	set_active_state (T & t)
{
	Engines :: Log :: trace (this -> me, Data_State_Machine <T> :: get_class_name (), "set_active_state", t);
	assert (Data_State_Machine <T> :: is_initialized ());
	assert (contains (t));

	State_Machine <T> :: set_active_state (t);
}

//	virtual
template <class T> bool Data_State_Machine <T> ::
	move (T & t, Set <T> & destination)
{
	Engines :: Log :: trace (this -> me, Data_State_Machine <T> :: get_class_name (), "move", t, destination);
	assert (Data_State_Machine <T> :: is_initialized ());
	assert (destination . Set <T> :: is_initialized ());
	assert (contains (t));

	return false;
}

//	to avert linking errors:
#include "gui.hpp"
#include "tile.hpp"

template class Data_State_Machine <GUI>;
template class Data_State_Machine <Tile>;
