#ifndef TSL_SET_HPP
#define TSL_SET_HPP

#include "object.hpp"

using namespace std;

namespace TSL
{

	///	Set can contain objects of a certain type, but not more then once.
	///	Set elements add the set as one of their dependencies.
	template <class T> class Set :
		public virtual Object
	{
		public :
			Set (string name = "", int new_maximal_size = unlimited);
			virtual ~Set ();
			virtual bool is_initialized () const;
			
			static const string get_class_name ();

			virtual void drop_implicit_dependency (const Object & dependency);

			bool is_empty () const;

			///	'add' returns true on success.
			virtual bool add (T & t);
			
			virtual bool contains (T & t) const;
			
			///	'move' returns true on success.
			///	'move' assumes that t is one of my children.
			virtual bool move (T & t, Set <T> & destination);

			void drop (T & t, bool stay = false);

			///	Combine 'get_child' with 'get_another_child' to get a pointer to each child.
			T * get_child () const;
			
			///	Combine 'get_another_child' with 'get_child' to get a pointer to each child.
			T * get_another_child () const;

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

			const string context;
		};
}

#endif	//	TSL_DISJOINT_SET_HPP
