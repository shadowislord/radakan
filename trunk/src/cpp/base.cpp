#include "base.hpp"

using namespace std;

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

string Radakan ::
	to_lower_case (const string & value)
{
	string result = value;
	
	transform (result . begin(), result . end(), result . begin(), :: tolower);

	return result;
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
