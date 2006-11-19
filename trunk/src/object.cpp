#include <fstream>
#include <sstream>
#include "object.hpp"

using namespace std;
using namespace sl;

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
	debug () << "Object (" << new_name << ")" << endl;
	
	assign (new_name);
	parent = NULL;
	
	#ifdef SL_DEBUG
		objects.insert (this);
	#endif

	assert (Object :: is_initialized ());
}

//  destructor
Object ::
	~Object ()
{
	assert (is_initialized ("~Object ()"));
	
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
	trace () << debug_message << endl;
	return is_initialized ();
}

//	virtual
ostream & Object ::
	print ()
	const
{
	return * log_cout;
}

//	virtual
ostream & Object ::
	debug ()
	const
{
	return * log_cout << "debug: " << * this << " - ";
}

//	virtual
ostream & Object ::
	trace ()
	const
{
	return * log_cout << "trace: " << * this << " - ";
}

//	virtual
ostream & Object ::
	error ()
	const
{
	return * log_cout << "ERROR: " << * this << " - ";
}

bool Object ::
	is_in (const Object * set)
	const
{
	assert (is_initialized ());
	
	return (parent == set);
}

void Object ::
	put_in (Object * new_parent)
{
	assert (is_initialized ());
	assert (parent == NULL);
	assert (new_parent != NULL);

	parent = new_parent;
}

void Object ::
	remove_from (Object * old_parent)
{
	assert (is_initialized ());
	assert (is_in (old_parent));

	parent = NULL;
}

string sl :: to_string (bool value)
{
	if (value)
	{
		return "true";
	}
	return "false";
}

string sl :: to_string (float value)
{
	ostringstream oss;
	oss << value;	//	insert int into stream
	return oss.str ();
}

int sl :: to_int (string value)
{
	istringstream iss (value);
	int result;
	iss >> result;	//	extract int from stream
	return result;
}

bool sl :: is_nan (float value)
{
	return (value != value);
}
