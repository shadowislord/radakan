#ifndef SET_IPP
#define SET_IPP

#include "set.hpp"

using namespace std;
using namespace sl;

//  constructor
template <typename T> Set <T> ::
	Set (string new_name) :
	Object (new_name)
{
	trace () << "Set (" << new_name << ")" << endl;
	assert (Object :: is_initialized ());

	assert (is_initialized ());
}

//  destructor
template <typename T> Set <T> ::
	~Set ()
{
	trace () << "~Set ()" << endl;
	assert (Object :: is_initialized ());

	for (T * i = get_one_child (); i != NULL; i = get_another_child ())
	{

		debug () << "deleting " << * i << "... " << (long int) (i) << endl;
		delete i;
	}
}

//	virtual
template <typename T> bool Set <T> ::
	is_initialized ()
	const
{
	return Object :: is_initialized ();
}

//	virtual
template <typename T> bool Set <T> ::
	add (T * t)
{
	trace () << "add (" << * t << ")" << endl;
	assert (Object :: is_initialized ());
	assert (t -> is_in (NULL));

	t -> put_in (this);
	debug () << * t << " added to " << * this << endl;
	
	//	second means we're interested in if it worked or not.
	//	first would give a iterator to the item
	return children . insert (t) . second;
}

//	virtual
template <typename T> bool Set <T> ::
	contains (T * t, bool recursive)
	const
{
	trace () << "contains (" << * t << ", " << to_string (recursive) + ")" << endl;
	assert (Object :: is_initialized ());

	bool result = (children . find (t) != children . end ());
	
	assert (result == (t -> is_in (this)));

	if ((! result) && recursive)
	{
		for (T * i = get_one_child (); i != NULL; i = get_another_child ())
		{
			if (i -> is_type <Set <T> > ())
			{
				if (i -> to_type <Set <T> > () -> contains (t, recursive))
				{
					return true;
				}
			}
		}
	}
	
	return result;
}

//	virtual
template <typename T> bool Set <T> ::
	move_to (T * t, Set <T> * other_set)
{
	trace () << "move_to (" << * t << ", " << * other_set << ")" << endl;
	assert (Object :: is_initialized ());
	assert (t -> is_initialized ());
	assert (other_set != NULL);
	assert (t -> is_initialized ());
	assert (other_set -> is_initialized ());
	assert (contains (t, false));

	t -> remove_from (this);
	if (other_set -> add (t))
	{
		children . erase (t);
		return true;
	}
	t -> put_in (this);
	return false;
}

template <typename T> template <typename U> U * Set <T> ::
	get_child ()
	const
{
	trace () << "get_child ()" << endl;
	assert (Object :: is_initialized ());
	
	for (T * i = get_one_child (); i != NULL; i = get_another_child ())
	{
		if (i -> is_type <U> ())
		{
			return i -> to_type <U> ();
		}
	}
	return NULL;
}

template <typename T> template <typename U> U * Set <T> ::
	get_child (string name)
	const
{
	trace () << "get_child (" << name << ")" << endl;
	assert (Object :: is_initialized ());

	for (_Rb_tree_const_iterator <T *> i = children . begin ();
													i != children . end (); i ++)
	{
		if ((* i) -> is_type <U> () && ((* i) -> compare (name) == 0))
		{
			return (* i) -> to_type <U> ();
		}
	}
	return NULL;
}

template <typename T> T * Set <T> ::
	get_one_child ()
	const
{
	trace () << "get_one_child ()" << endl;
	assert (Object :: is_initialized ());

	if (children . empty ())
	{
		return NULL;
	}
	const_cast <Set <T> *> (this) -> next_child = children . begin ();
	return * (const_cast <Set <T> *> (this) -> next_child ++);
}

template <typename T> T * Set <T> ::
	get_another_child ()
	const
{
	trace () << "get_another_child ()" << endl;
	assert (Object :: is_initialized ());

	if (next_child == children . end ())
	{
		return NULL;
	}
	return * (const_cast <Set <T> *> (this) -> next_child ++);
}

#endif	//	SET_IPP
