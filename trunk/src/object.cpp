#include <fstream>
#include <sstream>
#include "object.hpp"

using namespace std;
using namespace sl;

#ifdef SL_DEBUG
	extern ofstream * sl_out;
#endif

//  constructor
Object ::
	Object (string new_name)
{
	assert (! new_name.empty ());
	trace () << "Object (" << new_name << ")" << endl;
	
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
	trace () << "~Object ()" << endl;
	assert (is_initialized ());
	
	#ifdef SL_DEBUG
		objects . erase (this);
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

//	virtual
ostream & Object ::
	print ()
	const
{
	return * sl_out;
}

//	virtual
ostream & Object ::
	debug ()
	const
{
	#ifdef SL_DEBUG
		return * sl_out << "debug: " << * this << " - ";
	#endif
}

//	virtual
ostream & Object ::
	trace ()
	const
{
	#ifdef SL_TRACE
		return * sl_out << "trace: " << * this << " - ";
	#endif
}

//	virtual
ostream & Object ::
	error ()
	const
{
	return * sl_out << "ERROR: " << * this << " - ";
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
	trace () << "put_in (" << to_string (new_parent) << ")" << endl;
	assert (Object :: is_initialized ());
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

string sl :: to_string (Object * object)
{
	if (object != NULL)
	{
		return * object;
	}
	return "NULL";
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
