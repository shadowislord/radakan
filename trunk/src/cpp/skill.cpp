#include <OgreMath.h>
#include "engines/log.hpp"
#include "skill.hpp"

using namespace std;
using namespace Radakan;

//	static
string Skill ::
	get_class_name ()
{
	return "Skill";
}

//	static
const float Skill ::
	default_experience (13);

//  constructor
Skill ::
	Skill (string new_name, float initial_experience) :
	Object (new_name),
	experience (initial_experience, 0, Mathematics :: infinite)
{
	Engines :: Log :: trace (me, Skill :: get_class_name (), "", new_name, to_string (initial_experience));

	assert (Skill :: is_initialized ());
}

//  destructor
Skill ::
	~Skill ()
{
	Engines :: Log :: trace (me, Skill :: get_class_name (), "~");
	assert (Skill :: is_initialized ());

	prepare_for_destruction ();
}

//	virtual
bool Skill ::
	is_initialized ()
	const
{
	assert (Object :: is_initialized ());

	return true;
}

void Skill ::
	add_experience (float additional)
{
	assert (Skill :: is_initialized ());

	experience += additional;
}

float Skill ::
	get_value ()
	const
{
	assert (Skill :: is_initialized ());

	//	see http://radakan.org/wiki/index.php?title=Stats#Evolution
	return 4 / Ogre :: Math :: PI
		* Ogre :: Math :: ATan (experience / default_experience) . valueRadians ();
}
