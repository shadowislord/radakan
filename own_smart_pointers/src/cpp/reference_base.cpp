#include <sstream>

#include "reference_base.hpp"
//	#include "engines/log.hpp"

using namespace std;
using namespace Radakan;

//	static
unsigned int Reference_Base ::
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
	//	Engines :: Log :: trace (* this, get_class_name (), "");
}

Reference_Base ::
	~Reference_Base ()
{
	//	Engines :: Log :: trace (* this, get_class_name (), "~");
}

//	virtual
string Reference_Base ::
	get_name (bool suppress_debug_info)
	const
{
	if (suppress_debug_info)
	{
		return "";
	}
	else
	{
		return "[" + number + "]";
	}
}

ostream & Radakan ::
	operator<< (ostream & out_stream, const Reference_Base & reference_base)
{
	return out_stream << reference_base . get_name ();
}
