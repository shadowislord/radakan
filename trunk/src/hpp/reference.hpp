#ifndef RADAKAN_REFERENCE_HPP
#define RADAKAN_REFERENCE_HPP

#include "reference_base.hpp"

using namespace std;

namespace Radakan
{
	template <class T> class Container;

	template <class T> class Reference :
		public Reference_Base
	{
		public :
			static string get_class_name ();
			
			bool operator== (const Reference <T> & other) const;
			template <class U> bool operator== (const Reference <U> & other) const;
			bool operator!= (const Reference <T> & other) const;
			bool operator< (const Reference <T> & other) const;

			Reference (T * new_pointee = NULL);
			Reference (const Reference <T> & other);
			template <class U> Reference (const Reference <U> & other);
			virtual ~Reference ();

			//	I will ask my parent to destruct me, if possible.
			//	Otherwise I will reset myself.
			//	So 'destruct ()' can be used as a safe way of forced destruction.
			virtual void destruct () const;

			virtual string get_name (bool suppress_debug_info = false) const;
			
			T * operator-> ();
			const T * operator-> () const;

			bool points_to_object () const;
			void set_parent (Container <T> & new_parent);

			template <class U> bool is_castable () const;
			template <class U> Reference <U> cast ();

			///	Don't confuse 'cast_const' with 'const_cast'.
			template <class U> const Reference <U> cast_const () const;

		private :
			template <class U> friend class Pointer;
			template <class U> friend class Reference;
		
			T * pointee;

			Container <T> * parent;
	};
}

#endif	//	RADAKAN_REFERENCE_HPP
