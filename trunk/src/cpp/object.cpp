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

		/*trace () << "So far, we have:" << endl;
		for (set <Object *> :: const_iterator i = objects . begin ();
													i != objects . end (); i ++)
		{
			trace () << "\t" << * * i << " (" << * i << ")" << endl;
		}*/
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

template <typename T> bool Object ::
	is_type ()
	const
{
	trace () << "is_type <" << T :: get_type_name () << "> ()" << endl;
	assert (is_initialized ());
	
	return (dynamic_cast <T *> (const_cast <Object *> (this)) != NULL);
}

template <typename T> T & Object ::
	to_type ()
	const
{
	trace () << "to_type <" << T :: get_type_name () << "> ()" << endl;
	assert (is_initialized ());
	assert (is_type <T> ());
	
	return * dynamic_cast <T *> (const_cast <Object *> (this));
}

//	This method is only used for debugging.
//	Example usage: assert (warn <Class> (object -> Class :: is_initialized ()));
//	In case of an assertion failure, you get an informative warning in the log.
template <typename T> bool Object ::
	warn (bool initialization)
	const
{
	if (! initialization)
	{
		error () << " is not fully initialized as " << T :: get_type_name () << "!" << endl;
	}
	return initialization;
}

#include "tsl.hpp"

template Character & Object :: to_type <Character> () const;
template Container & Object :: to_type <Container> () const;
template Dead_State & Object :: to_type <Dead_State> () const;
template Fight_State & Object :: to_type <Fight_State> () const;
template Peace_State & Object :: to_type <Peace_State> () const;
template Sector & Object :: to_type <Sector> () const;
template Set <Entity> & Object :: to_type <Set <Entity> > () const;
template Set <Sector> & Object :: to_type <Set <Sector> > () const;
template Set <Sound> & Object :: to_type <Set <Sound> > () const;
template Set <State <Character> > & Object :: to_type <Set <State <Character> > > () const;
template State_Machine <Character> & Object :: to_type <State_Machine <Character> > () const;
template NPC & Object :: to_type <NPC> () const;
template Sound & Object :: to_type <Sound> () const;
template Weapon & Object :: to_type <Weapon> () const;

template bool Object :: is_type <Container> () const;
template bool Object :: is_type <NPC> () const;
template bool Object :: is_type <Sector> () const;
template bool Object :: is_type <Set <Entity> > () const;
template bool Object :: is_type <Set <Sector> > () const;
template bool Object :: is_type <Set <Sound> > () const;
template bool Object :: is_type <Set <State <Character> > > () const;
template bool Object :: is_type <Weapon> () const;

template bool Object :: warn <Battle_Engine> (bool initialization) const;
template bool Object :: warn <Character> (bool initialization) const;
template bool Object :: warn <Entity> (bool initialization) const;
template bool Object :: warn <GUI_Engine> (bool initialization) const;
template bool Object :: warn <Input_Engine> (bool initialization) const;
template bool Object :: warn <NPC> (bool initialization) const;
template bool Object :: warn <Sector> (bool initialization) const;
template bool Object :: warn <Set <Entity> > (bool initialization) const;
template bool Object :: warn <Set <Sector> > (bool initialization) const;
template bool Object :: warn <Set <Sound> > (bool initialization) const;
template bool Object :: warn <Set <State <Character> > > (bool initialization) const;
template bool Object :: warn <State <Character> > (bool initialization) const;
template bool Object :: warn <State_Machine <Character> > (bool initialization) const;
template bool Object :: warn <TSL> (bool initialization) const;
template bool Object :: warn <Weapon> (bool initialization) const;

