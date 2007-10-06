#ifndef RADAKAN_SLOT_HPP
#define RADAKAN_SLOT_HPP

#include "container.hpp"

using namespace std;

namespace Radakan
{
	template <class T> class Slot :
		public Container <T>
	{
		public :
			static string get_class_name ();

			Slot (string new_name);
			virtual ~Slot ();
			virtual bool is_initialized () const;

			virtual bool is_empty () const;

			virtual bool contains (const Reference <T> contained) const;
			
			///	On success, I return 'true'.
			virtual bool add (Reference <T> additive);
			
			///	'drop' is allowed when destructing, even for sealed Sets.
			virtual void drop (Reference <T> dropped);

			///	Combine 'get_child' with 'get_another_child' to get a pointer to each child.
			virtual Reference <T> get_child () const;
			
			///	Combine 'get_another_child' with 'get_child' to get a pointer to each child.
			virtual Reference <T> get_another_child () const;

		private :
			Reference <T> child;
	};
}

#endif	//	RADAKAN_SLOT_HPP
