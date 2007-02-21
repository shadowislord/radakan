#ifndef TSL_DISJOINT_SET_HPP
#define TSL_DISJOINT_SET_HPP

#include "object.hpp"

using namespace std;

namespace tsl
{

	///	An Object can't be inside more then one Disjoint_Set at once.
	///	A Disjoint_Set cannot contain multiple Objects with the same name.
	template <class T> class Disjoint_Set :
		public virtual Object
	{
		public :
			//	protected constructor, see below
			virtual ~Disjoint_Set ();
			virtual bool is_initialized () const;
			static string get_class_name ();

			bool is_empty () const;

			///	'add' returns true on success.
			virtual bool add (T & t);
			
			virtual bool contains (string name) const;
			
			///	'move' returns true on success.
			///	'move' assumes that t is one of my children.
			virtual bool move (T & t, Disjoint_Set <T> & destination);

			///	'get_child' assumes that I have a child with that name.
			T & get_child (string name) const;
			
			///	Combine 'get_child' with 'get_another_child' to get a pointer to each child.
			T * get_child () const;
			
			///	Combine 'get_another_child' with 'get_child' to get a pointer to each child.
			T * get_another_child () const;

			///	Delete all children, usefull for destruction.
			void delete_children ();

		protected :
			Disjoint_Set ();

		private :
			set <T *> children;
		
			//	'typename' added to assure that const_iterator is a type.
			//	'class' would give MSV problems.
			typedef typename set <T *> :: const_iterator T_iterator;
		
			//	'mutable' added to allow change even if in a const Disjoint_Set.
			mutable T_iterator next_child;
		};
}

#endif	//	TSL_DISJOINT_SET_HPP
