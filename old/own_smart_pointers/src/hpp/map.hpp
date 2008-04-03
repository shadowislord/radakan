#ifndef RADAKAN_MAP_HPP
#define RADAKAN_MAP_HPP

#include <map>

#include "container.hpp"
#include "pair.hpp"

using namespace std;

namespace Radakan
{

	template <typename T, class U> class Map :
		public Container <Pair <T, U> >
	{
		public :
			static string get_class_name ();

			Map (string name, bool new_weak_children = false);
			virtual ~Map ();
			virtual bool is_initialized () const;
			
			virtual bool is_empty () const;

			virtual bool contains (const Reference <Pair <T, U> > & contained) const;

			virtual bool add (Reference <Pair <T, U> > additive);
			
			///	'drop' is allowed when destructing, even for sealed Containers.
			virtual void drop (Reference <Pair <T, U> > dropped);

			///	Combine 'get_child' with 'get_another_child' to get a pointer to each child.
			virtual Reference <Pair <T, U> > get_child () const;
			
			///	Combine 'get_another_child' with 'get_child' to get a pointer to each child.
			virtual Reference <Pair <T, U> > get_another_child () const;

			Reference <U> look_up (const T & t) const;

		private :
			boost :: scoped_ptr <map <T, Reference <U> > > children;

			typedef typename map <T, Reference <U> > :: const_iterator Next_Child_Type;

			//	'mutable' added to allow change even if in a const Map.
			mutable Next_Child_Type next_child;
		};
}

#endif	//	RADAKAN_MAP_HPP
