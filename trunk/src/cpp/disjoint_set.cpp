#include "disjoint_set.hpp"

using namespace std;
using namespace tsl;

//  constructor
template <class T> Disjoint_Set <T> ::
	Disjoint_Set () :
	Object ("The name doesn't matter as this class is an abstact class.")
{
	trace () << get_class_name () << " ()" << endl;
	assert (Object :: is_initialized ());

	assert (Disjoint_Set <T> :: is_initialized ());
	trace () << get_class_name () << " () finished" << endl;
}

//  destructor
template <class T> Disjoint_Set <T> ::
	~Disjoint_Set ()
{
	trace () << "~" << get_class_name () << " ()" << endl;
	assert (Disjoint_Set <T> :: is_initialized ());

	delete_children ();
}

//	virtual
template <class T> bool Disjoint_Set <T> ::
	is_initialized ()
	const
{
	assert (warn <Disjoint_Set <T> > (Object :: is_initialized ()));
	#ifdef TSL_TRACE
		//	Don't use the 'get_child' & 'get_another_child' methods here,
		//	as they both require me to be initialized.

		for (typename set<T*>::const_iterator i = children.begin (); i != children.end (); i ++)

		for (T_iterator i = children . begin (); i != children . end (); i ++)

		{
			assert (warn <Disjoint_Set <T> > ((* i) -> T :: is_initialized ()));
		}
	#endif
	
	return true;
}

//	static
template <class T> string Disjoint_Set <T> ::
	get_class_name ()
{
	return "Disjoint_Set <" + T :: get_class_name () + ">";
}

template <class T> bool Disjoint_Set <T> ::
	is_empty () const
{
	assert (Disjoint_Set <T> :: is_initialized ());

	return children . empty ();
}

//	virtual
template <class T> bool Disjoint_Set <T> ::
	add (T & t)
{
	trace () << "add (" << t << ")" << endl;
	assert (Disjoint_Set <T> :: is_initialized ());
	assert (! t . has_parent ());

	debug () << t << " will be added..." << endl;
	t . put_in (* this);
	debug () << t << " was added." << endl;

	//	'second' means we're interested in if it worked or not.
	//	'first' would give a iterator to the item.
	bool check = children . insert (& t) . second;
	assert (check);
	
	assert (Disjoint_Set <T> :: is_initialized ());
	return true;
}

//	virtual
template <class T> bool Disjoint_Set <T> ::
	contains (string name)
	const
{
//	trace () << "contains (" << name << ")" << endl;
	assert (Disjoint_Set <T> :: is_initialized ());

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
template <class T> bool Disjoint_Set <T> ::
	move (T & t, Disjoint_Set <T> & destination)
{
	trace () << "move (" << t << ", " << destination << ")" << endl;
	assert (Disjoint_Set <T> :: is_initialized ());
	assert (t . is_initialized ());
	assert (destination . is_initialized ());
	assert (contains (t));

	t . remove_from (* this);
	children . erase (& t);
	bool check = destination . add (t);
	assert (check);

	return true;
}

template <class T> T & Disjoint_Set <T> ::
	get_child (string name)
	const
{
	trace () << "get_child (" << name << ")" << endl;
	assert (Disjoint_Set <T> :: is_initialized ());
	assert (contains (name));

	for (T * i = get_child (); i != NULL; i = get_another_child ())
	{
		if (* i == name)
		{
			return * i;
		}
	}

	abort ();
	return * get_child ();
}

template <class T> T * Disjoint_Set <T> ::
	get_child ()
	const
{
//	trace () << "get_child ()" << endl;
	assert (Disjoint_Set <T> :: is_initialized ());

	if (children . empty ())
	{
		return NULL;
	}
	next_child = children . begin ();
	return * (next_child ++);
}

template <class T> T * Disjoint_Set <T> ::
	get_another_child ()
	const
{
//	trace () << "get_another_child ()" << endl;
	assert (Disjoint_Set <T> :: is_initialized ());

	if (next_child == children . end ())
	{
		return NULL;
	}
	return * (next_child ++);
}

template <class T> void Disjoint_Set <T> ::
	delete_children ()
{
	assert (Disjoint_Set <T> :: is_initialized ());

	const int initial_number_of_children = children . size ();

	//	We can't use 'get_child' and 'get_another_child' here.

	int child_number = 0;
	for (typename set<T*> :: const_iterator i = children . begin (); i != children . end (); i ++)

	for (T_iterator i = children . begin (); i != children . end (); i ++)

	{
		child_number ++;
		assert (child_number <= initial_number_of_children);
		assert (* i != NULL);
		trace () << "deleting child " << child_number << " of " << initial_number_of_children << ": " << * * i << "..." << endl;
		delete * i;
	}

	children . clear ();
}

//	to avert linking errors:
#include "gui.hpp"
#include "tile.hpp"
#include "audio_engine.hpp"

template class Disjoint_Set <GUI>;
template class Disjoint_Set <Item>;
template class Disjoint_Set <Tile>;
template class Disjoint_Set <Sound>;
