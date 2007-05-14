#ifndef TSL_SET_HPP
#define TSL_SET_HPP

#include "object.hpp"

using namespace std;

namespace TSL
{

	///	An Object can't be inside more then one Set at once in a certain context.
	///	A Set cannot contain multiple Objects with the same name.
	template <class T> class Set :
		public virtual Object
	{
		public :
			Set (int new_maximal_size = unlimited, string name = "anonymous set");
			virtual ~Set ();
			virtual bool is_initialized () const;
			
			static const string class_name;

			bool is_empty () const;

			///	'add' returns true on success.
			virtual bool add (T & t);
			
			virtual bool contains (string name) const;
			
			///	'move' returns true on success.
			///	'move' assumes that t is one of my children.
			virtual bool move (T & t, Set <T> & destination);

			void drop (T & t);

			///	'get_child' assumes that I have a child with that name.
			T & get_child (string name) const;
			
			///	Combine 'get_child' with 'get_another_child' to get a pointer to each child.
			T * get_child () const;
			
			///	Combine 'get_another_child' with 'get_child' to get a pointer to each child.
			T * get_another_child () const;

			///	Delete all children, usefull for destruction.
			void delete_children ();

			static const int unlimited;

			const int maximal_size;

			///	When sealed, no more children can be added or moved away.
			///	There's no unseal!
			void seal ();

			bool is_sealed () const;

		private :
			set <T *> children;

			//	'typename' added to assure that const_iterator is a type.
			//	'class' would give MSV problems.
			typedef typename set <T *> :: const_iterator T_iterator;
		
			//	'mutable' added to allow change even if in a const Set.
			mutable T_iterator next_child;

			bool sealed;
		};
}

#endif	//	TSL_DISJOINT_SET_HPP
