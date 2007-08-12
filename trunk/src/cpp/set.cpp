#include "log.hpp"
#include "set.hpp"

using namespace std;
using namespace TSL;

//	static
template <class T> const string Set <T> ::
	get_class_name ()
{
	return "Set <" + T :: get_class_name () + ">";
}

//	This could have been any number (strictly) below 0.
template <class T> const int Set <T> ::
	unlimited = - 10;

//  constructor
template <class T> Set <T> ::
	Set (string name, int new_maximal_size) :
	Object (name),
	maximal_size (new_maximal_size),
	sealed (false)
{
	Engines :: Log :: trace (me, Set <T> :: get_class_name (), "", name, to_string (new_maximal_size));
	assert ((new_maximal_size == unlimited) || (0 <= new_maximal_size));

	//	Do nothing.

	assert (Set <T> :: is_initialized ());
}

//  destructor
template <class T> Set <T> ::
	~Set ()
{
	Engines :: Log :: trace (me, Set <T> :: get_class_name (), "~");
	assert (Set <T> :: is_initialized ());

	prepare_for_destruction ();

	for (T * child = get_child (); child != NULL; child = get_child ())
	{
		Engines :: Log :: log (me) << "Dropping child '" << child -> name << "'..." << endl;
		drop (* child);
	}
	Engines :: Log :: log (me) << "All children were dropped." << endl;

	assert (children . empty ());
}

//	virtual
template <class T> bool Set <T> ::
	is_initialized ()
	const
{
	assert (Object :: is_initialized ());
	assert (unlimited < 0);
	assert ((maximal_size == unlimited) || (0 <= maximal_size));
	assert (children . size () <= maximal_size);

	/*
	//	Don't use the 'get_child' & 'get_another_child' methods here,
	//	as they both require me to be initialized.
	for (T_iterator i = children . begin (); i != children . end (); i ++)
	{
		assert ((* i) -> T :: is_initialized ());
	}
	*/

	return true;
}

template <class T> bool Set <T> ::
	is_empty () const
{
	assert (Set <T> :: is_initialized ());

	return children . empty ();
}

//	virtual
template <class T> bool Set <T> ::
	add (T & t)
{
	Engines :: Log :: trace (me, Set <T> :: get_class_name (), "add", t . name);
	assert (Set <T> :: is_initialized ());
	assert (! sealed);

	if (contains (t))
	{
		return false;
	}

	if ((maximal_size != unlimited) && (children . size () == maximal_size))
	{
		Engines :: Log :: log (me) << t . name << " could not be added. I'm full." << endl;
		return false;
	}

	t . remember (me);
	//	Engines :: Log :: log (me) << t << " was added." << endl;

	//	'second' means we're interested in if it worked or not.
	//	'first' would give a iterator to the item.
	bool check = children . insert (& t) . second;
	assert (check);
	
	assert (Set <T> :: is_initialized ());
	return true;
}

//	virtual
template <class T> bool Set <T> ::
	contains (T & t)
	const
{
//	Engines :: Log :: trace (me, Set <T> :: get_class_name (), "contains", name);
	assert (Set <T> :: is_initialized ());

	return (children . find (& t) != children . end ());
}

//	virtual
template <class T> bool Set <T> ::
	move (T & t, Set <T> & destination)
{
	Engines :: Log :: trace (me, Set <T> :: get_class_name (), "move", t . name, destination . name);
	assert (Set <T> :: is_initialized ());
	assert (t . is_initialized ());
	assert (destination . is_initialized ());
	assert (! sealed);
	assert (contains (t));

	//	'true' as second parameter forces 't' to not self-destruct.
	drop (t, true);
	
	if (destination . add (t))
	{
		return true;
	}
	bool check = add (t);
	assert (check);
	
	return false;
}

//	virtual
template <class T> void Set <T> ::
	drop (Object & t, bool stay)
{
	Engines :: Log :: trace (this -> me, Set <T> :: get_class_name (), "drop", t . name, bool_to_string (stay));
	assert (Set <T> :: is_initialized ());
	assert ((! sealed) || is_destructing ());
	T & dropped = t . to_type <T> ();
	assert (contains (dropped));

	dropped . forget (me, stay);
	children . erase (& dropped);
}

template <class T> T * Set <T> ::
	get_child ()
	const
{
//	Engines :: Log :: trace (me, Set <T> :: get_class_name (), get_child);
	assert (Set <T> :: is_initialized ());

	if (children . empty ())
	{
		return NULL;
	}
	next_child = children . begin ();
	return * (next_child ++);
}

template <class T> T * Set <T> ::
	get_another_child ()
	const
{
//	Engines :: Log :: trace (me, Set <T> :: get_class_name (), get_another_child);
	assert (Set <T> :: is_initialized ());

	if (next_child == children . end ())
	{
		return NULL;
	}
	return * (next_child ++);
}

template <class T> void Set <T> ::
	seal ()
{
	sealed = true;
}

template <class T> bool Set <T> ::
	is_sealed () const
{
	return sealed;
}

//	to avert linking errors:
#include "audio_engine.hpp"
#include "strategy.hpp"
#include "gui.hpp"
#include "tile.hpp"
#include "play_state.hpp"

template class Set <Strategies :: Strategy>;
template class Set <GUI>;
template class Set <Items :: Character>;
template class Set <Items :: Item>;
template class Set <Items :: NPC>;
template class Set <Model>;
template class Set <Object>;
template class Set <Observer <Strategies :: Play_State> >;
template class Set <Observer <Engines :: Log> >;
template class Set <Observer <GUI> >;
template class Set <Observer <Items :: Character> >;
template class Set <Sound_Sample>;
template class Set <Tile>;
