#include <fstream>
#include <sstream>
#include "object.hpp"

using namespace std;
using namespace tsl;

//  constructor
Object ::
	Object (string new_name)
{
	assert (! new_name . empty ());
	trace () << "Object (" << new_name << ")" << endl;
	
	assign (new_name);
	parent = NULL;
	
	#ifdef TSL_TRACE
		objects . insert (this);

		trace () << "So far, we have:" << endl;
		for (set <Object *> :: const_iterator i = objects . begin ();
													i != objects . end (); i ++)
		{
			trace () << "\t" << * * i << " (" << * i << ")" << endl;
		}
	#endif

	assert (Object :: is_initialized ());
}

//  destructor
Object ::
	~Object ()
{
	trace () << "~Object ()" << endl;
	assert (is_initialized ());
	
	#ifdef TSL_DEBUG
		objects . erase (this);
	#endif
}

//	virtual
bool Object ::
	is_initialized ()
	const
{
	//	checks for empty string
	assert (warn <Object> (! empty ()));
	#ifdef TSL_DEBUG
		assert (warn <Object> (objects . find (const_cast <Object *> (this)) != objects . end ()));
	#endif
	return true;
}

//	static
string Object ::
	get_type_name ()
{
	return "object";
}

//	virtual
ostream & Object ::
	debug ()
	const
{
	#ifdef TSL_DEBUG
		return print ("debug");
	#else
		return cout << "<#1>";
	#endif
}

ostream & Object ::
	trace ()
	const
{
	#ifdef TSL_TRACE
		return print ("trace");
	#else
		return cout << "<#2>";
	#endif
}

ostream & Object ::
	error ()
	const
{
	return cerr << "ERROR: '" << * this << "' (" << this << ") ";
}

bool Object ::
	has_parent ()
	const
{
	assert (is_initialized ());
	
	return (parent != NULL);
}

bool Object ::
	is_in (const Object & set)
	const
{
	assert (is_initialized ());

	return (parent == & set);
}

void Object ::
	put_in (const Object & new_parent)
{
	trace () << "put_in (" << new_parent << ")" << endl;
	assert (Object :: is_initialized ());
	assert (! has_parent ());

	parent = & new_parent;
}

void Object ::
	remove_from (const Object & old_parent)
{
	assert (is_initialized ());
	assert (is_in (old_parent));

	parent = NULL;
}

string Object ::
	to_string (const Object * object)
{
	if (object != NULL)
	{
		return * object;
	}
	return "NULL";
}

string Object ::
	bool_to_string (bool value)
{
	if (value)
	{
		return "true";
	}
	return "false";
}

string Object ::
	to_string (float value)
{
	ostringstream oss;
	oss << value;	//	insert int into stream
	return oss.str ();
}

int Object ::
	to_int (string value)
{
	istringstream iss (value);
	int result;
	iss >> result;	//	extract int from stream
	return result;
}

bool Object ::
	is_nan (float value)
{
	return (value != value);
}

ostream & Object ::
	print (string message) const
{
	return cout << message << ": " << * this << " - ";
}
