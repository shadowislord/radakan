#ifndef TSL_OBJECT_HPP
#define TSL_OBJECT_HPP

///	All other TSL headers should (in)directly include this header.

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

	///	I'm the abstract base class for all TSL classes.
	///	I'm a subclass of 'string', because I use the string as my name.
	///	I can't be copied, consider a reference of me instead.
	class Object :
		public string,
		public boost :: noncopyable
	{
		public :
			#ifdef TSL_WIN
				///	Some Windows compilers give an error otherwise.
				Object ();
			#endif
			Object (string new_name);
			virtual ~Object ();
			virtual bool is_initialized () const;
			
			static const string get_class_name ();
			
			template <class T> bool is_type () const;
			template <class T> T & to_type () const;

			static const bool debugging;

			void remember (Object & dependency);

			///	I will self-destruct, when I forget my last dependency.
			///	Set 'stay' to 'true' to force me to not self-destruct.
			void forget (const Object & dependency, bool stay = false);

			virtual void drop_implicit_dependency (const Object & dependency);

			///	'me' is '* this'.
			Object & me;
			const string my;

			static const Object update;
			static const Object terminate;
			
		private :
			bool has_dependency () const;
			bool does_depend (const Object & candidate) const;

			///	I store my dependencies as const to reduce the number of casts,
			///	but they are const_cast-ed, at my destruction.
			set <const Object *> dependencies;
	};
}

#endif	//	TSL_OBJECT_HPP
