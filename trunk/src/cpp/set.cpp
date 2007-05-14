#include "set.hpp"

using namespace std;
using namespace TSL;

//	static
template <class T> const string Set <T> ::
	class_name ("Set <" + T :: class_name + ">");

//	This could have been any number (strictly) below 0.
template <class T> const int Set <T> ::
	unlimited = - 10;

//  constructor
template <class T> Set <T> ::
	Set (int new_maximal_size, string name) :
	Object (name),
	maximal_size (new_maximal_size),
	sealed (false)
{
	log (debugging) << class_name << " (" << new_maximal_size << ")" << endl;
	assert (Object :: is_initialized ());
	assert ((new_maximal_size == unlimited) || (0 <= new_maximal_size));

	assert (Set <T> :: is_initialized ());
	log (debugging) << class_name << " () finished" << endl;
}

//  destructor
template <class T> Set <T> ::
	~Set ()
{
	log (debugging) << "~" << class_name << " ()" << endl;
	assert (Set <T> :: is_initialized ());

	delete_children ();
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
	log (debugging) << "add (" << t << ")" << endl;
	assert (Set <T> :: is_initialized ());
	assert (! sealed);
	assert (! t . has_parent (T :: class_name));

	if ((maximal_size != unlimited) && (children . size () == maximal_size))
	{
		log (debugging) << t << " could not be added. I'm full." << endl;
		return false;
	}

	t . put_in (* this, T :: class_name);
	log (debugging) << t << " was added." << endl;

	//	'second' means we're interested in if it worked or not.
	//	'first' would give a iterator to the item.
	bool check = children . insert (& t) . second;
	assert (check);
	
	assert (Set <T> :: is_initialized ());
	return true;
}

//	virtual
template <class T> bool Set <T> ::
	contains (string name)
	const
{
//	log (debugging) << "contains (" << name << ")" << endl;
	assert (Set <T> :: is_initialized ());

	for (T * i = get_child (); i != NULL; i = get_another_child ())
	{
		if (* i == name)
		{
			return true;
		}
	}
	
	return false;
}

//	virtual
template <class T> bool Set <T> ::
	move (T & t, Set <T> & destination)
{
	log (debugging) << "move (" << t << ", " << destination << ")" << endl;
	assert (Set <T> :: is_initialized ());
	assert (t . is_initialized ());
	assert (destination . is_initialized ());
	assert (! sealed);
	assert (contains (t));

	t . remove_from (* this, T :: class_name);
	if (destination . add (t))
	{
		drop (t);

		return true;
	}
	t . put_in (* this, T :: class_name);
	
	return false;
}

template <class T> void Set <T> ::
	drop (T & t)
{
	assert (Set <T> :: is_initialized ());
	assert (! sealed);
	assert (t . is_in (* this, T :: class_name));

	children . erase (& t);
}

template <class T> T & Set <T> ::
	get_child (string name)
	const
{
	log (debugging) << "get_child (" << name << ")" << endl;
	assert (Set <T> :: is_initialized ());
	assert (contains (name));

	for (T * i = get_child (); i != NULL; i = get_another_child ())
	{
		if (* i == name)
		{
			return * i;
		}
	}

	error () << "Child not found." << endl;
	abort ();
	return * get_child ();
}

template <class T> T * Set <T> ::
	get_child ()
	const
{
//	log (debugging) << "get_child ()" << endl;
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
//	log (debugging) << "get_another_child ()" << endl;
	assert (Set <T> :: is_initialized ());

	if (next_child == children . end ())
	{
		return NULL;
	}
	return * (next_child ++);
}

template <class T> void Set <T> ::
	delete_children ()
{
	assert (Set <T> :: is_initialized ());
	assert (! sealed);

	const int initial_number_of_children = children . size ();

	//	We can't use 'get_child' and 'get_another_child' here.

	int child_number = 0;
	for (T_iterator i = children . begin (); i != children . end ();
		i = children . begin ())
	{
		child_number ++;
		assert (child_number <= initial_number_of_children);
		assert (* i != NULL);
		log (debugging) << "deleting child " << child_number << " of "
			<< initial_number_of_children << ": " << * * i << "..." << endl;
		drop (* * i);
		if ((* i) -> is_orphan ())
		{
			delete *i;
		}
	}

	assert (children . empty ());
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
#include "gui.hpp"
#include "tile.hpp"
#include "audio_engine.hpp"

template class Set <Body>;
template class Set <GUI>;
template class Set <Item>;
template class Set <Object>;
template class Set <NPC>;
template class Set <Sound>;
template class Set <Tile>;
