#include "engines/log.hpp"
#include "messages/communications/fight.hpp"
#include "items/character.hpp"

using namespace std;
using namespace Radakan;
using namespace Radakan :: Messages;
using namespace Radakan :: Messages :: Communications;

//	static
string Fight ::
	get_class_name ()
{
	return "Messages :: Communications :: Fight";
}

//  constructor
Fight ::
	Fight
	(
		const string & information,
		Reference <Items :: Character> new_from,
		Reference <Items :: Character> new_to
	) :
	Communication (information, new_from, new_to)
{
	Engines :: Log :: trace (me, Fight :: get_class_name (), "", information,
		new_from . get_name (), new_to . get_name ());
	
	//	Do nothing.
	
	assert (is_initialized ());
}

//  destructor
Fight ::
	~Fight ()
{
	Engines :: Log :: trace (me, Fight :: get_class_name (), "~");
	assert (is_initialized ());

	prepare_for_destruction ();
}

//	virtual
bool Fight ::
	is_initialized ()
	const
{
	return Object :: is_initialized ();
}

//	virtual
string Fight ::
	get_communication_name ()
{
	return get_class_name ();
}
