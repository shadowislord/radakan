#include <fstream>
#include <sstream>
#include "log.hpp"
#ifdef TSL_DEBUG
	#include "world.hpp"
#endif

using namespace std;
using namespace TSL;
using namespace TSL :: Engines;

//	static
const string Log ::
	get_class_name ()
{
	return "Log";
}

//  constructor
Log ::
	Log () :
	Object ("log")
{
	trace (me, Log :: get_class_name (), "");
	
 //	Do nothing.
	
	assert (is_initialized ());
}

//  destructor
Log ::
	~Log ()
{
	trace (me, Log :: get_class_name (), "~");
	assert (is_initialized ());

	forget_dependencies ();
}

//	virtual
bool Log ::
	is_initialized ()
	const
{
	return Object :: is_initialized ();
}

//	static
ostream & Log ::
	error (const Object & logger)
{
	log (logger) << "An error occurred:" << endl;
	return cout << "\t";
}

//	static
ostream & Log ::
	log (const Object & logger)
{
	#ifdef TSL_DEBUG
		cout << endl;
		if (World :: is_instantiated ())
		{
			cout << "= turn " << World :: get () . get_turn () << " =" << endl;
		}
		cout << "'" << logger << "' reports:" << endl;
		return cout << "\t";
	#else
		return cout;	//	completely ignored when not in TSL_DEBUG
	#endif
}

//	static
void Log ::
	show (string message_contents)
{
	trace (get (), Log :: get_class_name (), "show", message_contents);
	
	if (is_instantiated ())
	{
		Object * message = new Object (message_contents);
		get () . call_observers (* message);
		delete message;
	}
}

//	static
void Log ::
	trace
	(
		const Object & logger,
		string class_name,
		string method,
		string argument_1,
		string argument_2,
		string argument_3,
		string argument_4,
		string argument_5,
		string argument_6
	)
{
	//	assert (logger . Object :: is_type <T>);
	
	if (method . empty ())
	{
		method = class_name;
	}
	else if (method . at (0) == '~')
	{
		method = "~" + class_name;
	}

	log (logger) << class_name << " :: " << method << " ("
		<< argument_1
		<< (argument_2 == "" ? "" : ", ") << argument_2
		<< (argument_3 == "" ? "" : ", ") << argument_3
		<< (argument_4 == "" ? "" : ", ") << argument_4
		<< (argument_5 == "" ? "" : ", ") << argument_5
		<< (argument_6 == "" ? "" : ", ") << argument_6
		<< ")" << endl;
}

