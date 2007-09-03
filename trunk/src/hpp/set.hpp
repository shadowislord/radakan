#ifndef RADAKAN_SET_HPP
#define RADAKAN_SET_HPP

#include "object.hpp"

using namespace std;

namespace Radakan
{

	///	Set can contain instances of a specific Object subclass, but not more then once.
	template <class T> class Set :
		public virtual Object
	{
		public :
			Set (string name = "", int new_maximal_size = unlimited);
			virtual ~Set ();
			virtual bool is_initialized () const;
			
			static const string get_class_name ();

			bool is_empty () const;

			///	On success, I add myself to the dependencies of 't' and return 'true'.
			virtual bool add (T & t);
			
			virtual bool contains (T & t) const;
			
			///	'move' returns true on success.
			///	'move' assumes that t is one of my children.
			virtual bool move (T & t, Set <T> & destination);

			///	To allow 'drop' to be called from any class, t doesn't have to be of type T.
			///	I remove myself from the dependencies of 't'.
			///	'drop' is allowed when destructing, even for sealed Sets.
			virtual void drop (Object & t, bool stay = false);

			///	Combine 'get_child' with 'get_another_child' to get a pointer to each child.
			T * get_child () const;
			
			///	Combine 'get_another_child' with 'get_child' to get a pointer to each child.
			T * get_another_child () const;

			static const int unlimited;

			const unsigned int maximal_size;

			///	When sealed, no children can be added or dropped (see the exception above).
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

#endif	//	RADAKAN_SET_HPP
