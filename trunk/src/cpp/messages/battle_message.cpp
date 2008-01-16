#include "engines/log.hpp"
#include "messages/battle_message.hpp"
#include "items/characters/character.hpp"

using namespace std;
using namespace Radakan;
using namespace Radakan :: Messages;

//	static
string Battle_Message ::
	get_class_name ()
{
	return "Battle_Message";
}

//  constructor
Battle_Message ::
	Battle_Message (const string & information, Reference <Items :: Characters :: Character> new_from, Reference <Items :: Characters :: Character> new_to) :
	Object (information),
	Message <Items :: Characters :: Character> ("Doesn't matter.", new_from, new_to)
{
	Engines :: Log :: trace (me, Battle_Message :: get_class_name ());
	
	//	Do nothing.
	
	assert (is_initialized ());
}

//  destructor
Battle_Message ::
	~Battle_Message ()
{
	Engines :: Log :: trace (me, Battle_Message :: get_class_name (), "~");
	assert (is_initialized ());

	prepare_for_destruction ();
}

//	virtual
bool Battle_Message ::
	is_initialized ()
	const
{
	return Object :: is_initialized ();
}

//	virtual
Reference <Message <Items :: Characters :: Character> > Battle_Message ::
	copy ()
	const
{
	return Reference <Message <Items :: Characters :: Character> >
		(new Battle_Message (me . get_name () + "'s copy", from, to));
}
