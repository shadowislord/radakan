#include "engines/log.hpp"
#include "items/character.hpp"
#include "opinion.hpp"
#include "world.hpp"

using namespace std;
using namespace Radakan;

//	static
string Opinion ::
	get_class_name ()
{
	return "Opinion";
}

//  constructor
Opinion ::
	Opinion (const Reference <Items :: Character> & other) :
	Object ("opinion about " + other . get_name ()),
	about (other . get_name ()),
	fear (0),
	like (0.5)
{
	Engines :: Log :: trace (me, Opinion :: get_class_name (), "", other . get_name ());

	assert (Opinion :: is_initialized ());
}

//  destructor
Opinion ::
	~Opinion ()
{
	Engines :: Log :: trace (me, Opinion :: get_class_name (), "~");
	assert (Opinion :: is_initialized ());

	prepare_for_destruction ();
}

//	virtual
bool Opinion ::
	is_initialized ()
	const
{
	assert (Object :: is_initialized ());

	return true;
}
