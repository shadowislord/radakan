#include <fstream>
#include <sstream>
#include "object.hpp"

using namespace std;

ofstream debug_cout ("log.txt");

//  Constructor
Object ::
	Object (string new_name)
{
	assert (! new_name.empty ());
	debug () << new_name << "->Object::Object (" << new_name << ")" << endl;
	
	assign (new_name);
	#ifdef SL_DEBUG
		objects.insert (this);
	#endif
}

//  Destructor
Object ::
	~Object ()
{
	assert (is_initialized (* this + "->Object::~Object ()"));
	
	#ifdef SL_DEBUG
		objects.erase (this);
	#endif
}

//	checks for empty string
//	virtual
bool
	Object ::
	is_initialized ()
	const
{
	return ! empty ();
}

bool
	Object ::
	is_initialized (string debug_message)
	const
{
	debug () << debug_message << endl;
	return is_initialized ();
}

//	virtual
ostream &
	Object ::
	print ()
	const
{
	return cout;
}

//	virtual
ostream &
	Object ::
	debug ()
	const
{
	#ifdef SL_DEBUG
		return debug_cout << "debug: ";
	#else
		//	This is faster.
		//	Is it possible to 'absorb' this?
		return cout << "debug: ";
	#endif
}

//	virtual
ostream &
	Object ::
	error ()
	const
{
	return cerr << "ERROR: ";
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
