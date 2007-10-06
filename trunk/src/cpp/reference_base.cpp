#include <sstream>

//	#include "log.hpp"
#include "reference_base.hpp"

using namespace std;
using namespace Radakan;

string Radakan ::
	bool_to_string (const bool & value)
{
	if (value)
	{
		return "true";
	}
	return "false";
}

string Radakan ::
	to_string (const float & value)
{
	ostringstream oss;
	oss << value;	//	insert int into stream
	return oss . str ();
}

//	static
float Radakan ::
	to_float (const string & value)
{
	istringstream iss (value);
	float result;
	iss >> result;
	return result;
}

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
	//	Engines :: Log :: trace (* this, get_class_name (), "");

	assert (counter < 64000);
}

Reference_Base ::
	~Reference_Base ()
{
	//	Engines :: Log :: trace (* this, get_class_name (), "~");
}

//	virtual
string Reference_Base ::
	get_name ()
	const
{
	return "[" + number + "]";
}
