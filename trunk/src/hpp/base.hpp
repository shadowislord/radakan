///	All other Radakan headers should (in)directly include this header.

//	Uncomment the next line for release mode.
//	#define NDEBUG

#ifndef NDEBUG
	//	Radakan run with RADAKAN_DEBUG will print debug messages.
	#define RADAKAN_DEBUG
#endif

#include <cassert>
#include <cmath>
#include <string>

#include <boost/smart_ptr.hpp>
#include <boost/utility.hpp>

#include <OgreVector3.h>

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	#define WIN32_LEAN_AND_MEAN

	// Windows.h has a macro called max (retarded ms!) which conflicts with this STL method!
	#define NOMINMAX

	#include <windows.h>

	#if _MSC_VER
		#pragma warning(disable: 4250)
		#pragma warning(disable: 4355)
	#endif

	#define RADAKAN_WINDOWS
#endif



using namespace std;

namespace Radakan
{
	string bool_to_string (const bool & value);
	string to_string (const float & value);
	string to_lower_case (const string & value);
	float to_float (const string & value);
}