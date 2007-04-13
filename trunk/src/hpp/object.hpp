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
namespace tsl
{
	const Ogre :: Vector3 zero_vector (0, 0, 0);

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
			static string get_class_name ();
			template <class T> bool is_type () const;
			template <class T> T & to_type () const;

			stringstream & show (bool condition = true) const;
			ostream & log (bool condition = true) const;
			ostream & error () const;

			static const bool debugging;

			///	If possible, use the Disjoint_Set methods instead.
			bool has_parent () const;

			///	If possible, use the Disjoint_Set methods instead.
			bool is_in (const Object & set) const;

			///	If possible, use the Disjoint_Set methods instead.
			void put_in (const Object & new_parent);

			///	If possible, use the Disjoint_Set methods instead.
			void remove_from (const Object & old_parent);

			static string bool_to_string (const bool value);
			static string to_string (const float value);
			static string to_string (const Ogre :: Vector3 & vector);
			static float to_float (const string);

			///	The message shown in the GUI.
			static stringstream message;

			#ifdef TSL_DEBUG
				static unsigned long int turn;

				//	This set is used to check if all objects were properly destructed.
				static set <Object *> objects;
			#endif

		protected:
			///	To avoid plain Object instances, the constructor(s) is/are proteced.
			#ifdef TSL_WIN
				///	Some Windows compilers give an error otherwise.
				Object ();
			#endif
			Object (string new_name);
			
		private :
			const Object * parent;	//	the Disjoint_Set in which the object is

			/// for messages that should be ignored
			static stringstream the_abyss;
	};
}

#endif	//	TSL_OBJECT_HPP
