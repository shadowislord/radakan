#include "log.hpp"
#include "set.hpp"

using namespace std;
using namespace TSL;

string TSL ::
	bool_to_string (const bool & value)
{
	if (value)
	{
		return "true";
	}
	return "false";
}

string TSL ::
	to_string (const float & value)
{
	ostringstream oss;
	oss << value;	//	insert int into stream
	return oss . str ();
}

string TSL ::
	to_string (const Ogre :: Vector3 & vector)
{
	return "(" + to_string (vector . x) + ", " + to_string (vector . y) + ", "
												+ to_string (vector . z) + ")";
}

//	static
float TSL ::
	to_float (const string & value)
{
	istringstream iss (value);
	float result;
	iss >> result;
	return result;
}

//	static
const string Object ::
	get_class_name ()
{
	//	Do not turn this into a static data member,
	//	that'd cause problems for template (sub)classes.

	return "Object";
}

#ifdef TSL_DEBUG
	//	static
	unsigned long int Object ::
		turn (0);

	//	static
	Set <Object> Object ::
		objects ("objects", Set <Object> :: unlimited);
	
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
	string (new_name),
	me (* this),
	my (me + "'s ")
{
	Log :: trace <Object> (me, "", new_name);
	assert (! new_name . empty ());

	#ifdef TSL_DEBUG
		if (* this != objects)
		{
			objects . add (me);
			assert (has_dependency (Object :: get_class_name ()));
			assert (does_depend (objects, Object :: get_class_name ()));
		}
	#endif

	assert (Object :: is_initialized ());
}

//  destructor
Object ::
	~Object ()
{
	Log :: trace <Object> (me, "~");
	assert (is_initialized ());

	#ifdef TSL_DEBUG
		if (* this != objects)
		{
			objects . drop (* this);
		}
	#endif

	Log :: log (me) << "Farewell..." << endl;
}

//	virtual
bool Object ::
	is_initialized ()
	const
{
	//	Log :: trace <Object> (me, "is_initialized");
	//	checks for empty string
	assert (! empty ());
	
	#ifdef TSL_DEBUG
		if (* this != objects)
		{
			//	This slows down the game to much.
			//	assert (is_in (objects, Object :: get_class_name ()));
		}
	#endif
	
	return true;
}

bool Object ::
	has_dependency (const string context)
	const
{
	Log :: trace <Object> (me, "has_dependency", context);
	//	assert (is_initialized ());

	return (dependencies . find (context) != dependencies . end ());
}

//	private
bool Object ::
	does_depend (const Object & candidate, const string context)
	const
{
	Log :: trace <Object> (me, "does_depend", candidate, context);
	//	assert (is_initialized ());
	assert (has_dependency (context));
	assert (* dependencies . find (context) -> second == candidate);

	return true;
}

void Object ::
	remember (const Object & dependency, const string context)
{
	Log :: trace <Object> (me, "remember", dependency, context);
	//	assert (Object :: is_initialized ());
	assert (! has_dependency (context));

	dependencies [context] = & dependency;
}

void Object ::
	forget (const Object & dependency, const string context, bool stay)
{
	Log :: trace <Object> (me, "forget", dependency, context, bool_to_string (stay));
	assert (is_initialized ());
	assert (does_depend (dependency, context));

	dependencies . erase (context);

	#ifdef TSL_DEBUG
		if (context == Object :: get_class_name ())
		{
			assert (dependency == objects);
			Log :: log (me) << "I will not self-delete twice." << endl;
	
			return;
		}
	#endif

	//	Am I an orphan? 'objects' (debug only) is ignored here.
	if (dependencies . size () == (debugging ? 1 : 0))
	{
		if (stay)
		{
			Log :: log (me) << "I have no more dependencies, but I'm forced to stay." << endl;
		}
		else
		{
			Log :: log (me) << "I have no more dependencies and will self-delete." << endl;
			delete this;
		}
	}
	else
	{
		Log :: log (me) << "I have another dependency and will not self-delete." << endl;
	}
}

template <class T> bool Object ::
	is_type ()
	const
{
//	Log :: trace <Object> (me, "is_type", "<" + T :: get_class_name () + ">");
	assert (is_initialized ());

	return (dynamic_cast <T *> (const_cast <Object *> (this)) != NULL);
}

template <class T> T & Object ::
	to_type ()
	const
{
//	Log :: trace <Object> (me, "to_type", "<" + T :: get_class_name () + ">");
	assert (is_initialized ());
	assert (is_type <T> ());

	return dynamic_cast <T &> (const_cast <Object &> (me));
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
#include "speech_state.hpp"
#include "world.hpp"

template bool Object :: is_type <Container> () const;
template bool Object :: is_type <Data_State_Machine <Tile> > () const;
template bool Object :: is_type <Dead_State> () const;
template bool Object :: is_type <Set <Item> > () const;
template bool Object :: is_type <Set <Tile> > () const;
template bool Object :: is_type <Set <Sound> > () const;
template bool Object :: is_type <Game> () const;
template bool Object :: is_type <Item> () const;
template bool Object :: is_type <Movable_Model> () const;
template bool Object :: is_type <NPC> () const;
template bool Object :: is_type <Quit_State> () const;
template bool Object :: is_type <Speech_State> () const;
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
template Movable_Model & Object :: to_type <Movable_Model> () const;
template Object & Object :: to_type <Object> () const;
template Sound & Object :: to_type <Sound> () const;
template Tile & Object :: to_type <Tile> () const;
template Weapon & Object :: to_type <Weapon> () const;
template World & Object :: to_type <World> () const;
