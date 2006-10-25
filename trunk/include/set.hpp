#ifndef SET_HPP
#define SET_HPP

#include "object.hpp"

using namespace std;

template <typename T> class Set:
	public virtual Object
{
	public:
		Set (string new_name);
		virtual ~Set ();
		virtual bool is_initialized () const;

		virtual bool add (T * t);				//	true iff succes
		virtual bool contains (T * t, bool recursive) const;
		virtual bool move_to (T * t, Set <T> * other_set);
															//	true iff succes
		//	returns a child of type U, if possible.
		template <typename U> U * get_child () const;

		set <T *> children;
};

#include "set.ipp"

#endif	//	SET_HPP