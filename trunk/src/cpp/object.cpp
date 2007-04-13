#include <fstream>
#include <sstream>
#include "object.hpp"

using namespace std;
using namespace tsl;

#ifdef TSL_DEBUG
	unsigned long int Object :: turn = 0;
	set <Object *> Object :: objects;
	const bool Object :: debugging = true;
#else
	const bool Object :: debugging = false;
#endif

stringstream Object :: message;
stringstream Object :: the_abyss;

//  constructor
Object ::
	Object (string new_name) :
	string (new_name),
	parent (NULL)
{
	log (debugging) << "Object (" << new_name << ")" << endl;
	assert (! new_name . empty ());

	#ifdef TSL_DEBUG
		objects . insert (this);
	#endif

	assert (Object :: is_initialized ());
}

//  destructor
Object ::
	~Object ()
{
	log (debugging) << "~" << get_class_name () << " ()" << endl;
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
	assert (! empty ());
	/*#ifdef TSL_DEBUG
		assert (objects . find (const_cast <Object *> (this)) != objects . end ());
	#endif*/
	return true;
}

//	static
string Object ::
	get_class_name ()
{
	return "Object";
}

stringstream & Object ::
	show (bool condition) const
{
	if (condition)
	{
		return message;
	}

	the_abyss . str ("");
	return the_abyss;
}

ostream & Object ::
	log (bool condition) const
{
	if (condition)
	{
		#ifdef TSL_DEBUG
			cout << endl;
			cout << "(turn " << turn << ")" << endl;
			cout << "'" << string :: data () << "' reports: " << endl;
			return cout << "\t";
		#else
			return cout;
		#endif
	}
	
	the_abyss . str ("");
	return the_abyss;
}

ostream & Object ::
	error ()
	const
{
	return log () << "An error occurred: ";
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
	log (debugging) << "put_in (" << new_parent << ")" << endl;
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
//	log (debugging) << "is_type <" << T :: get_class_name () << "> ()" << endl;
	assert (is_initialized ());

	return (dynamic_cast <T *> (const_cast <Object *> (this)) != NULL);
}

template <class T> T & Object ::
	to_type ()
	const
{
//	log (debugging) << "to_type <" << T :: get_class_name () << "> ()" << endl;
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
#include "gui_engine.hpp"
#include "input_engine.hpp"
#include "menu_state.hpp"
#include "alive_state.hpp"
#include "quit_state.hpp"
#include "tsl.hpp"
#include "world.hpp"

template bool Object :: is_type <Container> () const;
template bool Object :: is_type <Data_State_Machine <Tile> > () const;
template bool Object :: is_type <Dead_State> () const;
template bool Object :: is_type <Disjoint_Set <Item> > () const;
template bool Object :: is_type <Disjoint_Set <Tile> > () const;
template bool Object :: is_type <Disjoint_Set <Sound> > () const;
template bool Object :: is_type <Item> () const;
template bool Object :: is_type <NPC> () const;
template bool Object :: is_type <Quit_State> () const;
template bool Object :: is_type <Tile> () const;
template bool Object :: is_type <TSL> () const;
template bool Object :: is_type <Weapon> () const;

template Alive_State & Object :: to_type <Alive_State> () const;
template Character & Object :: to_type <Character> () const;
template Container & Object :: to_type <Container> () const;
template Dead_State & Object :: to_type <Dead_State> () const;
template Data_State_Machine <Tile> & Object :: to_type <Data_State_Machine <Tile> > () const;
template Disjoint_Set <GUI> & Object :: to_type <Disjoint_Set <GUI> > () const;
template Disjoint_Set <Item> & Object :: to_type <Disjoint_Set <Item> > () const;
template Disjoint_Set <Tile> & Object :: to_type <Disjoint_Set <Tile> > () const;
template Disjoint_Set <Sound> & Object :: to_type <Disjoint_Set <Sound> > () const;
template Fight_State & Object :: to_type <Fight_State> () const;
template GUI & Object :: to_type <GUI> () const;
template GUI_Engine & Object :: to_type <GUI_Engine> () const;
template Item & Object :: to_type <Item> () const;
template NPC & Object :: to_type <NPC> () const;
template Menu_State & Object :: to_type <Menu_State> () const;
template Object & Object :: to_type <Object> () const;
template Sound & Object :: to_type <Sound> () const;
template Tile & Object :: to_type <Tile> () const;
template TSL & Object :: to_type <TSL> () const;
template Weapon & Object :: to_type <Weapon> () const;
template World & Object :: to_type <World> () const;
