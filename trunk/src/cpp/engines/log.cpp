#include <fstream>
#include <sstream>

#include "engines/log.hpp"
#include "engines/mediator.hpp"
#include "engines/settings.hpp"
#include "messages/list_update.hpp"

using namespace std;
using namespace Radakan;
using namespace Radakan :: Engines;

//	static
string Log ::
	get_class_name ()
{
	return "Engines :: Log";
}

//  constructor
Log ::
	Log () :
	Object ("log", "singleton")
{
	trace (me, Log :: get_class_name (), "");

	string radakan_path = Settings :: get () -> radakan_path;
	
	Log :: log (me) << "All further logs will be written to '" << radakan_path << "/log/log.txt'." << endl;

	//	'Log :: log (me)' is redirected to a log file.
	cout . rdbuf ((new ofstream ((radakan_path + "/log/log.txt") . c_str ())) -> rdbuf ());
	cerr . rdbuf (cout . rdbuf ());
	clog . rdbuf (cout . rdbuf ());
	
	assert (is_initialized ());
}

//  destructor
Log ::
	~Log ()
{
	trace (me, Log :: get_class_name (), "~");
	assert (is_initialized ());

	prepare_for_destruction ();
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
	error (const Reference_Base & logger)
{
	log (logger) << "An error occurred:" << endl;
	return cout << "\t";
}

//	static
ostream & Log ::
	log (const Reference_Base & logger)
{
	#ifdef RADAKAN_DEBUG
		cout << endl;
		cout << logger << " reports:" << endl;
		return cout << "\t";
	#else
		return cout;	//	completely ignored
	#endif
}

//	static
void Log ::
	show (string message_contents)
{
	trace (get (), Log :: get_class_name (), "show", message_contents);
	
	Mediator :: get () -> call_observers <Messages :: List_Update>
		(Messages :: List_Update :: create (message_contents, "log"));
}

//	static
void Log ::
	trace
	(
		const Reference_Base & logger,
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

//	static
void Log ::
	no_logs (const Reference_Base & logger)
{
	Log :: log (logger) << "No further logging will occur." << endl;

	//	From here on, all Log :: log (me) messages are ignored.
	cout . rdbuf ((new ostream (new stringbuf (ios_base :: out))) -> rdbuf ());
}
