#ifndef RADAKAN_OBJECT_HPP
#define RADAKAN_OBJECT_HPP

///	All other Radakan headers should (in)directly include this header.

//	Uncomment the next line for release mode.
//	#define NDEBUG

#ifndef NDEBUG
	//	Radakan run with RADAKAN_DEBUG will print debug messages.
	#define RADAKAN_DEBUG
#endif

#include <cassert>
#include <cmath>
#include <iostream>
#include <string>
#include <set>

#include <boost/smart_ptr.hpp>
#include <boost/utility.hpp>

#include <OgreVector3.h>

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	#define WIN32_LEAN_AND_MEAN

	#include <windows.h>

	#if _MSC_VER
		#pragma warning(disable: 4250)
		#pragma warning(disable: 4355)
	#endif

	#define RADAKAN_WINDOWS
#endif

#include "reference.hpp"

using namespace std;

namespace Radakan
{
	const Ogre :: Vector3 x_axis (1, 0, 0);
	const Ogre :: Vector3 y_axis (0, 1, 0);	//	upwards
	const Ogre :: Vector3 z_axis (0, 0, 1);

	const Ogre :: Vector3 zero_vector (0, 0, 0);

	string to_string (const Ogre :: Vector3 & vector);
	
	///	I'm the abstract base class for all Radakan classes.
	///	I can't be copied, consider a reference of me instead.
	class Object :
		public boost :: noncopyable
	{
		public :
			static string get_class_name ();

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

			void register_reference (const Reference_Base & reference) const;

			void unregister_reference (const Reference_Base & reference) const;

			bool is_destructing () const;
			virtual bool is_singleton () const;

			const string name;

		private :
			bool has_dependency () const;
			bool does_depend (const Reference_Base & candidate) const;

			///	I store my dependencies as const to reduce the number of casts,
			///	but they are 'const_cast'-ed, at my destruction.
			mutable boost :: scoped_ptr <set <const Reference_Base *> > dependencies;

			///	'status' can be 'constructing', 'running' or 'destructing'.
			string status;

		public :
			///	'me' corrensonds to 'this'.
			Reference <Object> me;
	};
}

#endif	//	RADAKAN_OBJECT_HPP
