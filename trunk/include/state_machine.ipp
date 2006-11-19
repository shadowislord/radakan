#ifndef STATE_MACHINE_IPP
#define STATE_MACHINE_IPP

using namespace std;
using namespace sl;

//virtual
template <typename T> void State_Machine ::
		change_active_state ()
{
	assert (Object :: is_initialized ("change_active_state ()"));

	active_child_state = get_child <T> ();

	assert (active_child_state != NULL);
}

template <typename T> void State_Machine ::
	add ()
{
	assert (Object :: is_initialized ("add ()"));
	
	assert (get_child <T> () == NULL);

	add (new T (owner, this));
}

#endif	//	STATE_MACHINE_IPP
