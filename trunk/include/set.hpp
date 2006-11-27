#ifndef SET_HPP
#define SET_HPP

#include "object.hpp"

using namespace std;

namespace sl
{
	template <typename T> class Set :
		public virtual Object
	{
		public :
			Set (string new_name);
			virtual ~Set ();
			virtual bool is_initialized () const;
			static string get_type_name ();

			virtual bool add (T & t);						//	true iff succes
			virtual bool contains (T & t, bool recursive) const;
			virtual bool move_to (T & t, Set <T> & other_set);
															//	true iff succes
			//	returns a child of type U, if possible.
			template <typename U> U * get_child () const;
			template <typename U> U * get_child (string name) const;

		protected:
			T * get_one_child () const;
			T * get_another_child () const;

		private:
			set <T *> children;
			
			//	Changes to next_child are ignored for const-ness.
			_Rb_tree_const_iterator <T *> next_child;
	};
}

#include "set.ipp"

#endif	//	SET_HPP
