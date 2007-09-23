#ifndef RADAKAN_REFERENCE_HPP
#define RADAKAN_REFERENCE_HPP

#include <cassert>
#include <string>

#include "reference_base.hpp"

using namespace std;

namespace Radakan
{
	template <class X> class Set;

	template <class T, class U = Set <T> > class Reference:
		public Reference_Base
	{
		public :
			Reference & operator= (const Reference & other);
			template <class V, class W> Reference & operator= (const Reference <V, W> & other);
			template <class V, class W> bool operator== (const Reference <V, W> & other) const;

			Reference (T * new_pointee = NULL);
			Reference (const Reference & other);
			template <class V, class W> Reference (const Reference <V, W> & other);
			virtual ~Reference ();

			static const string get_class_name ();
			
			virtual string get_name () const;
			
			T * operator-> ();
			const T * operator-> () const;

			bool points_to_object () const;
			template <class V, class W> bool points_to_the_same_object (const Reference <V, W> & other) const;
			void reset_pointee (T * new_pointee = NULL);

			bool has_parent () const;
			void set_parent (U * new_parent = NULL);

		private :
			template <class V, class W> friend class Reference;
		
			T * pointee;

			///	'parent' points to an object that allows me to self-destruct,
			///	upon destruction of the pointee.
			U * parent;
	};
}

//	#include "../cpp/reference.cpp"

#endif	//	RADAKAN_REFERENCE_HPP
