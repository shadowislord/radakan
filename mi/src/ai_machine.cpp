#include "ai_machine.hpp"

using namespace std;

//  constructor
AI_Machine ::
	AI_Machine (string new_name):
	Object (new_name),
	AI_State (new_name),
	Tree (new_name)
{
	assert (AI_State :: is_initialized ());
	assert (Tree :: is_initialized ());
	
	assert (is_initialized ());
}

//  destructor
AI_Machine ::
	~AI_Machine ()
{
	assert (is_initialized ());

}

//	virtual
bool AI_Machine ::
	is_initialized ()
	const
{
	return AI_State :: is_initialized () && Tree :: is_initialized ();
}

//	returns subtree, iff succes
//	virtual
bool AI_Machine ::
	add (Object * sub_tree)
{
	assert (is_initialized ());
	assert (sub_tree != NULL);
	assert (sub_tree->is_initialized ());
	assert (sub_tree->is_type <AI_State> ());
	
	return Tree :: add (sub_tree);
}
