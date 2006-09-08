#ifndef TREE_IPP
#define TREE_IPP

#include "tree.hpp"

using namespace std;

//  constructor
template <typename T> Tree <T> ::
	Tree (string new_name):
	Object (new_name)
{
	assert (Object :: is_initialized ());

	data = NULL;
	
	assert (is_initialized ());
}

//  constructor
template <typename T> Tree <T> ::
	Tree (T * new_t):
	Object (* new_t + " tree")
{
	assert (Object :: is_initialized ());
	assert (new_t != NULL);
	assert (new_t->is_initialized ());

	data = new_t;
	
	assert (is_initialized ());
}

//  destructor
template <typename T> Tree <T> ::
	~Tree ()
{
	assert (Object :: is_initialized (* this + "->Entity :: ~Entity ()"));

	delete data;
	
//	I've no idea why the out commented version foesn't work...

//	for (set <Tree <T> *> :: const_iterator i = children.begin ();
//													i != children.end (); i ++)
	for (_Rb_tree_const_iterator <Tree <T> *> i = children.begin (); i != children.end (); i ++)
	{
		delete (* i);
	}
}

//	virtual
template <typename T> bool Tree <T> ::
	is_initialized ()
	const
{
	return Object :: is_initialized ();
}

//	virtual
template <typename T> bool Tree <T> ::
	is_initialized (string debug_message)
	const
{
	debug () << debug_message << endl;

	return is_initialized ();
}

//	virtual
template <typename T> Tree <T> * Tree <T> ::
	add (T * t)
{
	assert (is_initialized ());
	assert (t->is_in_tree (NULL));

	Tree <T> * tree = new Tree <T> (t);

	if (add (tree))
	{
		return tree;
	}
	return NULL;
}

//	virtual
template <typename T> bool Tree <T> ::
	add (Tree <T> * tree)
{
	assert (is_initialized ());
	assert (tree->is_in_tree (NULL));

	tree->put_in_tree (this);
	debug () << * tree << " added to " << * this << endl;
	
	//	second means we're interested in if it worked or not.
	//	first would give a iterator to the item
	return children.insert (tree).second;
}

//	virtual
template <typename T> bool Tree <T> ::
	contains (T * t)
	const
{
	assert (is_initialized ());

	bool result = false;

//	I've no idea why the out commented version doesn't work...

//	for (set <Tree <T> *> :: const_iterator i = children.begin ();
//													i != children.end (); i ++)
//	for (set <Tree <T> *> :: iterator i = children.begin ();
//													i != children.end (); i ++)
	for (_Rb_tree_const_iterator <Tree <T> *> i = children.begin (); i != children.end (); i ++)
	{
		if ((* i)->data == t)
		{
			result = true;
		}
	}

	return result;
}

//	virtual
template <typename T> bool Tree <T> ::
	contains (Tree <T> * tree)
	const
{
	assert (is_initialized ());

	bool result = (children.find (tree) != children.end ());
	
	assert (result == (tree->is_in_tree (this)));
	return result;
}

//	virtual
template <typename T> bool Tree <T> ::
	move_to (T * t, Tree <T> * other_tree)
{
	assert (is_initialized ());

	Tree <T> * sub_tree = NULL;

//	I've no idea why the out commented version foesn't work...

//	for (set <Tree <T> *> :: const_iterator i = children.begin ();
//													i != children.end (); i ++)
	for (_Rb_tree_const_iterator <Tree <T> *> i = children.begin (); i != children.end (); i ++)
	{
		if ((* i)->data == t)
		{
			sub_tree = * i;
		}
	}

	assert (sub_tree != NULL);

	sub_tree->remove_from_tree (this);
	if (other_tree->add (sub_tree))
	{
		children.erase (sub_tree);
		return true;
	}
	sub_tree->put_in_tree (other_tree);
	return false;
}

//	virtual
template <typename T> bool Tree <T> ::
	move_to (Tree <T> * sub_tree, Tree <T> * other_tree)
{
	assert (is_initialized ());
	assert (contains (sub_tree));

	sub_tree->remove_from_tree (this);
	if (other_tree->add (sub_tree))
	{
		children.erase (sub_tree);
		return true;
	}
	sub_tree->put_in_tree (other_tree);
	return false;
}

#endif	//	TREE_IPP
