#include <fstream>
#include <sstream>
#include "log.hpp"

using namespace std;
using namespace TSL;

//	static
const string Log ::
	class_name ("Log");

//  constructor
Log ::
	Log () :
	Object ("Log"),
	the_abyss (NULL)
{
	Object :: log (debugging) << class_name << " ()" << endl;
	assert (Object :: is_initialized ());
	
	assert (is_initialized ());
}

//  destructor
Log ::
	~Log ()
{
	Object :: log (debugging) << "~" << class_name << " ()" << endl;
	assert (is_initialized ());
}

//	virtual
bool Log ::
	is_initialized ()
	const
{
	return Object :: is_initialized ();
}

ostream & Log ::
	error (const Object & logger)
	const
{
	log (logger, true) << "An error occurred:" << endl;
	return cout << "\t";
}

ostream & Log ::
	log (const Object & logger, bool condition)
	const
{
	if (condition)
	{
		#ifdef TSL_DEBUG
			cout << endl;
			cout << "(turn " << turn << ")" << endl;
			cout << "'" << logger << "' reports: " << endl;
			return cout << "\t";
		#else
			return cout;
		#endif
	}
	
	return * const_cast <ostream *> (& the_abyss);
}

void Log ::
	show (string message, bool condition)
	const
{
	if (condition)
	{
		for (list <Observer <Log> *> :: const_iterator i = observers . begin ();
			i != observers . end (); i ++)
		{
			log (* this, true) << "Showing: '" << message << "'" << endl;
			(* i) -> call ("show", message);
		}
	}
}
