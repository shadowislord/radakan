#ifndef OBJECT_HPP
#define OBJECT_HPP

//	The universal base class
//	All other files should (in)directly include this master include file.

#include <OgrePrerequisites.h>

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	#define SL_WIN32
	#define WIN32_LEAN_AND_MEAN
	#include "windows.h"
	#if _MSC_VER
		#pragma warning(disable: 4250)
	#endif
#endif

#include <cassert>
#include <cmath>
#include <iostream>
#include <string>
#include <set>

//	#define NDEBUG

#ifndef NDEBUG
	//	SL run with SL_DEBUG will print usefull messages.
	#define SL_DEBUG
	//	SL run with SL_TRACE will print all messages, do not use it without SL_DEBUG.
	#define SL_TRACE
#endif

using namespace std;

///	Scattered Lands namespace
namespace sl
{
	class Object :
		public string
	{
		public :
	#ifdef SL_WIN32
			Object ();	//	Some Windows compilers give an error otherwise.
	#endif
			Object (string new_name);								//	constructor
			virtual ~Object ();										//	destructor
			virtual bool is_initialized () const;
			static string get_type_name ();
			template <typename T> bool is_type () const;
			template <typename T> T & to_type () const;
			template <typename T> bool warn (bool initialization) const;
			
			virtual ostream & print () const;
			virtual ostream & debug () const;
			virtual ostream & trace () const;
			virtual ostream & error () const;

			//	These methods should only be used by a Set
			bool has_parent () const;
			bool is_in (const Object & set) const;
			void put_in (const Object & new_parent);
			void remove_from (const Object & old_parent);

			static string to_string (const Object * object);
			static string bool_to_string (bool value);
			static string to_string (float value);
			static bool is_nan (float value);
			static int to_int (string value);

		private :
			const Object * parent;	//	the set in which the object is
	};

	#ifdef SL_DEBUG
		//	This set is used to check if all objects were properly destructed.
		extern set <Object *> objects;
	#endif
}

#include "object.ipp"

#endif	//	OBJECT_HPP
