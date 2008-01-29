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
	to_string (const Ogre :: Vector3 & vector)
{
	return "(" + to_string (vector . x) + ", " + to_string (vector . y) + ", "
												+ to_string (vector . z) + ")";
}

string Radakan ::
	to_lower_case (const string & value)
{
	string result = value;
	
	transform (result . begin(), result . end(), result . begin(), :: tolower);

	return result;
}

float Radakan ::
	to_float (const string & value)
{
	istringstream iss (value);
	float result;
	iss >> result;
	return result;
}

//	static
string Radakan ::
	without (const string & value, const string & diff)
{
	string result (value);
	size_t length = diff . length ();

	for (size_t found = result . find (diff); found != string :: npos;
		found = result . find (diff))
	{
		result . erase (found, length);
	}
	
	return result;
}
