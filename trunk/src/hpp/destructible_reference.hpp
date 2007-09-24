#ifndef RADAKAN_DESTRUCTIBLE_REFERENCE_HPP
#define RADAKAN_DESTRUCTIBLE_REFERENCE_HPP

#include "reference.hpp"

using namespace std;

namespace Radakan
{
	template <class X> class Set;

	template <class T> class Destructible_Reference:
		public Reference <T>
	{
		public :
			Destructible_Reference (T * new_pointee = NULL);
			Destructible_Reference (const Destructible_Reference <T> & other);
			Destructible_Reference (const Reference <T> & other);
			template <class V> Destructible_Reference (const Destructible_Reference <V> & other);
			template <class V> Destructible_Reference (const Reference <V> & other);
			virtual ~Destructible_Reference ();

			static const string get_class_name ();
			
			bool has_parent () const;
			void set_parent (Reference <Set <T> >);

		private :
			///	'parent' points to an object that allows me to self-destruct,
			///	upon destruction of the pointee.
			Reference <Set <T> > parent;
	};
}

#endif	//	RADAKAN_DESTRUCTIBLE_REFERENCE_HPP
