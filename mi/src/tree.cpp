#include "tree.hpp"

using namespace std;

//  constructor
Tree ::
	Tree (string new_name):
	Object (new_name)
{
	assert (Object :: is_initialized ());

	assert (is_initialized ());
}

//  destructor
Tree ::
	~Tree ()
{
	assert (Object :: is_initialized (* this + "->~Tree ()"));

	for (set <Object *> :: const_iterator i = children.begin ();
													i != children.end (); i ++)
	{
		debug () << "deleting " << * (* i) << "... " << (long int) (* i) << endl;
		delete (* i);
	}
}

//	virtual
bool Tree ::
	is_initialized ()
	const
{
	return Object :: is_initialized ();
}

//	virtual
bool Tree ::
	add (Object * sub_tree)
{
	assert (is_initialized ());
	assert (sub_tree->is_in (NULL));

	sub_tree->put_in (this);
	debug () << * sub_tree << " added to " << * this << endl;
	
	//	second means we're interested in if it worked or not.
	//	first would give a iterator to the item
	return children.insert (sub_tree).second;
}

//	virtual
bool Tree ::
	contains (Object * sub_tree)
	const
{
	assert (is_initialized ());

	bool result = (children.find (sub_tree) != children.end ());
	
	assert (result == (sub_tree->is_in (this)));
	return result;
}

//	virtual
bool Tree ::
	move_to (Object * sub_tree, Object * other_tree)
{
	assert (is_initialized ());
	assert (contains (sub_tree));

	sub_tree->remove_from (this);
	if (other_tree->add (sub_tree))
	{
		children.erase (sub_tree);
		return true;
	}
	sub_tree->put_in (other_tree);
	return false;
}
