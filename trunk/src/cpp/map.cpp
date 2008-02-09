#include "engines/log.hpp"
#include "map.hpp"

using namespace std;
using namespace Radakan;

//	static
template <typename T, class U> string Map <T, U> ::
	get_class_name ()
{
	return "Map <@T@, " + U :: get_class_name () + ">";
}

//  constructor
template <typename T, class U> Map <T, U> ::
	Map (string name, bool new_weak_children) :
	Object (name),
	Container <Pair <T, U> > (Container <Pair <T, U> > :: unlimited (), new_weak_children),
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
	assert ((Container <Pair <T, U> > :: is_initialized ()));
	
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
	
	pair <Next_Child_Type, bool> result = children -> insert
		(pair <T, Reference <U> > (additive -> first, additive -> get_child ()));
	Engines :: Log :: log (this -> me) << "map insert: "
		<< result . first -> second . get_name () << endl;
	assert (result . second);
	/*	TODO

	const_cast <Reference <U> &> (result . first -> second) . set_parent (* this);
	if (Container <Pair <T, U> > :: weak_children)
	{
		const_cast <Reference <U> &> (result . first -> second) . weaken ();
	}*/
	
	return true;
}

//	virtual
template <typename T, class U> void Map <T, U> ::
	drop (Reference <Pair <T, U> > dropped)
{
	Engines :: Log :: trace (this -> me, Map <T, U> :: get_class_name (), "drop", dropped . get_name ());
	assert (Map :: is_initialized ());

	//	GCC needs the additional '(...)' to understand this:
	assert ((! Container <Pair <T, U> > :: is_sealed ()));
	
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
			(Pair <T, U> :: create (next_child -> first, next_child -> second));
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

#if RADAKAN_AUDIO_MODE == RADAKAN_AUDIERE_MODE
	#include "engines/audio_engine.hpp"
#endif
#include "messages/communications/communication.hpp"
#include "skill.hpp"
#include "tile.hpp"

template class Map <Mathematics :: Vector_3D, Tile>;
template class Map <string, Messages :: Communications :: Communication>;
template class Map <string, Skill>;
#if RADAKAN_AUDIO_MODE == RADAKAN_AUDIERE_MODE
	template class Map <string, Sound_Sample>;
#endif
