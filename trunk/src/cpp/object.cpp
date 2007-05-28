#include "log.hpp"
#include "tracker.hpp"

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

//  constructor
Object ::
	Object (string new_name) :
	string (new_name),
	me (* this),
	my (me + "'s "),
	destructing (false)
{
	Engines :: Log :: trace (me, Object :: get_class_name (), "", new_name);
	assert (! new_name . empty ());

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

	forget_dependencies ();

	Engines :: Log :: log (me) << "Farewell..." << endl;
}

//	virtual
bool Object ::
	is_initialized ()
	const
{
	//	Engines :: Log :: trace (me, Object :: get_class_name (), "is_initialized");
	//	checks for empty string
	assert (! empty ());
	
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
	Engines :: Log :: trace (me, Object :: get_class_name (), "remember", dependency);
	//	assert (Object :: is_initialized ());

	bool check = dependencies . insert (& dependency) . second;
	assert (check);
}

void Object ::
	forget (const Object & dependency, bool stay)
{
	Engines :: Log :: trace (me, Object :: get_class_name (), "forget", dependency, bool_to_string (stay));
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
		Engines :: Log :: log (me) << "I have another dependency and will not self-delete." << endl;
		return;
	}

	if (stay)
	{
		Engines :: Log :: log (me) << "I have no more dependencies, but I'm forced to stay." << endl;
		return;
	}

	Engines :: Log :: log (me) << "I have no more dependencies and will self-delete." << endl;
	delete this;
}

//	virtual
void Object ::
	drop (Object & t, bool stay)
{
	Engines :: Log :: trace (me, Object :: get_class_name (), "drop", t, bool_to_string (stay));
	assert (is_initialized ());

	Engines :: Log :: error (me) << "Plain object call to drop '" << t << "'." << endl;
	abort ();
}

void Object ::
	forget_dependencies ()
{
	Engines :: Log :: trace (me, Object :: get_class_name (), "forget_dependencies");
	assert (is_initialized ());

	destructing = true;

	for (set <const Object *> :: const_iterator i = dependencies . begin ();
		i != dependencies . end (); i++)
	{
		Engines :: Log :: log (me) << "Dependency to forget: '" << * * i << "'" << endl;
	}

	const Object * last = NULL;
	for (set <const Object *> :: const_iterator i = dependencies . begin ();
		i != dependencies . end (); i = dependencies . begin ())
	{
		assert (* i != last);
		//	The dropping object should call 'forget (...)' for me.
		const_cast <Object *> (* i) -> drop (me);

		last = * i;
	}
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

	return (dynamic_cast <T *> (const_cast <Object *> (this)) != NULL);
}

template <class T> T & Object ::
	to_type ()
	const
{
//	Engines :: Log :: trace (me, Object :: get_class_name (), "to_type", "<" + T :: get_class_name () + ">");
	assert (is_initialized ());
	assert (is_type <T> ());

	return dynamic_cast <T &> (const_cast <Object &> (me));
}

//	to avert linking errors:
#include "alive_state.hpp"
#include "audio_engine.hpp"
#include "chat_state.hpp"
#include "conversation_message.hpp"
#include "dead_state.hpp"
#include "fight_state.hpp"
#include "game.hpp"
#include "gui_engine.hpp"
#include "input_engine.hpp"
#include "menu_state.hpp"
#include "play_state.hpp"
#include "quit_state.hpp"
#include "world.hpp"

template bool Object ::
	is_type <Algorithms :: Dead_State> () const;
template bool Object ::
	is_type <Algorithms :: Play_State> () const;
template bool Object ::
	is_type <Algorithms :: Quit_State> () const;
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
	is_type <Messages :: Conversation_Message> () const;
template bool Object ::
	is_type <Model> () const;
template bool Object ::
	is_type <Observer <Algorithms :: Play_State> > () const;
template bool Object ::
	is_type <Observer <Engines :: Log> > () const;
template bool Object ::
	is_type <Observer <GUI> > () const;
template bool Object ::
	is_type <Observer <Items :: Character> > () const;
template bool Object ::
	is_type <Set <Items :: Item> > () const;
template bool Object ::
	is_type <Set <Tile> > () const;
template bool Object ::
	is_type <Set <Sound> > () const;
template bool Object ::
	is_type <State_Machines :: Data_State_Machine <Tile> > () const;
template bool Object ::
	is_type <Movable_Model> () const;
template bool Object ::
	is_type <Tile> () const;

template Algorithms :: Alive_State & Object ::
	to_type <Algorithms :: Alive_State> () const;
template Algorithms :: Dead_State & Object ::
	to_type <Algorithms :: Dead_State> () const;
template Algorithms :: Fight_State & Object ::
	to_type <Algorithms :: Fight_State> () const;
template Algorithms :: Menu_State & Object ::
	to_type <Algorithms :: Menu_State> () const;
template Algorithms :: Play_State & Object ::
	to_type <Algorithms :: Play_State> () const;
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
template Messages :: Conversation_Message & Object ::
	to_type <Messages :: Conversation_Message> () const;
template Model & Object ::
	to_type <Model> () const;
template Observer <Algorithms :: Play_State> & Object ::
	to_type <Observer <Algorithms :: Play_State> > () const;
template Observer <Engines :: Log> & Object ::
	to_type <Observer <Engines :: Log> > () const;
template Observer <GUI> & Object ::
	to_type <Observer <GUI> > () const;
template Observer <Items :: Character> & Object ::
	to_type <Observer <Items :: Character> > () const;
template Set <GUI> & Object ::
	to_type <Set <GUI> > () const;
template Set <Items :: Item> & Object ::
	to_type <Set <Items :: Item> > () const;
template Set <Tile> & Object ::
	to_type <Set <Tile> > () const;
template Set <Sound> & Object ::
	to_type <Set <Sound> > () const;
template State_Machines :: Data_State_Machine <Tile> & Object ::
	to_type <State_Machines :: Data_State_Machine <Tile> > () const;
template Movable_Model & Object ::
	to_type <Movable_Model> () const;
template Object & Object ::
	to_type <Object> () const;
template Sound & Object ::
	to_type <Sound> () const;
template Tile & Object ::
	to_type <Tile> () const;
template World & Object ::
	to_type <World> () const;
