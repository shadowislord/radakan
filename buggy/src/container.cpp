#include "container.hpp"

using namespace std;

//  constructor
Container ::
	Container (string new_name):
	Tree <Entity> (new_name)
{
	assert (Tree <Entity> :: is_initialized ());
	
	assert (is_initialized ());
}

//  constructor
Container ::
	Container ( Entity * new_entity):
	Tree <Entity> (new_entity)
{
	assert (Tree <Entity> :: is_initialized ());
	
	assert (is_initialized ());
}

//  destructor
Container ::
	~Container ()
{
	assert (is_initialized ());

}

//	virtual
bool Container ::
	is_initialized ()
	const
{
	return Tree <Entity> :: is_initialized ();
}

//	virtual
float Container ::
	get_total_weight ()
	const
{
	assert (is_initialized ());
	
	float total_weight = data->weight;

/* !!!*/	for (set <Tree <Entity> *> :: const_iterator i = children.begin ();
													i != children.end (); i ++)
	{
		assert ((* i)->is_type <Container> ());
		total_weight += (* i)->to_type <Container> ()->get_total_weight ();
	}

	return total_weight;
}

//	returns subtree, iff succes
//	virtual
Container * Container ::
	add (Entity * entity)
{
	Container * subcontainer = new Container (entity);

	Tree <Entity> :: add (subcontainer);

	return subcontainer;
}
