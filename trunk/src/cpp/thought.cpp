#include "character.hpp"
#include "log.hpp"
#include "thought.hpp"
#include "world.hpp"

using namespace std;
using namespace Radakan;

//	static
string Thought ::
	get_class_name ()
{
	return "Thought";
}

//  constructor
Thought ::
	Thought (Reference <const Items :: Character> other) :
	Object ("thought about " + other -> name),
	about (other -> name),
	fear (0),
	like (0.5)
{
	Engines :: Log :: trace (me, Thought :: get_class_name (), "", other -> name);

	assert (Thought :: is_initialized ());
}

//  destructor
Thought ::
	~Thought ()
{
	Engines :: Log :: trace (me, Thought :: get_class_name (), "~");
	assert (Thought :: is_initialized ());

	prepare_for_destruction ();
}

//	virtual
bool Thought ::
	is_initialized ()
	const
{
	assert (Object :: is_initialized ());

	return true;
}
