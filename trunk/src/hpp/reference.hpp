#ifndef RADAKAN_REFERENCE_HPP
#define RADAKAN_REFERENCE_HPP

#include <cassert>
#include <string>

#include "reference_base.hpp"

using namespace std;

namespace Radakan
{
	template <class X> class Set;

	template <class T> class Reference:
		public Reference_Base
	{
		public :
			static string get_class_name ();
			
			Reference & operator= (const Reference & other);
			template <class V> Reference & operator= (const Reference <V> & other);
			template <class V> bool operator== (const Reference <V> & other) const;
			bool operator!= (const Reference <T> & other) const;
			bool operator< (const Reference <T> & other) const;

			Reference (T * new_pointee = NULL);
			Reference (const Reference <T> & other);
			template <class V> Reference (const Reference <V> & other);
			virtual ~Reference ();

			virtual bool is_initialized () const;

			virtual string get_name () const;
			
			T * operator-> ();
			const T * operator-> () const;

			bool points_to_object () const;
			void reset_pointee (T * new_pointee = NULL);

			void set_parent (Set <T> & new_parent);

		private :
			template <class V> friend class Reference;
		
			T * pointee;

			Reference <Set <T> > * parent;
	};
}

//	#include "../cpp/reference.cpp"

#endif	//	RADAKAN_REFERENCE_HPP
