#ifndef STATE_MACHINE_IPP
#define STATE_MACHINE_IPP

using namespace std;
using namespace sl;

//virtual
template <typename T> void State_Machine ::
		change_active_state ()
{
	trace () << "change_active_state <" << T :: get_type_name () << "> ()" << endl;
	assert (Object :: is_initialized ());

	active_child_state = get_child <T> ();

	if (active_child_state == NULL)
	{
		add <T> ();
		active_child_state = get_child <T> ();
	}

	assert (active_child_state != NULL);
}

template <typename T> void State_Machine ::
	add ()
{
	trace () << "add <" << T :: get_type_name () << "> ()" << endl;
	assert (is_initialized ());
	
	assert (get_child <T> () == NULL);

	//add (* new T (owner));
	//T *state = new T (owner);
	add (* (new T (owner)) );
}

#endif	//	STATE_MACHINE_IPP
