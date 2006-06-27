//	The universal base class
//	All other files should (in)directly include this master include file.

#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <Ogre.h>
//	#include <ExampleApplication.h>

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	#define TSLRPG_WIN32
	#define WIN32_LEAN_AND_MEAN
	#include "windows.h"
#endif

#include <cassert>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Object :
	public string
{
	public:
#ifdef TSLRPG_WIN32
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

