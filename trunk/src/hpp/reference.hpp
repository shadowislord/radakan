#ifndef RADAKAN_REFERENCE_HPP
#define RADAKAN_REFERENCE_HPP

#include <cassert>
#include <string>

#include "reference_base.hpp"

using namespace std;

namespace Radakan
{
	template <class T> class Set;

	template <class T> class Reference :
		public Reference_Base
	{
		public :
			static string get_class_name ();
			
			Reference & operator= (const Reference & other);
			template <class U> Reference & operator= (const Reference <U> & other);
			bool operator== (const Reference <T> & other) const;
			template <class U> bool operator== (const Reference <U> & other) const;
			bool operator!= (const Reference <T> & other) const;
			bool operator< (const Reference <T> & other) const;

			Reference (T * new_pointee = NULL);
			Reference (const Reference <T> & other);
			template <class U> Reference (const Reference <U> & other);
			//	When I have a parent, use 'destruct_from_parent' to delete me.
			virtual ~Reference ();
			virtual void destruct_from_parent () const;

			virtual bool is_initialized () const;

			virtual string get_name () const;
			
			T * operator-> ();
			const T * operator-> () const;

			bool points_to_object () const;
			virtual void reset_pointee ();
			void reset_pointee (T * new_pointee);

			void set_parent (Set <T> & new_parent);
			virtual bool has_parent () const;
			
			template <class U> bool is_castable () const;
			template <class U> Reference <U> cast ();

			///	Don't confuse 'cast_const' with 'const_cast'.
			template <class U> const Reference <U> cast_const () const;

		private :
			template <class U> friend class Reference;
		
			T * pointee;

			Set <T> * parent;
	};
}

#endif	//	RADAKAN_REFERENCE_HPP
