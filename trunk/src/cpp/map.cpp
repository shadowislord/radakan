#include "engines/log.hpp"
#include "map.hpp"

using namespace std;
using namespace Radakan;

//	static
template <typename T, class U> string Map <T, U> ::
	get_class_name ()
{
	return "Map <~T~, " + U :: get_class_name () + ">";
}

//  constructor
template <typename T, class U> Map <T, U> ::
	Map (string name) :
	Object (name),
	children (new map <T, Reference <U> >)
{
	Engines :: Log :: trace (this -> me, Map <T, U> :: get_class_name ());

	//	Do nothing.

	assert (Map :: is_initialized ());
}

//  destructor
template <typename T, class U> Map <T, U> ::
	~Map ()
{
	Engines :: Log :: trace (this -> me, Map <T, U> :: get_class_name (), "~");
	assert (Map :: is_initialized ());

	Object :: prepare_for_destruction ();

	this -> clear ();

	assert (children -> empty ());
}

//	virtual
template <typename T, class U> bool Map <T, U> ::
	is_initialized ()
	const
{
	//	TODO Make the next line work.
	//	assert (Container <Pair <T, U> > :: is_initialized ());
	
	return true;
}

//	virtual
template <typename T, class U> bool Map <T, U> ::
	is_empty () const
{
	assert (Map :: is_initialized ());

	return children -> empty ();
}

//	virtual
template <typename T, class U> bool Map <T, U> ::
	contains (const Reference <Pair <T, U> > & contained)
	const
{
//	Engines :: Log :: trace (me, Map <T, U> :: get_class_name (), "contains", contained . get_name ());
	assert (Map :: is_initialized ());
	assert (contained . points_to_object ());
	assert (contained -> is_initialized ());

	return 0 < children -> count (contained -> first);
}

//	virtual
template <typename T, class U> bool Map <T, U> ::
	add (Reference <Pair <T, U> > additive)
{
	Engines :: Log :: trace
		(this -> me, Map <T, U> :: get_class_name (), "add", additive . get_name ());
	
	children -> insert (pair <T, Reference <U> > (additive -> first, additive -> get_child ()));
	
	return true;
}

//	virtual
template <typename T, class U> void Map <T, U> ::
	drop (Reference <Pair <T, U> > dropped)
{
	Engines :: Log :: trace (this -> me, Map <T, U> :: get_class_name (), "drop", dropped . get_name ());
	assert (Map :: is_initialized ());
	assert ((! Container <Pair <T, U> > :: is_sealed ()) || Object :: is_destructing ());
	assert (dropped -> is_initialized ());
	assert (contains (dropped));

	unsigned int check = children -> erase (dropped -> first);
	assert (check == 1);
}

template <typename T, class U> Reference <Pair <T, U> > Map <T, U> ::
	get_child ()
	const
{
//	Engines :: Log :: trace (me, Map <T, U> :: get_class_name (), get_child);
	assert (Map :: is_initialized ());

	next_child = children -> begin ();

	return get_another_child ();
}

template <typename T, class U> Reference <Pair <T, U> > Map <T, U> ::
	get_another_child ()
	const
{
//	Engines :: Log :: trace (me, Map <T, U> :: get_class_name (), get_another_child);
	assert (Map :: is_initialized ());

	if (next_child == children -> end ())
	{
		return Reference <Pair <T, U> > ();
	}
	else
	{
		Reference <Pair <T, U> > result
			(new Pair <T, U> (next_child -> first, next_child -> second));
		next_child ++;
		return result;
	}
}

template <typename T, class U> Reference <U> Map <T, U> ::
	look_up (const T & t) const
{
	Next_Child_Type found (children -> find (t));

	if (found == children -> end ())
	{
		return Reference <U> ();
	}
	else
	{
		return found -> second;
	}
}

#if RADAKAN_GUI_MODE == RADAKAN_CEGUI_MODE
	#include <elements/CEGUIListboxItem.h>
#endif
#include "items/character.hpp"
#include "messages/message.hpp"
#include "skill.hpp"
#include "tile.hpp"

#if RADAKAN_GUI_MODE == RADAKAN_CEGUI_MODE
	template class Map <CEGUI :: ListboxItem *, Messages :: Message <Items :: Character> >;
#endif
template class Map <pair <int, int>, Tile>;
template class Map <string, Skill>;
