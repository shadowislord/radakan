#include "data_state_machine.hpp"

using namespace std;
using namespace TSL;

//	static
template <class T> const string Data_State_Machine <T> ::
	class_name ("Data_State_Machine <" + T :: class_name + ">");

//  constructor
template <class T> Data_State_Machine <T> ::
	Data_State_Machine () :
	Object ("The name doesn't matter as this class is an abstact class.")
{
	Object :: log (Object :: debugging) << class_name << " ()" << endl;
	assert (State_Machine <T> :: is_initialized ());
	assert (Set <T> :: is_initialized ());

	assert (Data_State_Machine <T> :: is_initialized ());
}

//  destructor
template <class T> Data_State_Machine <T> ::
	~Data_State_Machine ()
{
	Object :: log (Object :: debugging) << "~" << class_name << " ()" << endl;
	assert (Data_State_Machine <T> :: is_initialized ());

	State_Machine <T> :: unset_active_state ();
}

//	virtual
template <class T> bool Data_State_Machine <T> ::
	is_initialized ()
	const
{
//	Object :: log (Object :: debugging) << class_name << " :: is_initialized ()" << endl;
	assert (State_Machine <T> :: is_initialized ());
	assert (Set <T> :: is_initialized ());

	return true;
}

//	virtual
template <class T> bool Data_State_Machine <T> ::
	add (T & t)
{
	Object :: log (Object :: debugging) << class_name << " :: add (" << t << ")" << endl;
	assert (is_initialized ());
	Object :: log (Object :: debugging) << "add (" << t << ") AA" << endl;
	assert (t . is_initialized ());

	Object :: log (Object :: debugging) << "add (" << t << ") A" << endl;
	bool check = Set <T> :: add (t);
	assert (check);

	Object :: log (Object :: debugging) << "add (" << t << ") B" << endl;
	
	if (! State_Machine <T> :: has_active_state ())
	{
		State_Machine <T> :: set_active_state (t);
	}

	Object :: log (Object :: debugging) << "add (" << t << ") C" << endl;
	
	return true;
}

//	virtual
template <class T> void Data_State_Machine <T> ::
	set_active_state (T & t)
{
	Object :: log (Object :: debugging) << "set_active_state (" << t << ")" << endl;
	assert (Data_State_Machine <T> :: is_initialized ());
	assert (contains (t));

	State_Machine <T> :: set_active_state (t);
}

//	virtual
template <class T> bool Data_State_Machine <T> ::
	move (T & t, Set <T> & destination)
{
	Object :: log (Object :: debugging) << "move (" << t << ", " << destination << ")" << endl;
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
