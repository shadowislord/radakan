//	The universal base class
//	All other files should (in)directly include this master include file.

#ifndef OBJECT_HPP
#define OBJECT_HPP

//	We expect to use Crystal Space everywhere.
#include <cssysdef.h>

//	MSVC users do not run configure, so use special MSVC configuration file.
#if defined (CS_WIN32_CSCONFIG)
	#include "config-msvc.h"
#else
	#include "config.h"
#endif

#include <cassert>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <csutil/ref.h>

using namespace std;

class Object :
	public string
{
	public:
#if defined (CS_WIN32_CSCONFIG)
		Object ();
#endif
		Object (string new_name);								//	constructor
		virtual ~Object ();										//	destructor
		template <typename T> T * copy () const;
		template <typename T> bool is_type () const;
		template <typename T> T * to_type () const;
		virtual bool is_initialized () const;
		virtual ostream & print () const;
		virtual ostream & debug () const;
		virtual ostream & error () const;
};

extern vector <Object *> objects;

string to_string (float value);
bool is_nan (float value);
int to_int (string value);
//	void pause ();

#include "object.ipp"

#endif	//	OBJECT_HPP

