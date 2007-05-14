#include "log.hpp"
#include "set.hpp"

using namespace std;
using namespace TSL;

//	static
const string Object ::
	class_name ("Object");

#ifdef TSL_DEBUG
	//	static
	unsigned long int Object ::
		turn (0);

/*	//	static
	Set <Object> Object ::
		objects (Set <Object> :: unlimited, "objects");*/
	
	//	static
	const bool Object ::
		debugging (true);

#else
	//	static
	const bool Object ::
		debugging (false);
#endif

//  constructor
Object ::
	Object (string new_name) :
	string (new_name)
{
	log (debugging) << class_name << " (" << new_name << ")" << endl;
	assert (! new_name . empty ());

/*	#ifdef TSL_DEBUG
		if (string :: data () != "objects")
		{
			objects . add (* this);
		}
	#endif*/

	assert (Object :: is_initialized ());
}

//  destructor
Object ::
	~Object ()
{
	log (debugging) << "~" << class_name << " ()" << endl;
	assert (is_initialized ());
}

//	virtual
bool Object ::
	is_initialized ()
	const
{
	//	checks for empty string
	assert (! empty ());
	/*#ifdef TSL_DEBUG
		assert (objects . find (const_cast <Object *> (this)) != objects . end ());
	#endif*/
	return true;
}

ostream & Object ::
	error ()
	const
{
	if (Log :: is_instantiated ())
	{
		return Log :: get () . error (* this);
	}

	return log (true) << "Error: ";
}

ostream & Object ::
	log (bool condition)
	const
{
	if (Log :: is_instantiated ())
	{
		return Log :: get () . log (* this, condition);
	}

	return cout;
}

void Object ::
	show (string message, bool condition)
	const
{
	if (Log :: is_instantiated ())
	{
		Log :: get () . show (message, condition);
	}

	log (condition) << "Show: " << message << endl;
}

bool Object ::
	is_orphan ()
	const
{
	assert (is_initialized ());

	return parents . empty ();
}

bool Object ::
	has_parent (const string & context)
	const
{
	assert (is_initialized ());

	return (parents . find (& context) != parents . end ());
}

bool Object ::
	is_in (const Object & set, const string & context)
	const
{
	assert (is_initialized ());

	return has_parent (context) && (parents . find (& context) -> second == & set);
}

void Object ::
	put_in (const Object & new_parent, const string & context)
{
	assert (Object :: is_initialized ());
	assert (! has_parent (context));

	parents [& context] = & new_parent;
}

void Object ::
	remove_from (const Object & old_parent, const string & context)
{
	assert (is_initialized ());
	assert (is_in (old_parent, context));

	parents . erase (& context);
}

string Object ::
	bool_to_string (const bool value)
{
	if (value)
	{
		return "true";
	}
	return "false";
}

string Object ::
	to_string (const float value)
{
	ostringstream oss;
	oss << value;	//	insert int into stream
	return oss . str ();
}

string Object ::
	to_string (const Ogre :: Vector3 & vector)
{
	return "(" + to_string (vector . x) + ", " + to_string (vector . y) + ", "
												+ to_string (vector . z) + ")";
}

//	static
float Object ::
	to_float (const string value)
{
	istringstream iss (value);
	float result;
	iss >> result;
	return result;
}

template <class T> bool Object ::
	is_type ()
	const
{
//	log (debugging) << "is_type <" << T :: class_name << "> ()" << endl;
	assert (is_initialized ());

	return (dynamic_cast <T *> (const_cast <Object *> (this)) != NULL);
}

template <class T> T & Object ::
	to_type ()
	const
{
//	log (debugging) << "to_type <" << T :: class_name << "> ()" << endl;
	assert (is_initialized ());
	assert (is_type <T> ());

//	return * dynamic_cast <T *> (const_cast <Object *> (this));
	return dynamic_cast <T &> (const_cast <Object &> (* this));
}

//	to avert linking errors:
#include "audio_engine.hpp"
#include "battle_engine.hpp"
#include "chat_state.hpp"
#include "dead_state.hpp"
#include "fight_state.hpp"
#include "game.hpp"
#include "gui_engine.hpp"
#include "input_engine.hpp"
#include "menu_state.hpp"
#include "alive_state.hpp"
#include "quit_state.hpp"
#include "world.hpp"

template bool Object :: is_type <Container> () const;
template bool Object :: is_type <Data_State_Machine <Tile> > () const;
template bool Object :: is_type <Dead_State> () const;
template bool Object :: is_type <Set <Item> > () const;
template bool Object :: is_type <Set <Tile> > () const;
template bool Object :: is_type <Set <Sound> > () const;
template bool Object :: is_type <Game> () const;
template bool Object :: is_type <Item> () const;
template bool Object :: is_type <Movable_Body> () const;
template bool Object :: is_type <NPC> () const;
template bool Object :: is_type <Quit_State> () const;
template bool Object :: is_type <Tile> () const;
template bool Object :: is_type <Weapon> () const;

template Alive_State & Object :: to_type <Alive_State> () const;
template Character & Object :: to_type <Character> () const;
template Container & Object :: to_type <Container> () const;
template Dead_State & Object :: to_type <Dead_State> () const;
template Data_State_Machine <Tile> & Object :: to_type <Data_State_Machine <Tile> > () const;
template Set <GUI> & Object :: to_type <Set <GUI> > () const;
template Set <Item> & Object :: to_type <Set <Item> > () const;
template Set <Tile> & Object :: to_type <Set <Tile> > () const;
template Set <Sound> & Object :: to_type <Set <Sound> > () const;
template Fight_State & Object :: to_type <Fight_State> () const;
template Game & Object :: to_type <Game> () const;
template GUI & Object :: to_type <GUI> () const;
template GUI_Engine & Object :: to_type <GUI_Engine> () const;
template Item & Object :: to_type <Item> () const;
template NPC & Object :: to_type <NPC> () const;
template Menu_State & Object :: to_type <Menu_State> () const;
template Movable_Body & Object :: to_type <Movable_Body> () const;
template Object & Object :: to_type <Object> () const;
template Sound & Object :: to_type <Sound> () const;
template Tile & Object :: to_type <Tile> () const;
template Weapon & Object :: to_type <Weapon> () const;
template World & Object :: to_type <World> () const;
