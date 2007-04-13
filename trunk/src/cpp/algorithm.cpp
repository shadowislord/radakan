#include "algorithm.hpp"

using namespace std;
using namespace tsl;

//  constructor
template <class T> Algorithm <T> ::
	Algorithm (Algorithm <T> * new_parent) :
	Object ("The name doesn't matter as this class is an abstact class."),
	parent (new_parent)
{
	log (debugging) << get_class_name () << " ()" << endl;
	assert (Object :: is_initialized ());
	if (parent != NULL)
	{
		assert (parent -> is_initialized ());
	}
	
	assert (Algorithm <T> :: is_initialized ());
}

//  destructor
template <class T> Algorithm <T> ::
	~Algorithm ()
{
	log (debugging) << "~" << get_class_name () << " ()" << endl;
	assert (Algorithm <T> :: is_initialized ());
}

//	virtual
template <class T> bool Algorithm <T> ::
	is_initialized ()
	const
{
	assert (Object :: is_initialized ());
	if (parent != NULL)
	{
		assert (parent -> is_initialized ());
	}

	return true;
}

//	static
template <class T> string Algorithm <T> ::
	get_class_name ()
{
	return "Algorithm <" + T :: get_class_name () + ">";
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

	Algorithm <T> * parental_result_state = this;
	
	if (parent != NULL)
	{
		parental_result_state = & parent -> transit (owner);
	}
	
	if (parental_result_state != this)
	{
		return * parental_result_state;
	}

	return transit (owner);
}

//	virtual
template <class T> void Algorithm <T> ::
	enter (T & owner)
{
	assert (is_initialized ());
}

//	virtual
template <class T> void Algorithm <T> ::
	exit (T & owner)
{
	assert (is_initialized ());
}

#include "npc.hpp"
#include "tsl.hpp"

template class Algorithm <NPC>;
template class Algorithm <TSL>;
