#ifndef SET_IPP
#define SET_IPP

#include "set.hpp"

using namespace std;

//  constructor
template <typename T> Set <T> ::
	Set (string new_name) :
	Object (new_name)
{
	debug () << * this << " -> Set (" << * this << ")"<< endl;
	assert (Object :: is_initialized ());

	assert (is_initialized ());
}

//  destructor
template <typename T> Set <T> ::
	~Set ()
{
	assert (Object :: is_initialized (* this + " -> ~Set ()"));

	for (_Rb_tree_const_iterator <T *> i = children . begin ();
													i != children . end (); i ++)
	{

		debug () << "deleting " << * (* i) << "... " << (long int) (* i) << endl;
		delete (* i);
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
	assert (is_initialized ());
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
	assert (is_initialized ());

	bool result = (children . find (t) != children . end ());
	
	assert (result == (t -> is_in (this)));

	if ((! result) && recursive)
	{
		for (_Rb_tree_const_iterator <T *> i = children . begin ();
													i != children . end (); i ++)
		{
			T * temp = * i;
			
			if (temp -> is_type <Set <T> > ())
			{
				if (temp -> to_type <Set <T> > () -> contains (t, recursive))
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
	assert (is_initialized ());
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
	for (_Rb_tree_const_iterator <T *> i = children . begin ();
													i != children . end (); i ++)
	{
		if ((* i) -> is_type <U> ())
		{
			return (* i) -> to_type <U> ();
		}
	}
	return NULL;
}

#endif	//	SET_IPP
