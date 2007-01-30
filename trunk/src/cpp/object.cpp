#include <fstream>
#include <sstream>
#include "object.hpp"

using namespace std;
using namespace tsl;

unsigned long int Object :: turn = 0;

//  constructor
Object ::
	Object (string new_name) :
	string (new_name),
	parent (NULL)
{
	assert (! new_name . empty ());
	trace () << "Object (" << new_name << ")" << endl;
	
	#ifdef TSL_DEBUG
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
	trace () << "~" << get_class_name () << " ()" << endl;
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
	get_class_name ()
{
	return "Object";
}

//	virtual
ostream & Object ::
	debug ()
	const
{
	#ifdef TSL_DEBUG
		return print ();
	#else
		return cout;
	#endif
}

ostream & Object ::
	trace ()
	const
{
	#ifdef TSL_TRACE
		return print ();
	#else
		return cout;
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

ostream & Object ::
	print () const
{
	return cout << turn << " - " << * this << ": ";
}

template <class T> bool Object ::
	is_type ()
	const
{
//	trace () << "is_type <" << T :: get_class_name () << "> ()" << endl;
	assert (is_initialized ());
	
	return (dynamic_cast <T *> (const_cast <Object *> (this)) != NULL);
}

template <class T> T & Object ::
	to_type ()
	const
{
//	trace () << "to_type <" << T :: get_class_name () << "> ()" << endl;
	assert (is_initialized ());
	assert (is_type <T> ());
	
//	return * dynamic_cast <T *> (const_cast <Object *> (this));
	return dynamic_cast <T &> (const_cast <Object &> (* this));
}

template <class T> bool Object ::
	warn (bool initialization)
	const
{
	if (! initialization)
	{
		error () << " is not fully initialized as " << T :: get_class_name () << "!" << endl;
	}
	return initialization;
}

//	to avert linking errors:
#include "tsl.hpp"
#include "dead_state.hpp"
#include "fight_state.hpp"
#include "menu_state.hpp"
#include "peace_state.hpp"
#include "play_state.hpp"
#include "quit_state.hpp"

template bool Object :: is_type <Container> () const;
template bool Object :: is_type <NPC> () const;
template bool Object :: is_type <Sector> () const;
template bool Object :: is_type <Data_State_Machine <Sector> > () const;
template bool Object :: is_type <Disjoint_Set <Item> > () const;
template bool Object :: is_type <Disjoint_Set <Sector> > () const;
template bool Object :: is_type <Disjoint_Set <Sound> > () const;
template bool Object :: is_type <Quit_State> () const;
template bool Object :: is_type <TSL> () const;
template bool Object :: is_type <Weapon> () const;

template Character & Object :: to_type <Character> () const;
template Container & Object :: to_type <Container> () const;
template Dead_State & Object :: to_type <Dead_State> () const;
template Fight_State & Object :: to_type <Fight_State> () const;
template GUI & Object :: to_type <GUI> () const;
template GUI_Engine & Object :: to_type <GUI_Engine> () const;
template Item & Object :: to_type <Item> () const;
template NPC & Object :: to_type <NPC> () const;
template Menu_State & Object :: to_type <Menu_State> () const;
template Peace_State & Object :: to_type <Peace_State> () const;
template Play_State & Object :: to_type <Play_State> () const;
template Sector & Object :: to_type <Sector> () const;
template Data_State_Machine <Sector> & Object :: to_type <Data_State_Machine <Sector> > () const;
template Disjoint_Set <GUI> & Object :: to_type <Disjoint_Set <GUI> > () const;
template Disjoint_Set <Item> & Object :: to_type <Disjoint_Set <Item> > () const;
template Disjoint_Set <Sector> & Object :: to_type <Disjoint_Set <Sector> > () const;
template Disjoint_Set <Sound> & Object :: to_type <Disjoint_Set <Sound> > () const;
template Sound & Object :: to_type <Sound> () const;
template TSL & Object :: to_type <TSL> () const;
template Weapon & Object :: to_type <Weapon> () const;

template bool Object :: warn <Audio_Engine> (bool initialization) const;
template bool Object :: warn <Algorithm <NPC> > (bool initialization) const;
template bool Object :: warn <Algorithm <TSL> > (bool initialization) const;
template bool Object :: warn <Algorithm_State_Machine <NPC> > (bool initialization) const;
template bool Object :: warn <Algorithm_State_Machine <TSL> > (bool initialization) const;
template bool Object :: warn <Battle_Engine> (bool initialization) const;
template bool Object :: warn <Character> (bool initialization) const;
template bool Object :: warn <Data_State_Machine <GUI> > (bool initialization) const;
template bool Object :: warn <Data_State_Machine <Sector> > (bool initialization) const;
template bool Object :: warn <Disjoint_Set <GUI> > (bool initialization) const;
template bool Object :: warn <Disjoint_Set <Item> > (bool initialization) const;
template bool Object :: warn <Disjoint_Set <Sector> > (bool initialization) const;
template bool Object :: warn <Disjoint_Set <Sound> > (bool initialization) const;
template bool Object :: warn <GUI> (bool initialization) const;
template bool Object :: warn <GUI_Engine> (bool initialization) const;
template bool Object :: warn <GUI_Listener> (bool initialization) const;
template bool Object :: warn <Input_Engine> (bool initialization) const;
template bool Object :: warn <Item> (bool initialization) const;
template bool Object :: warn <Multislot <Container> > (bool initialization) const;
template bool Object :: warn <Multislot <Item> > (bool initialization) const;
template bool Object :: warn <NPC> (bool initialization) const;
template bool Object :: warn <Representation> (bool initialization) const;
template bool Object :: warn <Sector> (bool initialization) const;
template bool Object :: warn <Singleton <Audio_Engine> > (bool initialization) const;
template bool Object :: warn <Singleton <Battle_Engine> > (bool initialization) const;
template bool Object :: warn <Singleton <GUI_Engine> > (bool initialization) const;
template bool Object :: warn <Singleton <Dead_State> > (bool initialization) const;
template bool Object :: warn <Singleton <Fight_State> > (bool initialization) const;
template bool Object :: warn <Singleton <Input_Engine> > (bool initialization) const;
template bool Object :: warn <Singleton <Menu_State> > (bool initialization) const;
template bool Object :: warn <Singleton <Peace_State> > (bool initialization) const;
template bool Object :: warn <Singleton <Play_State> > (bool initialization) const;
template bool Object :: warn <Singleton <Quit_State> > (bool initialization) const;
template bool Object :: warn <Singleton <Player> > (bool initialization) const;
template bool Object :: warn <Singleton <TSL> > (bool initialization) const;
template bool Object :: warn <State_Machine <Algorithm <NPC> > > (bool initialization) const;
template bool Object :: warn <State_Machine <Algorithm <TSL> > > (bool initialization) const;
template bool Object :: warn <State_Machine <GUI> > (bool initialization) const;
template bool Object :: warn <State_Machine <Sector> > (bool initialization) const;
template bool Object :: warn <TSL> (bool initialization) const;
template bool Object :: warn <Weapon> (bool initialization) const;
