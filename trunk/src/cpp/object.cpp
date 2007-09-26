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
Reference <const Object> Object ::
	update (new Object ("update (static)"));

//	static
Reference <const Object> Object ::
	terminate (new Object ("terminate (static)"));

//  constructor
Object ::
	Object (string new_name) :
	name (new_name),
	dependencies (new set <const Reference_Base *>),
	destructing (false),
	me (this)
{
	Engines :: Log :: trace (me, Object :: get_class_name (), "", new_name);
	assert (! name . empty ());

	if (Engines :: Tracker :: is_instantiated ())
	{
		bool check = Engines :: Tracker :: get () -> add (me);
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

	for (set <const Reference_Base *> :: const_iterator i = dependencies -> begin ();
		i != dependencies -> end (); i = dependencies -> begin ())
	{
		Engines :: Log :: log (me) << "Dependency to be dropped by: '" << (* i) -> get_name () << "'" << endl;
		
		delete (* i);
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

template <class T> Reference <T> Object ::
	to_type ()
{
//	Engines :: Log :: trace (me, Object :: get_class_name (), "to_type", "<" + T :: get_class_name () + ">");
	assert (is_initialized ());
	assert (is_type <T> ());

	return Reference <T> (dynamic_cast <T *> (this));
}

template <class T> Reference <const T> Object ::
	to_type ()
	const
{
//	Engines :: Log :: trace (me, Object :: get_class_name (), "to_type", "<" + T :: get_class_name () + ">");
	assert (is_initialized ());
	assert (is_type <T> ());

	return Reference <const T> (dynamic_cast <const T *> (this));
}

void  Object ::
	register_reference (const Reference_Base & reference)
	const
{
	Engines :: Log :: trace (me, Object :: get_class_name (), "register_reference", reference . get_name ());
	
	assert (is_initialized ());
	
	assert (! does_depend (reference));

	bool check = dependencies -> insert (& reference) . second;

	assert (check);
	assert (does_depend (reference));
}

void  Object ::
	unregister_reference (const Reference_Base & reference)
	const
{
	Engines :: Log :: trace (me, Object :: get_class_name (), "unregister_reference", reference . get_name ());
	
	assert (is_initialized ());

	dependencies -> erase (& reference);

	if (destructing)
	{
		Engines :: Log :: log (me) << "I'm already destructing." << endl;
		return;
	}

	unsigned int self_destruct_criterion = 0;
	self_destruct_criterion ++;	//	'me' should not forbid to destruct.
	if (does_depend (Engines :: Tracker :: get ()))
	{
		self_destruct_criterion = 1;
	}
	assert (dependencies -> size () >= self_destruct_criterion);

	if (dependencies -> size () > self_destruct_criterion)
	{
		Engines :: Log :: log (me) << "I have another dependency and will not self-destruct." << endl;
		return;
	}

	Engines :: Log :: log (me) << "I have no more dependencies and will self-destruct." << endl;
	delete this;
}

//	to avert linking errors:
#include "alive_state.hpp"
#include "audio_engine.hpp"
#include "battle_message.hpp"
#include "character.hpp"
#include "chat_state.hpp"
#include "conversation_message.hpp"
#include "container.hpp"
#include "fight_state.hpp"
#include "game.hpp"
#include "gui.hpp"
#include "gui_engine.hpp"
#include "input_engine.hpp"
#include "menu_state.hpp"
#include "model.hpp"
#include "movable_model.hpp"
#include "npc.hpp"
#include "play_state.hpp"
#include "tile.hpp"
#include "thought.hpp"
#include "world.hpp"
#include "weapon.hpp"

//	template bool Object ::
//		is_type <Strategies :: Strategy> () const;
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
//	template bool Object ::
//		is_type <Engines :: Game> () const;
//	template bool Object ::
//		is_type <Engines :: Log> () const;
template bool Object ::
	is_type <Items :: Container> () const;
template bool Object ::
	is_type <Items :: Item> () const;
template bool Object ::
	is_type <Items :: NPC> () const;
//	template bool Object ::
//		is_type <Items :: Weapon> () const;
template bool Object ::
	is_type <Messages :: Battle_Message> () const;
template bool Object ::
	is_type <const Messages :: Conversation_Message> () const;
template bool Object ::
	is_type <Messages :: Conversation_Message> () const;
//	template bool Object ::
//		is_type <Model> () const;
//	template bool Object ::
//	is_type <Observer <Strategies :: Play_State> > () const;
//	template bool Object ::
//		is_type <Observer <Engines :: Log> > () const;
//	template bool Object ::
//		is_type <Observer <GUI> > () const;
//	template bool Object ::
//		is_type <Observer <Items :: Character> > () const;
template bool Object ::
	is_type <Set <Messages :: Conversation_Message> > () const;
//	template bool Object ::
//		is_type <Set <Items :: Item> > () const;
//	template bool Object ::
//		is_type <Set <Tile> > () const;
//	template bool Object ::
//		is_type <Set <Sound_Sample> > () const;
//	template bool Object ::
//		is_type <Movable_Model> () const;
//	template bool Object ::
//		is_type <Sound_Sample> () const;
//	template bool Object ::
//		is_type <State_Machine <Tile> > () const;
//	template bool Object ::
//		is_type <Tile> () const;
//	template bool Object ::
//		is_type <Thought> () const;

template Reference <Strategies :: Strategy> Object ::
	to_type <Strategies :: Strategy> ();
template Reference <Strategies :: Alive_State> Object ::
	to_type <Strategies :: Alive_State> ();
template Reference <Strategies :: Fight_State> Object ::
	to_type <Strategies :: Fight_State> ();
template Reference <Strategies :: Menu_State> Object ::
	to_type <Strategies :: Menu_State> ();
template Reference <Strategies :: Play_State> Object ::
	to_type <Strategies :: Play_State> ();
template Reference <Strategies :: Strategy_State_Machine> Object ::
	to_type <Strategies :: Strategy_State_Machine> ();
//	template Reference <Engines :: Game> Object ::
//		to_type <Engines :: Game> ();
//	template Reference <Engines :: GUI_Engine> Object ::
//		to_type <Engines :: GUI_Engine> ();
//	template Reference <Engines :: Log> Object ::
//		to_type <Engines :: Log> ();
//	template Reference <GUI> Object ::
//		to_type <GUI> ();
//	template Reference <Items :: Character> Object ::
//		to_type <Items :: Character> ();
template Reference <Items :: Container> Object ::
	to_type <Items :: Container> ();
//	template Reference <Items :: Item> Object ::
//		to_type <Items :: Item> ();
template Reference <Items :: NPC> Object ::
	to_type <Items :: NPC> ();
//	template Reference <Items :: Weapon> Object ::
//		to_type <Items :: Weapon> ();
template Reference <Messages :: Battle_Message> Object ::
	to_type <Messages :: Battle_Message> ();
template Reference <Messages :: Conversation_Message> Object ::
	to_type <Messages :: Conversation_Message> ();
//	template Reference <Model> Object ::
//		to_type <Model> ();
//	template Reference <Observer <Strategies :: Play_State> > Object ::
//		to_type <Observer <Strategies :: Play_State> > ();
//	template Reference <Observer <Engines :: Log> > Object ::
//		to_type <Observer <Engines :: Log> > ();
//	template Reference <Observer <GUI> > Object ::
//		to_type <Observer <GUI> > ();
//	template Reference <Observer <Items :: Character> > Object ::
//		to_type <Observer <Items :: Character> > ();
//	template Reference <Set <Messages :: Conversation_Message> > Object ::
//		to_type <Set <Messages :: Conversation_Message> > ();
//	template Reference <Set <GUI> > Object ::
//		to_type <Set <GUI> > ();
//	template Reference <Set <Items :: Item> > Object ::
//		to_type <Set <Items :: Item> > ();
//	template Reference <Set <Tile> > Object ::
//		to_type <Set <Tile> > ();
//	template Reference <Set <Sound_Sample> > Object ::
//		to_type <Set <Sound_Sample> > ();
template Reference <Movable_Model> Object ::
	to_type <Movable_Model> ();
//	template Reference <Object> Object ::
//		to_type <Object> ();
//	template Reference <Sound_Sample> Object ::
//		to_type <Sound_Sample> ();
//	template Reference <State_Machine <Tile> > Object ::
//		to_type <State_Machine <Tile> > ();
//	template Reference <Tile> Object ::
//		to_type <Tile> ();
//	template Reference <Thought> Object ::
//		to_type <Thought> ();

//	template Reference <const Engines :: Game> Object ::
//		to_type <Engines :: Game> () const;
//	template Reference <const Engines :: GUI_Engine> Object ::
//		to_type <Engines :: GUI_Engine> () const;
//	template Reference <const Engines :: Log> Object ::
//		to_type <Engines :: Log> () const;
//	template Reference <const GUI> Object ::
//		to_type <GUI> () const;
//	template Reference <const Items :: Character> Object ::
//		to_type <Items :: Character> () const;
//	template Reference <const Items :: Container> Object ::
//		to_type <Items :: Container> () const;
//	template Reference <const Items :: Item> Object ::
//		to_type <Items :: Item> () const;
//	template Reference <const Items :: NPC> Object ::
//		to_type <Items :: NPC> () const;
//	template Reference <const Items :: Weapon> Object ::
//		to_type <Items :: Weapon> () const;
template Reference <const Messages :: Battle_Message> Object ::
	to_type <const Messages :: Battle_Message> () const;
template Reference <const Messages :: Battle_Message> Object ::
	to_type <Messages :: Battle_Message> () const;
template Reference <const Messages :: Conversation_Message> Object ::
	to_type <const Messages :: Conversation_Message> () const;
template Reference <const Messages :: Conversation_Message> Object ::
	to_type <Messages :: Conversation_Message> () const;
//	template Reference <const Model> Object ::
//		to_type <Model> () const;
//	template Reference <const Movable_Model> Object ::
//		to_type <Movable_Model> () const;
//	template Reference <const Object> Object ::
//		to_type <Object> () const;
//	template Reference <const Observer <Strategies :: Play_State> > Object ::
//		to_type <Observer <Strategies :: Play_State> > () const;
//	template Reference <const Observer <Engines :: Log> > Object ::
//		to_type <Observer <Engines :: Log> > () const;
//	template Reference <const Observer <GUI> > Object ::
//		to_type <Observer <GUI> > () const;
//	template Reference <const Observer <Items :: Character> > Object ::
//		to_type <Observer <Items :: Character> > () const;
template Reference <const Set <Messages :: Conversation_Message> > Object ::
	to_type <Set <Messages :: Conversation_Message> > () const;
//	template Reference <const Set <GUI> > Object ::
//		to_type <Set <GUI> > () const;
//	template Reference <const Set <Items :: Item> > Object ::
//		to_type <Set <Items :: Item> > () const;
//	template Reference <const Set <Tile> > Object ::
//		to_type <Set <Tile> > () const;
//	template Reference <const Set <Sound_Sample> > Object ::
//		to_type <Set <Sound_Sample> > () const;
//	template Reference <const Strategies :: Strategy> Object ::
//		to_type <Strategies :: Strategy> () const;
//	template Reference <const Strategies :: Alive_State> Object ::
//		to_type <Strategies :: Alive_State> () const;
//	template Reference <const Strategies :: Fight_State> Object ::
//		to_type <Strategies :: Fight_State> () const;
//	template Reference <const Strategies :: Menu_State> Object ::
//		to_type <Strategies :: Menu_State> () const;
//	template Reference <const Strategies :: Play_State> Object ::
//		to_type <Strategies :: Play_State> () const;
//	template Reference <const Strategies :: Strategy_State_Machine> Object ::
//		to_type <Strategies :: Strategy_State_Machine> () const;
//	template Reference <const Sound_Sample> Object ::
//		to_type <Sound_Sample> () const;
//	template Reference <const State_Machine <Tile> > Object ::
//		to_type <State_Machine <Tile> > () const;
//	template Reference <const Tile> Object ::
//		to_type <Tile> () const;
//	template Reference <const Thought> Object ::
//		to_type <Thought> () const;
