#include <fstream>
#include <sstream>
#include "object.hpp"

using namespace std;

#ifdef SL_DEBUG
	extern ofstream * log_cout;
#else
	extern ostream * log_cout;
#endif

//  constructor
Object ::
	Object (string new_name)
{
	assert (! new_name.empty ());
	debug () << new_name << "->Object :: Object (" << new_name << ")" << endl;
	
	assign (new_name);
	tree_parent = NULL;
	
	#ifdef SL_DEBUG
		objects.insert (this);
	#endif
}

//  destructor
Object ::
	~Object ()
{
	assert (is_initialized (* this + "->Object :: ~Object ()"));
	
	#ifdef SL_DEBUG
		objects.erase (this);
	#endif
}

//	virtual
bool Object ::
	is_initialized ()
	const
{
	//	checks for empty string
	return ! empty ();
}

bool Object ::
	is_initialized (string debug_message)
	const
{
	debug () << debug_message << endl;
	return is_initialized ();
}

//	virtual
ostream & Object ::
	print ()
	const
{
	return cout;
}

//	virtual
ostream & Object ::
	debug ()
	const
{
	return * log_cout << "debug: ";
}

//	virtual
ostream & Object ::
	error ()
	const
{
	return * log_cout << "ERROR: ";
}

bool Object ::
	is_in_tree (const Object * tree)
	const
{
	assert (is_initialized ());
	
	return (tree_parent == tree);
}

void Object ::
	put_in_tree (Object * new_tree_parent)
{
	assert (is_initialized ());
	assert (tree_parent == NULL);
	assert (new_tree_parent != NULL);

	tree_parent = new_tree_parent;
}

void Object ::
	remove_from_tree (Object * old_tree_parent)
{
	assert (is_initialized ());
	assert (is_in_tree (old_tree_parent));

	tree_parent = NULL;
}

string to_string (float value)
{
	ostringstream oss;
	oss << value;	//	insert int into stream
	return oss.str ();
}

int to_int (string value)
{
	istringstream iss (value);
	int result;
	iss >> result;	//	extract int from stream
	return result;
}

bool is_nan (float value)
{
	return (value != value);
}
