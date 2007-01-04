#include <fstream>
#include <sstream>
#include "object.hpp"

using namespace std;
using namespace sl;

#ifndef SL_DEBUG

	//	!!! Doesn't work yet!
	class IgnoreLog : public ostream
	{
		public:
			IgnoreLog () {init (clog . rdbuf ());};
			IgnoreLog & operator<< (bool& val ){return * this;};
			IgnoreLog & operator<< (short& val ){return * this;};
			IgnoreLog & operator<< (unsigned short& val ){return * this;};
			IgnoreLog & operator<< (int& val ){return * this;};
			IgnoreLog & operator<< (unsigned int& val ){return * this;};
			IgnoreLog & operator<< (long& val ){return * this;};
			IgnoreLog & operator<< (unsigned long& val ){return * this;};
			IgnoreLog & operator<< (float& val ){return * this;};
			IgnoreLog & operator<< (double& val ){return * this;};
			IgnoreLog & operator<< (long double& val ){return * this;};
			IgnoreLog & operator<< (void*& val ){return * this;};
 
			IgnoreLog & operator<< (streambuf& sb ){return * this;};
 
			IgnoreLog & operator<< (ostream& ( *pf )(ostream&)){return * this;};
			IgnoreLog & operator<< (ios& ( *pf )(ios&)){return * this;};
			IgnoreLog & operator<< (ios_base& ( *pf )(ios_base&)){return * this;};
	};
	
	IgnoreLog & operator<< (IgnoreLog & o, char c ){return o;};
	IgnoreLog & operator<< (IgnoreLog & o, signed char c ){return o;};
	IgnoreLog & operator<< (IgnoreLog & o, unsigned char c ){return o;};
 
	IgnoreLog & operator<< (IgnoreLog & o, const char* s ){return o;};
	IgnoreLog & operator<< (IgnoreLog & o, const signed char* s ){return o;};
	IgnoreLog & operator<< (IgnoreLog & o, const unsigned char* s ){return o;};
	
	template <class T> IgnoreLog & operator << (IgnoreLog & o , T const & obj)
	{return o;}

	IgnoreLog * ignore = new IgnoreLog();
#endif

//  constructor
Object ::
	Object (string new_name)
{
	assert (! new_name . empty ());
	trace () << "Object (" << new_name << ")" << endl;
	
	assign (new_name);
	parent = NULL;
	
	#ifdef SL_TRACE
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
	assert (warn <Object> (! empty ()));
	#ifdef SL_DEBUG
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
	#ifdef SL_DEBUG
		return print ("debug");
	#else
		return * ignore;
	#endif
}

ostream & Object ::
	trace ()
	const
{
	#ifdef SL_TRACE
		return print ("trace");
	#else
		return * ignore;
	#endif
}

ostream & Object ::
	error ()
	const
{
	return print ("ERROR");
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
	return clog << message << ": " << * this << " - ";
}
