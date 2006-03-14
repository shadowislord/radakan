#ifndef OBJECT_CPP
#define OBJECT_CPP

#include <fstream>
#include <sstream>
#include "object.hpp"

using namespace std;

ofstream debug_cout ("log.txt");

//  Constructor
Object::
	Object (string new_name)
{
	assert (! new_name.empty ());
	debug () << new_name << "->Object::Object (" << new_name << ")" << endl;
	objects.push_back (this);
	assign (new_name);
//	assign (new_name + "_" + to_string (objects.size () - 1));
}

//  Destructor
Object::
	~Object ()
{
	debug () << * this << "->Object::~Object ()" << endl;
	assert (is_initialized ());
	
	for (unsigned int i = 0; i < objects.size (); i++)
	{
		if (objects.at (i) == this)
		{
			objects.at (i) = NULL;
//			debug () << * this << " (object nr. " << i << ") is destructed. " << endl;
			break;	//	breaks from the for loop to speed up things
		}
	}
}

//	checks for empty string
//	virtual
bool
	Object::
	is_initialized ()
	const
{
//	debug () << * this  << "->Object::is_initialized ()" << endl;
//	debug () << "result: " <<  ! empty () << " (1 means true)" << endl;
	return ! empty ();

}

//	virtual
ostream&
	Object::
	print ()
	const
{
	return cout;
}

//	virtual
ostream&
	Object::
	debug ()
	const
{
	return debug_cout << "DEBUG: ";
//	return cout << "DEBUG: ";
}

//	virtual
ostream&
	Object::
	error ()
	const
{
	return cerr << "ERROR: ";
}

string to_string (int value)
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

#endif
