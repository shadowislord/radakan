#ifndef RADAKAN_REFERENCE_HPP
#define RADAKAN_REFERENCE_HPP

#include <cassert>
#include <string>

#include "reference_base.hpp"

using namespace std;

namespace Radakan
{
	template <class X> class Set;

	template <class T> class Reference :
		public Reference_Base
	{
		public :
			static string get_class_name ();
			
			Reference & operator= (const Reference & other);
			template <class V> Reference & operator= (const Reference <V> & other);
			bool operator== (const Reference <T> & other) const;
			template <class V> bool operator== (const Reference <V> & other) const;
			bool operator!= (const Reference <T> & other) const;
			bool operator< (const Reference <T> & other) const;

			Reference (T * new_pointee = NULL);
			Reference (const Reference <T> & other);
			template <class V> Reference (const Reference <V> & other);
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
			
		private :
			template <class V> friend class Reference;
		
			T * pointee;

			Set <T> * parent;
	};
}

#endif	//	RADAKAN_REFERENCE_HPP
