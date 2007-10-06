#ifndef RADAKAN_CONTAINER_HPP
#define RADAKAN_CONTAINER_HPP

#include "object.hpp"

using namespace std;

namespace Radakan
{

	///	Container is a generic container.
	template <class T> class Container :
		public virtual Object
	{
		public :
			static string get_class_name ();

			static const int unlimited;

			Container (string name = "", int new_maximal_size = unlimited);
			virtual ~Container ();
			virtual bool is_initialized () const;
			
			///	When sealed, no children can be added or dropped (see the exception below).
			void seal ();

			bool is_sealed () const;

			virtual bool is_empty () const = 0;

			virtual bool contains (const Reference <T> contained) const = 0;
			
			///	On success, I return 'true'.
			virtual bool add (Reference <T> additive) = 0;
			
			///	'move' returns true on success.
			///	'move' assumes that 'moved' is one of my children.
			virtual bool move (Reference <T> moved, Reference <Container <T> > destination);

			///	'drop' is allowed when destructing, even for sealed Containers.
			virtual void drop (Reference <T> dropped) = 0;

			///	Combine 'get_child' with 'get_another_child' to get a pointer to each child.
			virtual Reference <T> get_child () const = 0;
			
			///	Combine 'get_another_child' with 'get_child' to get a pointer to each child.
			virtual Reference <T> get_another_child () const = 0;

			const unsigned int maximal_size;

		private :
			bool sealed;
	};
}

#endif	//	RADAKAN_CONTAINER_HPP
