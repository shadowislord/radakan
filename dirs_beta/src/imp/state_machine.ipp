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
}

template <typename T> void State_Machine ::
	add ()
{
	trace () << "add <" << T :: get_type_name () << "> ()" << endl;
	assert (is_initialized ());
	
	assert (get_child <T> () == NULL);

	add (* new T (owner));
}

#endif	//	STATE_MACHINE_IPP
