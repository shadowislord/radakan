#include "engines/log.hpp"
#include "engines/tracker.hpp"

using namespace std;
using namespace Radakan;

string Radakan ::
	to_string (const Ogre :: Vector3 & vector)
{
	return "(" + to_string (vector . x) + ", " + to_string (vector . y) + ", "
												+ to_string (vector . z) + ")";
}

//	static
string Object ::
	get_class_name ()
{
	//	Do not turn this into a static data member,
	//	that'd cause problems for template (sub)classes.

	return "Object";
}

//  constructor
Object ::
	Object (string new_name, bool new_prevent_automatic_destruction) :
	name (new_name),
	prevent_automatic_destruction (new_prevent_automatic_destruction),
	dependencies (new set <const Reference_Base *>),
	status ("constructing"),
	me (this)
{
	//	reset_pointee (this);
	
	Engines :: Log :: trace (me, Object :: get_class_name (), "", new_name, to_string (new_prevent_automatic_destruction));
	assert (! name . empty ());

	#ifdef RADAKAN_DEBUG
		if (Engines :: Tracker :: is_instantiated ())
		{
			bool check = Engines :: Tracker :: get () -> add (me);
			assert (check);
			//	assert (does_depend (Engines :: Tracker :: get ()));

			is_tracked = true;
		}
		else
		{
			is_tracked = false;
		}
	#endif

	status = "running";
	assert (Object :: is_initialized ());
	Engines :: Log :: trace (me, Object :: get_class_name (), "", new_name, "(end)");
}

//  destructor
Object ::
	~Object ()
{
	Engines :: Log :: trace (me, Object :: get_class_name (), "~");
	assert (is_initialized ());

	prepare_for_destruction ();

	Engines :: Log :: log (me) << "Farewell..." << endl;
}

void Object ::
	prepare_for_destruction ()
{
	Engines :: Log :: trace (me, Object :: get_class_name (), "prepare_for_destruction");
	assert (is_initialized ());

	status = "destructing";

	for (set <const Reference_Base *> :: const_iterator i = dependencies -> begin ();
		i != dependencies -> end (); i = dependencies -> begin ())
	{
		Engines :: Log :: log (me) << (* i) -> get_name () << " is going be be destruct-ed." << endl;
		(* i) -> destruct ();
	}
}

//	virtual
bool Object ::
	is_initialized ()
	const
{
	//	Engines :: Log :: trace (me, Object :: get_class_name (), "is_initialized");
	
	//	checks for empty string
	assert (! name . empty ());

	//	checks if dependencies contains an object
	assert (dependencies);
	
	return true;
}

//	private
bool Object ::
	does_depend (const Reference_Base & candidate)
	const
{
	//	Engines :: Log :: trace (me, Object :: get_class_name (), "does_depend", candidate);
	//	assert (Object :: is_initialized ());

	return (0 < dependencies -> count (& candidate));
}

bool Object ::
	is_destructing ()
	const
{
	//	Being initialized isn't necessairy here.

	return status == "destructing";
}

void  Object ::
	register_reference (const Reference_Base & reference)
	const
{
	//	Engines :: Log :: trace (me, Object :: get_class_name (), "register_reference", reference . get_name ());
	assert (Object :: is_initialized ());
	assert (! does_depend (reference));

	bool check = dependencies -> insert (& reference) . second;

	assert (check);
	assert (does_depend (reference));

	if (name . find ("tile") != string :: npos)
	{
		for (set <const Reference_Base *> :: iterator i = dependencies -> begin (); i != dependencies -> end ();
				i ++)
		{
			Engines :: Log :: log (me) << "Dependency: " << (* i) -> get_name () << endl;
		}
	}
}

void  Object ::
	unregister_reference (const Reference_Base & reference)
	const
{
	//	Engines :: Log :: trace (me, Object :: get_class_name (), "unregister_reference", reference . get_name ());
	
	assert (Object :: is_initialized ());
	assert (does_depend (reference));

	dependencies -> erase (& reference);

	/*for (set <const Reference_Base *> :: iterator i = dependencies -> begin (); i != dependencies -> end ();
			i ++)
	{
		Engines :: Log :: log (me) << "Dependency: " << (* i) -> get_name () << endl;
	}*/

	if (status == "constructing")
	{
		//	Engines :: Log :: log (me) << "I'm still constructing." << endl;
		return;
	}

	if (status == "destructing")
	{
		//	Engines :: Log :: log (me) << "I'm already destructing." << endl;
		return;
	}

	if (prevent_automatic_destruction)
	{
		//	Engines :: Log :: log (me) << "I will not self-destruct, because I prevent it." << endl;
		return;
	}

	if (name . find ("static") != string :: npos)
	{
		//	Engines :: Log :: log (me) << "I will not self-destruct, because I'm a static." << endl;
		return;
	}

	unsigned int self_destruct_criterion = 0;
	self_destruct_criterion ++; //	I shouldn't forbid myself to self-destruct.

	#ifdef RADAKAN_DEBUG
		if (is_tracked)
		{
			//	The 'Tracker' should not forbid me to destruct.
			self_destruct_criterion ++;
		}
	#endif

	assert (dependencies -> size () >= self_destruct_criterion);

	if (dependencies -> size () == self_destruct_criterion)
	{
		Engines :: Log :: log (me) << "I will self-destruct, because I have no more dependencies." << endl;
		delete this;
	}

	//	Engines :: Log :: log (me) << "I will not self-destruct, because I have another dependency." << endl;
}
