#ifndef RADAKAN_LOCATION_HPP
#define RADAKAN_LOCATION_HPP

#include "container.hpp"
#include "pointer.hpp"

using namespace std;

namespace Radakan
{

	///	Only one instances of me can hold a specifc Resident.
	template <class T> class Location :
		public Container <T>
	{
		public :
			static string get_class_name ();

			//	protected constructor, see below
			virtual ~Location ();
			virtual bool is_initialized () const;

			virtual bool is_empty () const;
			
			virtual bool contains (const Reference <T> & contained) const;
			
			///	'add' returns true on success.
			virtual bool add (Reference <T> additive);

			virtual void drop (Reference <T> dropped);

			virtual Reference <T> get_child () const;
			virtual Reference <T> get_another_child () const;
			
		protected :
			Location (unsigned int new_maximal_size = Container <T> :: unlimited());

		private :
			///	This can either be a Slot or a Set.
			Reference <Container <T> > implementation;
	};
}

#endif	// RADAKAN_LOCATION_HPP
