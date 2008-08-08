#include <tinyxml.h>

#include "engines/log.hpp"
#include "items/character.hpp"
#include "messages/communications/converse.hpp"

using namespace std;
using namespace Radakan;
using namespace Radakan :: Messages;
using namespace Radakan :: Messages :: Communications;

//	static
string Converse ::
	get_class_name ()
{
	return "Messages :: Communications :: Converse";
}

//  constructor
Converse ::
	Converse
	(
		Reference <Items :: Character> new_from,
		Reference <Items :: Character> new_to,
		const TiXmlElement * new_option
	) :
	Communication (create_name (new_option), new_from, new_to),
	option (new_option)
{
	Engines :: Log :: trace (me, Converse :: get_class_name ());
	
	//	Do nothing.
	
	assert (is_initialized ());
}

//  destructor
Converse ::
	~Converse ()
{
	Engines :: Log :: trace (me, Converse :: get_class_name (), "~");
	assert (is_initialized ());

	prepare_for_destruction ();
}

//	virtual
bool Converse ::
	is_initialized ()
	const
{
	return Object :: is_initialized ();
}

//	virtual
string Converse ::
	get_communication_name ()
{
	return get_class_name ();
}

Reference <Communication> Converse ::
	get_reaction ()
	const
{
	const TiXmlNode * temp = option;

	for (int i = 0; temp -> ValueStr () != "dialog"; i ++)
	{
		assert (i < 5);

		temp = temp -> Parent ();
		assert (temp != NULL);
	}

	temp = temp -> FirstChild ("reactions");
	assert (temp != NULL);

	for (int i = 0; temp -> ValueStr () != "reaction"; i ++)
	{
		assert (i < 5);

		temp = temp -> FirstChild ();
		assert (temp != NULL);
	}

	const TiXmlElement * reaction = temp -> ToElement ();
	assert (reaction != NULL);

	return Reference <Converse> (new Converse (to, from, reaction));
}

//	private & static
string Converse ::
	create_name (const TiXmlElement * option)
{
	assert (option != NULL);
	
	string result;
	
	if (option -> Attribute ("effect") != NULL)
	{
		result = "(" + * option -> Attribute (string ("effect")) + ") ";
	}
	
	if (option -> Attribute ("say") != NULL)
	{
		if (option -> Attribute ("shout") != NULL)
		{
			result += "Shout ";
		}
		result += "\"" + * option -> Attribute (string ("say")) + "\" ";
	}
	else if (option -> Attribute ("do") != NULL)
	{
		result += * option -> Attribute (string ("do"));
	}

	assert (! result . empty ());

	return result;
}
