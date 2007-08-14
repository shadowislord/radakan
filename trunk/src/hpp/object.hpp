#ifndef RADAKAN_OBJECT_HPP
#define RADAKAN_OBJECT_HPP

///	All other Radakan headers should (in)directly include this header.

#include <OgrePrerequisites.h>
#include <OgreVector3.h>

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	#define WIN32_LEAN_AND_MEAN

	#include <windows.h>

	#if _MSC_VER
		#pragma warning(disable: 4250)
	#endif

	#define RADAKAN_WINDOWS
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
	//	Radakan run with RADAKAN_DEBUG will print debug messages.
	#define RADAKAN_DEBUG
#endif

using namespace std;

namespace Radakan
{
	const Ogre :: Vector3 zero_vector (0, 0, 0);

	string bool_to_string (const bool & value);
	string to_string (const float & value);
	string to_string (const Ogre :: Vector3 & vector);
	float to_float (const string & value);

	#ifdef RADAKAN_DEBUG
		template <class T> class Set;
	#endif

	///	I'm the abstract base class for all Radakan classes.
	///	I'm a subclass of 'string', because I use the string as my name.
	///	I can't be copied, consider a reference of me instead.
	class Object :
		public boost :: noncopyable
	{
		public :
			bool operator== (const Object & other_object) const;
			bool operator!= (const Object & other_object) const;

			#ifdef RADAKAN_WINDOWS
				///	Some Windows compilers give an error otherwise.
				Object ();
			#endif
			Object (string new_name);
			virtual ~Object ();
			virtual bool is_initialized () const;
			
			///	Call 'prepare_for_destruction ()' at the beginning of the destructor of each non-abstract subclass. Don't worry about calling it twice.
			///	'destructing' is set to 'true'.
			///	I'm removed from each parent Set.
			void prepare_for_destruction ();

			static const string get_class_name ();
			
			template <class T> bool is_type () const;
			template <class T> T & to_type () const;

			void remember (Object & dependency);

			///	I will self-destruct, when I forget my last dependency.
			///	Set 'stay' to 'true' to force me to not self-destruct.
			void forget (const Object & dependency, bool stay = false);

			///	'drop' results in an error, except for Sets.
			virtual void drop (Object & t, bool stay = false);

			const bool & is_destructing () const;

			///	'me' is '* this'.
			Object & me;
			const string name;

			static const bool debugging;

			static const Object update;
			static const Object terminate;
			
		private :
			bool has_dependency () const;
			bool does_depend (const Object & candidate) const;

			///	I store my dependencies as const to reduce the number of casts,
			///	but they are 'const_cast'-ed, at my destruction.
			set <const Object *> dependencies;

			bool destructing;
	};
}

#endif	//	RADAKAN_OBJECT_HPP
