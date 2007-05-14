#include "algorithm.hpp"

using namespace std;
using namespace TSL;

//	static
template <class T> const string Algorithm <T> ::
	class_name ("Algorithm <" + T :: class_name + ">");

//  constructor
template <class T> Algorithm <T> ::
	Algorithm (Algorithm <T> * new_parent) :
	Object ("The name doesn't matter as this class is an abstact class."),
	parent (new_parent)
{
	log (debugging) << class_name << " ()" << endl;
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
	log (debugging) << "~" << class_name << " ()" << endl;
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
		parental_result_state = & parent -> full_transit (owner);
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

#include "game.hpp"
#include "npc.hpp"

template class Algorithm <Game>;
template class Algorithm <NPC>;
