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
	Set (string name, unsigned int new_maximal_size) :
	Object (name),
	Container <T> (new_maximal_size),
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

	//	We can't use <= or >= here, because 'Container <T> :: unlimited < 0'.
	if (children -> size () == Container <T> :: maximal_size)
	{
		Engines :: Log :: log (this -> me) << additive << " could not be added. I'm full." << endl;
		return false;
	}

	pair <Next_Child_Type, bool> result = children -> insert (additive);
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
#include "engines/audio_engine.hpp"
#include "engines/game.hpp"
#include "gui.hpp"
#include "items/character.hpp"
#include "items/container_item.hpp"
#include "messages/conversation_message.hpp"
#include "model.hpp"
#include "movable_model.hpp"
#include "opinion.hpp"
#include "strategies/actions/action.hpp"
#include "strategies/behaviors/behavior.hpp"
#include "strategies/game_modes/game_mode.hpp"
#include "tile.hpp"

template class Set <GUI>;
template class Set <Items :: Character>;
template class Set <Items :: Container_Item <Items :: Container_Item <Items :: Item> > >;
template class Set <Items :: Container_Item <Items :: Item> >;
template class Set <Items :: Item>;
template class Set <Messages :: Message <Items :: Character> >;
template class Set <Model>;
template class Set <Object>;
template class Set <Observer <Messages :: Message <Items :: Character> > >;
template class Set <Observer <Messages :: Message <Object> > >;
template class Set <Observer <Object> >;
template class Set <Opinion>;
#if RADAKAN_AUDIO_MODE == RADAKAN_AUDIERE_MODE
	template class Set <Sound_Sample>;
#endif
template class Set <Strategies :: Actions :: Action>;
template class Set <Strategies :: Behaviors :: Behavior>;
template class Set <Strategies :: Game_Modes :: Game_Mode>;
template class Set <Tile>;
