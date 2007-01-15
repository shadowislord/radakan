#include "set.hpp"

using namespace std;
using namespace tsl;

//  constructor
template <typename T> Set <T> ::
	Set (string new_name) :
	Object (new_name)
{
	trace () << "Set <" << T :: get_type_name () << "> (" << new_name << ")" << endl;
	assert (Object :: is_initialized ());

	assert (Set <T> :: is_initialized ());
}

//  destructor
template <typename T> Set <T> ::
	~Set ()
{
	trace () << "~Set <" << T :: get_type_name () << "> ()" << endl;
	assert (Set <T> :: is_initialized ());

	for (typename set<T *> :: const_iterator i = children . begin (); i != children . end (); i ++)
	{
		trace () << "deleting " << to_string (* i) << "..." << endl;
		delete (* i);
	}
}

//	virtual
template <typename T> bool Set <T> ::
	is_initialized ()
	const
{
	for (typename set <T *> :: const_iterator i = children . begin ();
							i != children . end (); i ++)
	{
		assert (warn <Set <T> > ((* i) -> T :: is_initialized ()));
	}
	assert (warn <Set <T> > (Object :: is_initialized ()));
	return true;
}

//	static
template <typename T> string Set <T> ::
	get_type_name ()
{
	return "Set <" + T :: get_type_name () + ">";
}

//	virtual
template <typename T> bool Set <T> ::
	add (T & t)
{
	trace () << "add <" << T :: get_type_name () << "> (" << t << ")" << endl;
	assert (Set <T> :: is_initialized ());
	assert (! t . has_parent ());

	t . put_in (* this);
	debug () << t << " was added." << endl;

	//	'second' means we're interested in if it worked or not.
	//	'first' would give a iterator to the item.
	bool result = children . insert (& t) . second;
	assert (Set <T> :: is_initialized ());
	return result;
}

//	virtual
template <typename T> bool Set <T> ::
	contains (T & t, bool recursive)
	const
{
	trace () << "contains <" << T :: get_type_name () << "> (" << t << ", " << bool_to_string (recursive) + ")" << endl;
	assert (Set <T> :: is_initialized ());

	bool result = (children . find (& t) != children . end ());

	if ((! result) && recursive)
	{
		for (T * i = get_child (); i != NULL; i = get_another_child ())
		{
			if (i -> is_type <Set <T> > ())
			{
				if (i -> to_type <Set <T> > () . contains (t, recursive))
				{
					return true;
				}
			}
		}
	}

	if (result)
	{
		trace () << "contains " << t << endl;
	}
	else
	{
		trace () <<"doesn't contain " << t << endl;
	}
	return result;
}

//	virtual
template <typename T> bool Set <T> ::
	move_to (T & t, Set <T> & other_set)
{
	trace () << "move_to <" << T :: get_type_name () << "> (" << t << ", " << other_set << ")" << endl;
	assert (Set <T> :: is_initialized ());
	assert (t . is_initialized ());
	assert (other_set . is_initialized ());
	assert (contains (t, false));

	t . remove_from (* this);
	if (other_set . add (t))
	{
		children . erase (& t);
		return true;
	}
	t . put_in (* this);
	return false;
}

template <typename T> T * Set <T> ::
	get_child (string name, bool recursive)
	const
{
	trace () << "get_child <" << T :: get_type_name () << "> (" << name << ")" << endl;
	assert (Set <T> :: is_initialized ());

	for (T * i = get_child (); i != NULL; i = get_another_child ())
	{
		if (i -> compare (name) == 0)
		{
			return i;
		}
		else if (recursive)
		{
			if (i -> is_type <Set <T> > ())
			{
				T * result = i -> to_type <Set <T> > () . get_child (name, true);
				if (result != NULL)
				{
					return result;
				}
			}
		}
	}
	return NULL;
}

template <typename T> template <typename U> U * Set <T> ::
	get_typed_child ()
	const
{
	trace () << "get_typed_child <" << T :: get_type_name () << ", " << U :: get_type_name () << "> ()" << endl;
	assert (Set <T> :: is_initialized ());

	for (T * i = get_child (); i != NULL; i = get_another_child ())
	{
		if (i -> is_type <U> ())
		{
			return & i -> to_type <U> ();
		}
	}
	return NULL;
}

template <typename T> template <typename U> U * Set <T> ::
	get_typed_child (string name)
	const
{
	trace () << "get_typed_child <" << T :: get_type_name () << "> (" << name << ")" << endl;
	assert (Set <T> :: is_initialized ());

	for (T * i = get_child (); i != NULL; i = get_another_child ())
	{
		if ( i -> is_type <U> () && (i -> compare (name) == 0))
		{
			return & i -> to_type <U> ();
		}
	}
	return NULL;
}

template <typename T> T * Set <T> ::
	get_child ()
	const
{
	trace () << "get_child <" << T :: get_type_name () << "> ()" << endl;
	assert (Set <T> :: is_initialized ());

	if (children . empty ())
	{
		return NULL;
	}
	next_child = children . begin ();
	return * (next_child ++);
}

template <typename T> T * Set <T> ::
	get_another_child ()
	const
{
	trace () << "get_another_child <" << T :: get_type_name () << "> ()" << endl;
	assert (Set <T> :: is_initialized ());

	if (next_child == children . end ())
	{
		return NULL;
	}
	return * (next_child ++);
}

//	to avert linking errors:
#include "play_state.hpp"
#include "pause_state.hpp"

template class Set <Entity>;
template class Set <Sector>;
//	template class Set <Sound>;
template class Set <State <NPC> >;
template class Set <State <TSL> >;

//	using that method is way to slow to find npcs, use get_npcs () instead
//	template NPC * Set <Entity> :: get_typed_child <NPC> () const;

template Dead_State * Set <State <NPC> > :: get_typed_child <Dead_State> () const;
template Fight_State * Set <State <NPC> > :: get_typed_child <Fight_State> () const;
template Peace_State * Set <State <NPC> > :: get_typed_child <Peace_State> () const;
template Play_State * Set <State <TSL> > :: get_typed_child <Play_State> () const;
template Pause_State * Set <State <TSL> > :: get_typed_child <Pause_State> () const;
template Sector * Set <Sector> :: get_typed_child <Sector> () const;

template Sector * Set <Sector> :: get_typed_child <Sector> (string name) const;
