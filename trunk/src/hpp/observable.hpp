#ifndef TSL_OBSERVABLE_HPP
#define TSL_OBSERVABLE_HPP

#include "observer.hpp"
#include "disjoint_set.hpp"

using namespace std;

namespace tsl
{
	template <class T> class Observable :
		public virtual Object
	{
		public :
			//	protected constructor, see below
			virtual ~Observable ();
			virtual bool is_initialized () const;
			static string get_class_name ();

		protected :
			Observable ();
	};
}

#endif	//	TSL_OBSERVABLE_HPP
