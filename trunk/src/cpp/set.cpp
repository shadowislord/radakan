#include "engines/log.hpp"
#include "set.hpp"

using namespace std;
using namespace Radakan;

//	static
template <class T> string Set <T> ::
	get_class_name ()
{
	return "Set <" + T :: get_class_name () + ">";
}

//  constructor
template <class T> Set <T> ::
	Set (string new_name, unsigned int new_maximal_size, bool new_weak_children) :
	Object (new_name),
	Container <T> (new_maximal_size, new_weak_children),
	children (new set <Reference <T> >)
{
	Engines :: Log :: trace (this -> me, Set <T> :: get_class_name (), "", new_name, to_string (new_maximal_size), bool_to_string (new_weak_children));

	//	Do nothing.

	assert (Set <T> :: is_initialized ());
}

//  destructor
template <class T> Set <T> ::
	~Set ()
{
	Engines :: Log :: trace (this -> me, Set <T> :: get_class_name (), "~");
	assert (Set <T> :: is_initialized ());

	Object :: prepare_for_destruction ();

	this -> clear ();

	assert (children -> empty ());
}

//	virtual
template <class T> bool Set <T> ::
	is_initialized ()
	const
{
	assert (Container <T> :: is_initialized ());
	assert (children -> size () <= Container <T> :: maximal_size);
	
	return true;
}

//	virtual
template <class T> bool Set <T> ::
	is_empty () const
{
	assert (Set <T> :: is_initialized ());

	return children -> empty ();
}

//	virtual
template <class T> bool Set <T> ::
	contains (const Reference <T> & contained)
	const
{
//	Engines :: Log :: trace (me, Set <T> :: get_class_name (), "contains", contained . get_name ());
	assert (Set <T> :: is_initialized ());
	assert (contained . points_to_object ());
	assert (contained -> is_initialized ());

	return 0 < children -> count (contained);
}

//	virtual
template <class T> bool Set <T> ::
	add (Reference <T> additive)
{
	Engines :: Log :: trace (this -> me, Set <T> :: get_class_name (), "add", additive . get_name ());
	assert (Set <T> :: is_initialized ());
	assert (! Container <T> :: is_sealed ());
	assert (additive . points_to_object ());
	assert (additive -> is_initialized ());

	if (contains (additive))
	{
		return false;
	}

	if (children -> size () == Container <T> :: maximal_size)
	{
		Engines :: Log :: log (this -> me) << additive << " could not be added. I'm full." << endl;
		return false;
	}

	pair <Next_Child_Type, bool> result = children -> insert (additive);
	assert (result . second);
	const_cast <Reference <T> &> (* result . first) . set_parent (* this);
	if (Container <T> :: weak_children)
	{
		const_cast <Reference <T> &> (* result . first) . weaken ();
	}
	
	assert (Set <T> :: is_initialized ());
	return true;
}

//	virtual
template <class T> void Set <T> ::
	drop (Reference <T> dropped)
{
	Engines :: Log :: trace (this -> me, Set <T> :: get_class_name (), "drop", dropped . get_name ());
	assert (Set <T> :: is_initialized ());
	assert (! Container <T> :: is_sealed ());
	assert (dropped -> is_initialized ());
	assert (contains (dropped));

	unsigned int check = children -> erase (dropped);
	assert (check == 1);
}

template <class T> Reference <T> Set <T> ::
	get_child ()
	const
{
//	Engines :: Log :: trace (me, Set <T> :: get_class_name (), get_child);
	assert (Set <T> :: is_initialized ());

	if (children -> empty ())
	{
		return Reference <T> ();
	}
	next_child = children -> begin ();
	return * (next_child ++);
}

template <class T> Reference <T> Set <T> ::
	get_another_child ()
	const
{
//	Engines :: Log :: trace (me, Set <T> :: get_class_name (), get_another_child);
	assert (Set <T> :: is_initialized ());

	if (next_child == children -> end ())
	{
		return Reference <T> ();
	}
	return * (next_child ++);
}

//	to avert linking errors:
#include "items/character.hpp"
#include "items/container_item.hpp"
#include "messages/button_event.hpp"
#include "messages/communications/communication.hpp"
#include "messages/list_event.hpp"
#include "messages/list_update.hpp"
#include "messages/nothing.hpp"
#include "opinion.hpp"

//	template class Set <GUI>;
template class Set <Items :: Character>;
template class Set <Items :: Container_Item <Items :: Container_Item <Items :: Item> > >;
template class Set <Items :: Container_Item <Items :: Item> >;
template class Set <Items :: Item>;
template class Set <Messages :: Communications :: Communication>;
template class Set <Object>;
template class Set <Observer <Messages :: Button_Event> >;
template class Set <Observer <Messages :: Communications :: Communication> >;
template class Set <Observer <Messages :: List_Event> >;
template class Set <Observer <Messages :: List_Update> >;
template class Set <Observer <Messages :: Nothing> >;
template class Set <Opinion>;
