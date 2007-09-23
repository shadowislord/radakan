#include "battle_message.hpp"
#include "character.hpp"
#include "log.hpp"

using namespace std;
using namespace Radakan;
using namespace Radakan :: Messages;

//	static
const string Battle_Message ::
	get_class_name ()
{
	return "Battle_Message";
}

//  constructor
Battle_Message ::
	Battle_Message (const string & information, Reference <Items :: Character> new_from, Reference <Items :: Character> new_to) :
	Object (information),
	from (new_from),
	to (new_to)
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
