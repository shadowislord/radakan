#include "engines/log.hpp"
#include "engines/tracker.hpp"

using namespace std;
using namespace Radakan;

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
	Object (string new_name, string automatic_destruction_prevention_key) :
	name (new_name),
	automatic_destruction_preventions (new set <string>),
	strong_dependencies (new set <const Reference_Base *>),
	weak_dependencies (new set <const Reference_Base *>),
	me (this, true)
{
	add_automatic_destruction_prevention ("constructing");
	if (! automatic_destruction_prevention_key . empty ())
	{
		add_automatic_destruction_prevention (automatic_destruction_prevention_key);
	}

	Engines :: Log :: trace (me, Object :: get_class_name (), "", new_name,
		automatic_destruction_prevention_key);
	assert (! name . empty ());

	#ifdef RADAKAN_DEBUG
		if (Engines :: Tracker :: is_tracking ())
		{
			bool check = Engines :: Tracker :: get () -> objects -> add (me);
			assert (check);
			//	assert (does_depend (Engines :: Tracker :: get ()));

			is_tracked = true;
		}
		else
		{
			is_tracked = false;
		}
	#endif

	remove_automatic_destruction_prevention ("constructing");
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

	add_automatic_destruction_prevention ("destructing");

	for (set <const Reference_Base *> :: const_iterator i = weak_dependencies -> begin ();
		i != weak_dependencies -> end (); i = weak_dependencies -> begin ())
	{
		Engines :: Log :: log (me) << (* i) -> get_name () << " is going be be destruct-ed." << endl;
		(* i) -> destruct ();
	}

	for
	(
		set <const Reference_Base *> :: const_iterator i = strong_dependencies -> begin ();
		i != strong_dependencies -> end ();
		i = strong_dependencies -> begin ()
	)
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
	assert (strong_dependencies);
	assert (weak_dependencies);
	
	return true;
}

//	private
bool Object ::
	does_depend (const Reference_Base & candidate)
	const
{
	//	Engines :: Log :: trace (me, Object :: get_class_name (), "does_depend", candidate);
	//	assert (Object :: is_initialized ());

	return (0 < strong_dependencies -> count (& candidate)
		+ weak_dependencies -> count (& candidate));
}

void Object ::
	register_reference (const Reference_Base & reference, bool weak)
	const
{
	//	Engines :: Log :: trace (me, Object :: get_class_name (), "register_reference", reference . get_name ());
	assert (Object :: is_initialized ());
	assert (! does_depend (reference));

	if (weak)
	{
		bool check = weak_dependencies -> insert (& reference) . second;
		assert (check);
	}
	else
	{
		bool check = strong_dependencies -> insert (& reference) . second;
		assert (check);
	}
	assert (does_depend (reference));
}

void Object ::
	unregister_reference (const Reference_Base & reference)
	const
{
	//	Engines :: Log :: trace (me, Object :: get_class_name (), "unregister_reference", reference . get_name ());
	
	assert (Object :: is_initialized ());
	assert (does_depend (reference));

	strong_dependencies -> erase (& reference);
	weak_dependencies -> erase (& reference);


	//	Is there anything that prevents automatic destruction?
	if (! automatic_destruction_preventions -> empty ())
	{
		//	Engines :: Log :: log (me) << "I will not self-destruct, because I prevent it." << endl;
		return;
	}

	if (strong_dependencies -> empty ())
	{
		Engines :: Log :: log (me) << "I will self-destruct, because I have no more strong dependencies." << endl;
		delete this;
	}

	//	Engines :: Log :: log (me) << "I will not self-destruct, because I have another dependency." << endl;
}

#ifdef RADAKAN_DEBUG
	void Object ::
		list_references ()
		const
	{
		for (set <const Reference_Base *> :: iterator i = strong_dependencies -> begin ();
			i != strong_dependencies -> end (); i ++)
		{
			Engines :: Log :: log (me) << "Strong dependency: " << (* * i) . get_name ()
				<< endl;
		}

		for (set <const Reference_Base *> :: iterator i = weak_dependencies -> begin ();
			i != weak_dependencies -> end (); i ++)
		{
			Engines :: Log :: log (me) << "Weak dependency: " << (* * i) . get_name () << endl;
		}
	}
#endif

void Object ::
	add_automatic_destruction_prevention (string key)
{
	Engines :: Log :: trace (me, Object :: get_class_name (),
		"add_automatic_destruction_prevention", key);

	automatic_destruction_preventions -> insert (key);
}

void Object ::
	remove_automatic_destruction_prevention (string key)
{
	Engines :: Log :: trace (me, Object :: get_class_name (),
		"remove_automatic_destruction_prevention", key);
		
	bool check = automatic_destruction_preventions -> erase (key);
	assert (check);
}
