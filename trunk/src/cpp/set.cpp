#include "log.hpp"
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
	Set (string name, int new_maximal_size) :
	Object (name),
	Container <T> (name, new_maximal_size),
	children (new set <Reference <T> >)
{
	Engines :: Log :: trace (this -> me, Set <T> :: get_class_name (), "", name, to_string (new_maximal_size));

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

	for (T_Iterator child = children -> begin (); child != children -> end (); child = children -> begin ())
	{
		Engines :: Log :: log (this -> me) << "Dropping child '" << child -> get_name () << "'..." << endl;
		drop (* child);
	}
	Engines :: Log :: log (this -> me) << "All children were dropped." << endl;

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
//	Engines :: Log :: trace (this -> me, Set <T> :: get_class_name (), "contains", contained . get_name ());
	assert (Set <T> :: is_initialized ());
	assert (contained . is_initialized ());
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
	assert (additive . is_initialized ());
	assert (additive . points_to_object ());
	assert (additive -> is_initialized ());

	if (contains (additive))
	{
		return false;
	}

	//	We can't use <= or >= here, because 'Container <T> :: unlimited < 0'.
	if (children -> size () == Container <T> :: maximal_size)
	{
		Engines :: Log :: log (this -> me) << additive -> name << " could not be added. I'm full." << endl;
		return false;
	}

	pair <T_Iterator, bool> result = children -> insert (additive);
	assert (result . second);
	const_cast <Reference <T> &> (* result . first) . set_parent (* this);
	
	assert (Set <T> :: is_initialized ());
	return true;
}

//	virtual
template <class T> void Set <T> ::
	drop (Reference <T> dropped)
{
	Engines :: Log :: trace (this -> me, Set <T> :: get_class_name (), "drop", dropped . get_name ());
	assert (Set <T> :: is_initialized ());
	assert ((! Container <T> :: is_sealed ()) || Object :: is_destructing ());
	assert (dropped -> is_initialized ());
	assert (contains (dropped));

	unsigned int check = children -> erase (dropped);
	assert (check == 1);
}

template <class T> Reference <T> Set <T> ::
	get_child ()
	const
{
//	Engines :: Log :: trace (this -> me, Set <T> :: get_class_name (), get_child);
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
//	Engines :: Log :: trace (this -> me, Set <T> :: get_class_name (), get_another_child);
	assert (Set <T> :: is_initialized ());

	if (next_child == children -> end ())
	{
		return Reference <T> ();
	}
	return * (next_child ++);
}

//	to avert linking errors:
#include "audio_engine.hpp"
#include "conversation_message.hpp"
#include "strategy.hpp"
#include "gui.hpp"
#include "container_item.hpp"
#include "npc.hpp"
#include "model.hpp"
#include "movable_model.hpp"
#include "tile.hpp"
#include "thought.hpp"
#include "play_state.hpp"

template class Set <GUI>;
template class Set <Items :: Character>;
template class Set <Items :: Container_Item <Items :: Container_Item <Items :: Item> > >;
template class Set <Items :: Container_Item <Items :: Item> >;
template class Set <Items :: Item>;
template class Set <Items :: NPC>;
template class Set <Messages :: Conversation_Message>;
template class Set <Model>;
template class Set <Object>;
template class Set <Observer <Engines :: Log> >;
template class Set <Observer <GUI> >;
template class Set <Observer <Items :: Character> >;
template class Set <Observer <Strategies :: Play_State> >;
template class Set <Sound_Sample>;
template class Set <Strategies :: Strategy>;
template class Set <Tile>;
template class Set <Thought>;

