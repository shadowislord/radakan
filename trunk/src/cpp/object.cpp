#include "log.hpp"
#include "tracker.hpp"

using namespace std;
using namespace Radakan;

string Radakan ::
	to_string (const Ogre :: Vector3 & vector)
{
	return "(" + to_string (vector . x) + ", " + to_string (vector . y) + ", "
												+ to_string (vector . z) + ")";
}

const Reference <Object> Radakan :: update (new Object ("update (static)"));
const Reference <Object> Radakan :: terminate (new Object ("terminate (static)"));

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
	Object (string new_name) :
	name (new_name),
	dependencies (new set <const Reference_Base *>),
	status ("constructing"),
	me (this)
{
	Engines :: Log :: trace (me, Object :: get_class_name (), "", new_name);
	assert (! name . empty ());

	if (Engines :: Tracker :: is_instantiated ())
	{
		bool check = Engines :: Tracker :: get () -> add (me);
		assert (check);
		//	assert (does_depend (Engines :: Tracker :: get ()));
	}

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

	me . reset_pointee ();
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

//	virtual
bool Object ::
	is_singleton ()
	const
{
	Engines :: Log :: trace (me, Object :: get_class_name (), "is_singleton");
	
	return false;
}

template <class T> bool Object ::
	is_class ()
	const
{
//	Engines :: Log :: trace (me, Object :: get_class_name (), "is_class", "<" + T :: get_class_name () + ">");
	assert (is_initialized ());

	return (dynamic_cast <const T *> (this) != NULL);
}

template <class T> Reference <T> Object ::
	to_class ()
{
//	Engines :: Log :: trace (me, Object :: get_class_name (), "to_class", "<" + T :: get_class_name () + ">");
	assert (is_initialized ());
	assert (is_class <T> ());

	return Reference <T> (dynamic_cast <T *> (this));
}

template <class T> const Reference <T> Object ::
	to_class_const ()
	const
{
//	Engines :: Log :: trace (me, Object :: get_class_name (), "to_class", "<" + T :: get_class_name () + ">");
	assert (is_initialized ());
	assert (is_class <T> ());

	return (const_cast <Object *> (this)) -> to_class <T> ();
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

	/*for (set <const Reference_Base *> :: iterator i = dependencies -> begin (); i != dependencies -> end ();
			i ++)
	{
		Engines :: Log :: log (me) << "Dependency: " << (* i) -> get_name () << endl;
	}*/
}

void  Object ::
	unregister_reference (const Reference_Base & reference)
	const
{
	Engines :: Log :: trace (me, Object :: get_class_name (), "unregister_reference", reference . get_name ());
	
	assert (is_initialized ());

	assert (does_depend (reference));
	dependencies -> erase (& reference);

	/*for (set <const Reference_Base *> :: iterator i = dependencies -> begin (); i != dependencies -> end ();
			i ++)
	{
		Engines :: Log :: log (me) << "Dependency: " << (* i) -> get_name () << endl;
	}*/

	if (status == "constructing")
	{
		Engines :: Log :: log (me) << "I'm still constructing." << endl;
		return;
	}

	if (status == "destructing")
	{
		Engines :: Log :: log (me) << "I'm already destructing." << endl;
		return;
	}

	if (is_singleton ())
	{
		Engines :: Log :: log (me) << "I will not self-destruct, because I'm a singleton." << endl;
		return;
	}

	if (name . find ("static") != string :: npos)
	{
		Engines :: Log :: log (me) << "I will not self-destruct, because I'm a static." << endl;
		return;
	}

	unsigned int self_destruct_criterion = 0;
	self_destruct_criterion ++;	//	'me' should not forbid me to destruct.
	if (Engines :: Tracker :: is_instantiated ())
	{
		//	The 'Tracker' Singleton should not forbid me to destruct.
		self_destruct_criterion ++;
	}
	assert (dependencies -> size () >= self_destruct_criterion);

	if (dependencies -> size () == self_destruct_criterion)
	{
		Engines :: Log :: log (me) << "I will self-destruct, because I have no more dependencies." << endl;
		delete this;
	}

	Engines :: Log :: log (me) << "I will not self-destruct, because I have another dependency." << endl;
}

//	to avert linking errors:
#include "alive_state.hpp"
#include "battle_message.hpp"
#include "chat_state.hpp"
#include "conversation_message.hpp"
#include "container_item.hpp"
#include "fight_state.hpp"
#include "menu_state.hpp"
#include "movable_model.hpp"
#include "npc.hpp"
#include "play_state.hpp"

template bool Object ::
	is_class <Items :: Container_Item <Items :: Item> > () const;
template bool Object ::
	is_class <Items :: Item> () const;
template bool Object ::
	is_class <Items :: NPC> () const;
template bool Object ::
	is_class <Messages :: Battle_Message> () const;
template bool Object ::
	is_class <Messages :: Conversation_Message> () const;
template bool Object ::
	is_class <Set <Messages :: Conversation_Message> > () const;
template bool Object ::
	is_class <Strategies :: Alive_State> () const;
template bool Object ::
	is_class <Strategies :: Chat_State> () const;
template bool Object ::
	is_class <Strategies :: Fight_State> () const;
template bool Object ::
	is_class <Strategies :: Play_State> () const;
template bool Object ::
	is_class <Strategies :: Strategy_State_Machine> () const;

template Reference <Items :: Container_Item <Items :: Item> > Object ::
	to_class <Items :: Container_Item <Items :: Item> > ();
template Reference <Items :: NPC> Object ::
	to_class <Items :: NPC> ();
template Reference <Movable_Model> Object ::
	to_class <Movable_Model> ();
template Reference <Set <Messages :: Conversation_Message> > Object ::
	to_class <Set <Messages :: Conversation_Message> > ();
template Reference <Strategies :: Strategy> Object ::
	to_class <Strategies :: Strategy> ();
template Reference <Strategies :: Alive_State> Object ::
	to_class <Strategies :: Alive_State> ();
template Reference <Strategies :: Fight_State> Object ::
	to_class <Strategies :: Fight_State> ();
template Reference <Strategies :: Menu_State> Object ::
	to_class <Strategies :: Menu_State> ();
template Reference <Strategies :: Play_State> Object ::
	to_class <Strategies :: Play_State> ();
template Reference <Strategies :: Strategy_State_Machine> Object ::
	to_class <Strategies :: Strategy_State_Machine> ();

template const Reference <Messages :: Battle_Message> Object ::
	to_class_const <Messages :: Battle_Message> () const;
template const Reference <Messages :: Conversation_Message> Object ::
	to_class_const <Messages :: Conversation_Message> () const;
