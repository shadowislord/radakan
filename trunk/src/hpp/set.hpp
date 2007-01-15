#ifndef SET_HPP
#define SET_HPP

#include "object.hpp"

using namespace std;

namespace tsl
{

	///	Set is a generic container, but every Object can't be in more then one Set at once.

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

		protected:
			//	These methods can take a while, in case of a large Set.
			T * get_child (string name, bool recursive) const;
			template <typename U> U * get_typed_child () const;
			template <typename U> U * get_typed_child (string name) const;

			//	These two methods make it very easy to get a pointer to each child:
			T * get_child () const;
			T * get_another_child () const;

		private:
			set <T *> children;
		
			//	'mutable' added to allow change even if in a const Set.
			//	'typename' added to assure that const_iterator is a type. (MSV warning C4346)
			mutable typename set <T *> :: const_iterator next_child;
		};
}

#endif	//	SET_HPP
