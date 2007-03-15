#include "data_state_machine.hpp"

using namespace std;
using namespace tsl;

//  constructor
template <class T> Data_State_Machine <T> ::
	Data_State_Machine () :
	Object ("The name doesn't matter as this class is an abstact class.")
{
	Object :: log (Object :: debugging) << get_class_name () << " ()" << endl;
	assert (State_Machine <T> :: is_initialized ());
	assert (Disjoint_Set <T> :: is_initialized ());

	assert (Data_State_Machine <T> :: is_initialized ());
}

//  destructor
template <class T> Data_State_Machine <T> ::
	~Data_State_Machine ()
{
	Object :: log (Object :: debugging) << "~" << get_class_name () << " ()" << endl;
	assert (Data_State_Machine <T> :: is_initialized ());

	State_Machine <T> :: unset_active_state ();
}

//	virtual
template <class T> bool Data_State_Machine <T> ::
	is_initialized ()
	const
{
//	Object :: log (Object :: debugging) << get_class_name () << " :: is_initialized ()" << endl;
	assert (Object :: warn <Data_State_Machine <T> >
								(State_Machine <T> :: is_initialized ()));
	assert (Object :: warn <Data_State_Machine <T> >
								(Disjoint_Set <T> :: is_initialized ()));

	return true;
}

//	static
template <class T> string Data_State_Machine <T> ::
	get_class_name ()
{
	return "Data_State_Machine <" + T :: get_class_name () + ">";
}

//	virtual
template <class T> bool Data_State_Machine <T> ::
	add (T & t)
{
	Object :: log (Object :: debugging) << get_class_name () << " :: add (" << t << ")" << endl;
	assert (is_initialized ());
	Object :: log (Object :: debugging) << "add (" << t << ") AA" << endl;
	assert (t . is_initialized ());

	Object :: log (Object :: debugging) << "add (" << t << ") A" << endl;
	bool check = Disjoint_Set <T> :: add (t);
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
	move (T & t, Disjoint_Set <T> & destination)
{
	Object :: log (Object :: debugging) << "move (" << t << ", " << destination << ")" << endl;
	assert (Data_State_Machine <T> :: is_initialized ());
	assert (destination . Disjoint_Set <T> :: is_initialized ());
	assert (contains (t));

	return false;
}

//	to avert linking errors:
#include "gui.hpp"
#include "tile.hpp"

template class Data_State_Machine <GUI>;
template class Data_State_Machine <Tile>;
