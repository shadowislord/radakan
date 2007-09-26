#ifndef RADAKAN_REFERENCE_HPP
#define RADAKAN_REFERENCE_HPP

#include <cassert>
#include <string>

#include "reference_base.hpp"

using namespace std;

namespace Radakan
{
	template <class T> class Reference:
		public Reference_Base
	{
		public :
			Reference & operator= (const Reference & other);
			template <class V> Reference & operator= (const Reference <V> & other);
			template <class V> bool operator== (const Reference <V> & other) const;
			template <class V> bool operator!= (const Reference <V> & other) const;
			bool operator< (const Reference <T> & other) const;

			Reference (T * new_pointee = NULL);
			Reference (const Reference <T> & other);
			template <class V> Reference (const Reference <V> & other);
			virtual ~Reference ();

			static const string get_class_name ();
			
			virtual const string get_name () const;
			
			T * operator-> ();
			const T * operator-> () const;

			bool points_to_object () const;
			void reset_pointee (T * new_pointee = NULL);

		private :
			template <class V> friend class Reference;
		
			T * pointee;
	};
}

//	#include "../cpp/reference.cpp"

#endif	//	RADAKAN_REFERENCE_HPP
