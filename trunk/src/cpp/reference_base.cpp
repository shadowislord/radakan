#include <iostream>

#include "log.hpp"
#include "reference_base.hpp"

using namespace std;
using namespace Radakan;

//	static
int Reference_Base ::
	counter (0);

//	static
const string Reference_Base ::
	get_class_name ()
{
	//	Do not turn this into a static data member,
	//	that'd cause problems for template (sub)classes.

	return "Reference_Base";
}

bool Reference_Base ::
	operator== (const Reference_Base & other) const
{
	return counter == other . counter;
}

bool Reference_Base ::
	operator!= (const Reference_Base & other) const
{
	return ! (counter == other . counter);
}

bool Reference_Base ::
	operator< (const Reference_Base & other) const
{
	return counter < other . counter;
}

Reference_Base ::
	Reference_Base () :
	number (counter ++)
{
	assert (counter < 36);

	Engines :: Log :: trace (* this, get_class_name (), "", to_string (number));
}

Reference_Base ::
	~Reference_Base ()
{
	Engines :: Log :: trace (* this, get_class_name (), "~", to_string (number));
}

//	virtual
const string Reference_Base ::
	get_name ()
	const
{
	return to_string (number);
}
