#ifndef TSL_OBJECT_HPP
#define TSL_OBJECT_HPP

#include <OgrePrerequisites.h>
#include <OgreVector3.h>

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	#define WIN32_LEAN_AND_MEAN

	#include <windows.h>

	#if _MSC_VER
		#pragma warning(disable: 4250)
	#endif

	#define TSL_WIN
#endif

#include <cassert>
#include <cmath>
#include <iostream>
#include <string>
#include <set>

#include <boost/utility.hpp>

//	Uncomment the next line for release mode.
//	#define NDEBUG

#ifndef NDEBUG
	//	TSL run with TSL_DEBUG will print debug messages.
	#define TSL_DEBUG
#endif

using namespace std;

///	The Scattered Lands namespace
namespace TSL
{
	const Ogre :: Vector3 zero_vector (0, 0, 0);

	string bool_to_string (const bool & value);
	string to_string (const float & value);
	string to_string (const Ogre :: Vector3 & vector);
	float to_float (const string & value);

	#ifdef TSL_DEBUG
		template <class T> class Set;
	#endif

	///	Object is the universal abstract base class for all TSL classes.
	///	All other files should (in)directly include this master include file.
	class Object :
		public string,
		public boost :: noncopyable
	{
		public :
			//	protected constructor(s), see below
			virtual ~Object ();
			virtual bool is_initialized () const;
			
			static const string get_class_name ();
			
			template <class T> bool is_type () const;
			template <class T> T & to_type () const;

			static const bool debugging;

			void remember (const Object & dependency, const string context);

			///	I will self-destruct, when I forget my last dependency.
			///	Set 'stay' to 'true' to force me to not self-destruct.
			void forget (const Object & dependency, const string context, bool stay = false);

			#ifdef TSL_DEBUG
				static unsigned long int turn;

				//	This set is used to check if all objects were properly destructed.
				static Set <Object> objects;
			#endif

			Object & me;
			const string my;

		protected:
			///	To avoid plain Object instances, the constructor(s) is/are proteced.
			#ifdef TSL_WIN
				///	Some Windows compilers give an error otherwise.
				Object ();
			#endif
			Object (string new_name);

		private :
			bool has_dependency (const string context) const;
			bool does_depend (const Object & candidate, const string context) const;

			///	Only one dependency is allowed per context.
			map <const string, const Object *> dependencies;
	};
}

#endif	//	TSL_OBJECT_HPP
