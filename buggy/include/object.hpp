#ifndef OBJECT_HPP
#define OBJECT_HPP

//	The universal base class
//	All other files should (in)directly include this master include file.

#include <OgrePrerequisites.h>

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	#define SL_WIN32
	#define WIN32_LEAN_AND_MEAN
	#include "windows.h"
#endif

#include <cassert>
#include <cmath>
#include <iostream>
#include <string>
#include <set>

//	#define NDEBUG

#ifndef NDEBUG
	#define SL_DEBUG
#endif

using namespace std;

class Object :
	public string
{
	public:
#ifdef SL_WIN32
		Object ();	//	Some Windows compilers give an error otherwise.
#endif
		Object (string new_name);								//	constructor
		virtual ~Object ();										//	destructor
		template <typename T> T * copy () const;
		template <typename T> bool is_type () const;
		template <typename T> T * to_type () const;
		virtual bool is_initialized () const;
		bool is_initialized (string debug_message) const;
		virtual ostream & print () const;
		virtual ostream & debug () const;
		virtual ostream & error () const;
		
	//	protected:
		//	These methods should only be used by a container
		bool is_in_tree (const Object * tree) const;
		void put_in_tree (Object * new_tree_parent);
		void remove_from_tree (Object * old_tree_parent);
		
	private:
		Object * tree_parent;	//	the tree in which the object is
};

#ifdef SL_DEBUG
	//	This set is used to check if all objects were properly destructed.
	extern set <Object *> objects;
#endif

string to_string (float value);
bool is_nan (float value);
int to_int (string value);

#include "object.ipp"

#endif	//	OBJECT_HPP

