#include "algorithm.hpp"
#include "log.hpp"

using namespace std;
using namespace TSL;

//	static
template <class T> const string Algorithm <T> ::
	get_class_name ()
{
	return "Algorithm <" + T :: get_class_name () + ">";
}

//  constructor
template <class T> Algorithm <T> ::
	Algorithm () :
	Object ("The name doesn't matter as this class is an abstact class."),
	parent (* this)
{
	Log :: trace <Algorithm <T> > (me, "");
	assert (Object :: is_initialized ());

	assert (Algorithm <T> :: is_initialized ());
}

//  constructor
template <class T> Algorithm <T> ::
	Algorithm (Algorithm <T> & new_parent) :
	Object ("The name doesn't matter as this class is an abstact class."),
	parent (new_parent)
{
	Log :: trace <Algorithm <T> > (me, "", new_parent);
	assert (Object :: is_initialized ());
	assert (new_parent . is_initialized ());
	
	assert (Algorithm <T> :: is_initialized ());
}

//  destructor
template <class T> Algorithm <T> ::
	~Algorithm ()
{
	Log :: trace <Algorithm <T> > (me, "~");
	assert (Algorithm <T> :: is_initialized ());
}

//	virtual
template <class T> bool Algorithm <T> ::
	is_initialized ()
	const
{
	assert (Object :: is_initialized ());
	if (parent != * this)
	{
		assert (parent . is_initialized ());
	}

	return true;
}

//	virtual
template <class T> Algorithm <T> & Algorithm <T> ::
	transit (T & owner)
{
	assert (is_initialized ());

	return owner . get_active_state ();
}

template <class T> Algorithm <T> & Algorithm <T> ::
	full_transit (T & owner)
{
	assert (is_initialized ());

	if (parent != * this)
	{
		Algorithm <T> & parental_result_state = parent . full_transit (owner);
	
		if (parental_result_state != * this)
		{
			return parental_result_state;
		}
	}

	return transit (owner);
}

//	virtual
template <class T> void Algorithm <T> ::
	enter (T & owner)
{
	Log :: trace <Algorithm <T> > (me, "enter", owner);
	assert (is_initialized ());
}

//	virtual
template <class T> void Algorithm <T> ::
	exit (T & owner)
{
	Log :: trace <Algorithm <T> > (me, "exit", owner);
	assert (is_initialized ());
}

#include "game.hpp"
#include "npc.hpp"

template class Algorithm <Game>;
template class Algorithm <NPC>;
