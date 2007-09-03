#include "log.hpp"
#include "tracker.hpp"

using namespace std;
using namespace Radakan;

string Radakan ::
	bool_to_string (const bool & value)
{
	if (value)
	{
		return "true";
	}
	return "false";
}

string Radakan ::
	to_string (const float & value)
{
	ostringstream oss;
	oss << value;	//	insert int into stream
	return oss . str ();
}

string Radakan ::
	to_string (const Ogre :: Vector3 & vector)
{
	return "(" + to_string (vector . x) + ", " + to_string (vector . y) + ", "
												+ to_string (vector . z) + ")";
}

//	static
float Radakan ::
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

#ifdef RADAKAN_DEBUG
	//	static
	const bool Object ::
		debugging (true);

#else
	//	static
	const bool Object ::
		debugging (false);
#endif

//	static
const Object Object ::
	update ("update (static)");

//	static
const Object Object ::
	terminate ("terminate (static)");

bool Object ::
	operator== (const Object & other_object)
	const
{
	return (this == & other_object);
}

bool Object ::
	operator!= (const Object & other_object)
	const
{
	return ! (me == other_object);
}

//  constructor
Object ::
	Object (string new_name) :
	name (new_name),
	me (* this),
	destructing (false)
{
	Engines :: Log :: trace (me, Object :: get_class_name (), "", new_name);
	assert (! name . empty ());

	if (Engines :: Tracker :: is_instantiated ())
	{
		bool check = Engines :: Tracker :: get () . add (me);
		assert (check);
		assert (does_depend (Engines :: Tracker :: get ()));
	}

	assert (Object :: is_initialized ());
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

	destructing = true;

	for (set <const Object *> :: const_iterator i = dependencies . begin ();
		i != dependencies . end (); i ++)
	{
		Engines :: Log :: log (me) << "Dependency to be dropped by: '" << (* i) -> name << "'" << endl;
	}

	const Object * last = NULL;
	for (set <const Object *> :: const_iterator i = dependencies . begin ();
		i != dependencies . end (); i = dependencies . begin ())
	{
		assert (* i != last);
		//	The dropping object should call 'forget (...)' for me.
		
#ifdef RADAKAN_WINDOWS
		// I don't really know how to handle this
		// alternatively.
		(Object *) (* i) -> drop (me);
#else
		const_cast <Object *> (* i) -> drop (me);
#endif

		last = * i;
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
	
	return true;
}

//	private
bool Object ::
	does_depend (const Object & candidate)
	const
{
	//	Engines :: Log :: trace (me, Object :: get_class_name (), "does_depend", candidate);
	//	assert (Object :: is_initialized ());

	return (dependencies . find (& candidate) != dependencies . end ());
}

void Object ::
	remember (Object & dependency)
{
	Engines :: Log :: trace (me, Object :: get_class_name (), "remember", dependency . name);
	//	assert (Object :: is_initialized ());

	bool check = dependencies . insert (& dependency) . second;
	assert (check);
}

void Object ::
	forget (const Object & dependency, bool stay)
{
	Engines :: Log :: trace (me, Object :: get_class_name (), "forget", dependency . name, bool_to_string (stay));
	assert (is_initialized ());
	assert (does_depend (dependency));

	dependencies . erase (& dependency);

	if (destructing)
	{
		Engines :: Log :: log (me) << "I'm already destructing." << endl;
		return;
	}

	unsigned int self_destruct_criterion = 0;
	if (does_depend (Engines :: Tracker :: get ()))
	{
		self_destruct_criterion = 1;
	}
	assert (dependencies . size () >= self_destruct_criterion);

	if (dependencies . size () > self_destruct_criterion)
	{
		Engines :: Log :: log (me) << "I have another dependency and will not self-destruct." << endl;
		return;
	}

	if (stay)
	{
		Engines :: Log :: log (me) << "I have no more dependencies, but I'm forced to stay." << endl;
		return;
	}

	Engines :: Log :: log (me) << "I have no more dependencies and will self-destruct." << endl;
	delete this;
}

//	virtual
void Object ::
	drop (Object & t, bool stay)
{
	Engines :: Log :: trace (me, Object :: get_class_name (), "drop", t . name, bool_to_string (stay));
	assert (is_initialized ());

	Engines :: Log :: error (me) << "Plain object call to drop '" << t . name << "'." << endl;
	abort ();
}

const bool & Object ::
	is_destructing ()
	const
{
	//	Being initialized isn't necessairy here.

	return destructing;
}

template <class T> bool Object ::
	is_type ()
	const
{
//	Engines :: Log :: trace (me, Object :: get_class_name (), "is_type", "<" + T :: get_class_name () + ">");
	assert (is_initialized ());

	return (dynamic_cast <const T *> (this) != NULL);
}

template <class T> T & Object ::
	to_type ()
	const
{
//	Engines :: Log :: trace (me, Object :: get_class_name (), "to_type", "<" + T :: get_class_name () + ">");
	assert (is_initialized ());
	assert (is_type <T> ());

	Engines :: Log :: log (me) << "RTTI is not available for " << me . name << ". I'll fall back to an unsafe cast." << endl;
	return dynamic_cast <T &> (me);
}

//	to avert linking errors:
#include "alive_state.hpp"
#include "audio_engine.hpp"
#include "battle_message.hpp"
#include "chat_state.hpp"
#include "conversation_message.hpp"
#include "fight_state.hpp"
#include "game.hpp"
#include "gui_engine.hpp"
#include "input_engine.hpp"
#include "menu_state.hpp"
#include "play_state.hpp"
#include "world.hpp"

template bool Object ::
	is_type <Strategies :: Strategy> () const;
template bool Object ::
	is_type <Strategies :: Alive_State> () const;
template bool Object ::
	is_type <Strategies :: Chat_State> () const;
template bool Object ::
	is_type <Strategies :: Fight_State> () const;
template bool Object ::
	is_type <Strategies :: Play_State> () const;
template bool Object ::
	is_type <Strategies :: Strategy_State_Machine> () const;
template bool Object ::
	is_type <Engines :: Game> () const;
template bool Object ::
	is_type <Engines :: Log> () const;
template bool Object ::
	is_type <Items :: Container> () const;
template bool Object ::
	is_type <Items :: Item> () const;
template bool Object ::
	is_type <Items :: NPC> () const;
template bool Object ::
	is_type <Items :: Weapon> () const;
template bool Object ::
	is_type <Messages :: Battle_Message> () const;
template bool Object ::
	is_type <const Messages :: Conversation_Message> () const;
template bool Object ::
	is_type <Messages :: Conversation_Message> () const;
template bool Object ::
	is_type <Model> () const;
template bool Object ::
	is_type <Observer <Strategies :: Play_State> > () const;
template bool Object ::
	is_type <Observer <Engines :: Log> > () const;
template bool Object ::
	is_type <Observer <GUI> > () const;
template bool Object ::
	is_type <Observer <Items :: Character> > () const;
template bool Object ::
	is_type <Set <Messages :: Conversation_Message> > () const;
template bool Object ::
	is_type <Set <Items :: Item> > () const;
template bool Object ::
	is_type <Set <Tile> > () const;
template bool Object ::
	is_type <Set <Sound_Sample> > () const;
template bool Object ::
	is_type <Movable_Model> () const;
template bool Object ::
	is_type <Sound_Sample> () const;
template bool Object ::
	is_type <State_Machine <Tile> > () const;
template bool Object ::
	is_type <Tile> () const;

template Strategies :: Strategy & Object ::
	to_type <Strategies :: Strategy> () const;
template Strategies :: Alive_State & Object ::
	to_type <Strategies :: Alive_State> () const;
template Strategies :: Fight_State & Object ::
	to_type <Strategies :: Fight_State> () const;
template Strategies :: Menu_State & Object ::
	to_type <Strategies :: Menu_State> () const;
template Strategies :: Play_State & Object ::
	to_type <Strategies :: Play_State> () const;
template Strategies :: Strategy_State_Machine & Object ::
	to_type <Strategies :: Strategy_State_Machine> () const;
template Engines :: Game & Object ::
	to_type <Engines :: Game> () const;
template Engines :: GUI_Engine & Object ::
	to_type <Engines :: GUI_Engine> () const;
template Engines :: Log & Object ::
	to_type <Engines :: Log> () const;
template GUI & Object ::
	to_type <GUI> () const;
template Items :: Character & Object ::
	to_type <Items :: Character> () const;
template Items :: Container & Object ::
	to_type <Items :: Container> () const;
template Items :: Item & Object ::
	to_type <Items :: Item> () const;
template Items :: NPC & Object ::
	to_type <Items :: NPC> () const;
template Items :: Weapon & Object ::
	to_type <Items :: Weapon> () const;
template Messages :: Battle_Message & Object ::
	to_type <Messages :: Battle_Message> () const;
template const Messages :: Conversation_Message & Object ::
	to_type <const Messages :: Conversation_Message> () const;
template Messages :: Conversation_Message & Object ::
	to_type <Messages :: Conversation_Message> () const;
template Model & Object ::
	to_type <Model> () const;
template Observer <Strategies :: Play_State> & Object ::
	to_type <Observer <Strategies :: Play_State> > () const;
template Observer <Engines :: Log> & Object ::
	to_type <Observer <Engines :: Log> > () const;
template Observer <GUI> & Object ::
	to_type <Observer <GUI> > () const;
template Observer <Items :: Character> & Object ::
	to_type <Observer <Items :: Character> > () const;
template Set <Messages :: Conversation_Message> & Object ::
	to_type <Set <Messages :: Conversation_Message> > () const;
template Set <GUI> & Object ::
	to_type <Set <GUI> > () const;
template Set <Items :: Item> & Object ::
	to_type <Set <Items :: Item> > () const;
template Set <Tile> & Object ::
	to_type <Set <Tile> > () const;
template Set <Sound_Sample> & Object ::
	to_type <Set <Sound_Sample> > () const;
template Movable_Model & Object ::
	to_type <Movable_Model> () const;
template Object & Object ::
	to_type <Object> () const;
template Sound_Sample & Object ::
	to_type <Sound_Sample> () const;
template State_Machine <Tile> & Object ::
	to_type <State_Machine <Tile> > () const;
template Tile & Object ::
	to_type <Tile> () const;
