#include "algorithm.hpp"
#include "log.hpp"

using namespace std;
using namespace TSL;
using namespace TSL :: Algorithms;

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
	Engines :: Log :: trace <Algorithm <T> > (me, "");
	
	//	Do nothing.

	assert (Algorithm <T> :: is_initialized ());
}

//  constructor
template <class T> Algorithm <T> ::
	Algorithm (Algorithm <T> & new_parent) :
	Object ("The name doesn't matter as this class is an abstact class."),
	parent (new_parent)
{
	Engines :: Log :: trace <Algorithm <T> > (me, "", new_parent);
	assert (new_parent . is_initialized ());

	//	Do nothing.
	
	assert (Algorithm <T> :: is_initialized ());
}

//  destructor
template <class T> Algorithm <T> ::
	~Algorithm ()
{
	Engines :: Log :: trace <Algorithm <T> > (me, "~");
	assert (Algorithm <T> :: is_initialized ());

	//	Do nothing.
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
template <class T> void Algorithm <T> ::
	enter (T & owner)
{
	Engines :: Log :: trace <Algorithm <T> > (me, "enter", owner);
	assert (is_initialized ());
}

//	virtual
template <class T> void Algorithm <T> ::
	exit (T & owner)
{
	Engines :: Log :: trace <Algorithm <T> > (me, "exit", owner);
	assert (is_initialized ());
}

template <class T> Algorithm <T> & Algorithm <T> ::
	recursive_transit (T & owner, const Object & message)
{
	assert (is_initialized ());

	if (parent != * this)
	{
		Algorithm <T> & parental_result_state = parent . recursive_transit (owner, message);
	
		if (parental_result_state != * this)
		{
			return parental_result_state;
		}
	}

	return transit (owner, message);
}

template <class T> void Algorithm <T> ::
	recursive_enter (T & owner)
{
	assert (is_initialized ());

	if (parent != * this)
	{
		parent . recursive_enter (owner);
	}

	enter (owner);
}

template <class T> void Algorithm <T> ::
	recursive_exit (T & owner)
{
	assert (is_initialized ());

	exit (owner);
	
	if (parent != * this)
	{
		parent . recursive_exit (owner);
	}
}

#include "game.hpp"
#include "npc.hpp"

template class Algorithm <Engines :: Game>;
template class Algorithm <Items :: NPC>;
