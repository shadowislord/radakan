#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <OgrePrerequisites.h>

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	#define TSL_WIN32
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

//	Uncomment the next line for release mode.
//	#define NDEBUG

#ifndef NDEBUG
	//	TSL run with TSL_DEBUG will print usefull messages.
	#define TSL_DEBUG
	//	TSL run with TSL_TRACE will print all messages. Do not use it without TSL_DEBUG.
	#define TSL_TRACE
#endif

using namespace std;

///	The Scattered Lands namespace
namespace tsl
{
	
	///	Object is the universal abstract base class for all TSL classes.
	///	All other files should (in)directly include this master include file.

	class Object :
		public string
	{
		public :
			//	protected constructor(s), see below
			virtual ~Object ();
			virtual bool is_initialized () const;
			static string get_type_name ();
			template <typename T> bool is_type () const;
			template <typename T> T & to_type () const;
			template <typename T> bool warn (bool initialization) const;
			
			ostream & debug () const;
			ostream & trace () const;
			ostream & error () const;

			///	These methods should only be used by a Set &#60;T&#62;. Don't use them directly.
			///	Use the Set &#60;T&#62; methods (add, contains & move_to) instead.
			bool has_parent () const;
			bool is_in (const Object & set) const;
			void put_in (const Object & new_parent);
			void remove_from (const Object & old_parent);

			static string to_string (const Object * object);
			static string bool_to_string (bool value);
			static string to_string (float value);
			static bool is_nan (float value);
			static int to_int (string value);

		protected:
			///	To avoid 'plain' Object instances, the constructor(s) is/are proteced.
			#ifdef TSL_WIN32
				///	Some Windows compilers give an error otherwise.
				Object ();
			#endif
			Object (string new_name);

		private :
			ostream & print (string message) const;
			const Object * parent;	//	the Set <T> in which the object is
	};

	#ifdef TSL_DEBUG
		//	This set is used to check if all objects were properly destructed.
		extern set <Object *> objects;
	#endif
}

#include "object.ipp"

#endif	//	OBJECT_HPP
