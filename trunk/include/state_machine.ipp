#ifndef STATE_MACHINE_IPP
#define STATE_MACHINE_IPP

//virtual
template <typename T> void State_Machine ::
		change_active_state ()
{
	assert (is_initialized ());

	active_child_state = get_child <T> ();

	assert (active_child_state != NULL);
}

template <typename T> void State_Machine ::
	add ()
{
	assert (is_initialized ());
	assert (get_child <T> () == NULL);

	add (new T (owner, this));
}

#endif	//	STATE_MACHINE_IPP
