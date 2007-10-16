#ifndef RADAKAN_SET_HPP
#define RADAKAN_SET_HPP

#include "container.hpp"

using namespace std;

namespace Radakan
{

	///	Set can contain instances of a specific Object subclass, but not more then once.
	template <class T> class Set :
		public Container <T>
	{
		public :
			Set (string name = "", int new_maximal_size = Container <T> :: unlimited);
			virtual ~Set ();
			virtual bool is_initialized () const;
			
			static string get_class_name ();

			virtual bool is_empty () const;

			virtual bool contains (const Reference <T> & contained) const;
			
			///	On success, I return 'true'.
			virtual bool add (Reference <T> additive);
			
			///	'drop' is allowed when destructing, even for sealed Sets.
			virtual void drop (Reference <T> dropped);

			///	Combine 'get_child' with 'get_another_child' to get a pointer to each child.
			virtual Reference <T> get_child () const;
			
			///	Combine 'get_another_child' with 'get_child' to get a pointer to each child.
			virtual Reference <T> get_another_child () const;

		private :
			boost :: scoped_ptr <set <Reference <T> > > children;

			//	'typename' added to assure that const_iterator is a type.
			//	'class' would give MSV problems.
			typedef typename set <Reference <T> > :: const_iterator T_Iterator;
		
			//	'mutable' added to allow change even if in a const Set.
			mutable T_Iterator next_child;
		};
}

#endif	//	RADAKAN_SET_HPP
