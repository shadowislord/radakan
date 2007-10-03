#include <iostream>

#include "log.hpp"
#include "reference_base.hpp"

using namespace std;
using namespace Radakan;

//	static
int Reference_Base ::
	counter (0);

//	static
string Reference_Base ::
	get_class_name ()
{
	//	Do not turn this into a static data member,
	//	that'd cause problems for template (sub)classes.

	return "Reference_Base";
}

Reference_Base ::
	Reference_Base () :
	number (to_string (counter ++))
{
	Engines :: Log :: trace (* this, get_class_name (), "");

	assert (counter < 64000);
}

Reference_Base ::
	~Reference_Base ()
{
	Engines :: Log :: trace (* this, get_class_name (), "~");
}

//	virtual
string Reference_Base ::
	get_name ()
	const
{
	return "[" + number + "]";
}
