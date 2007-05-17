#ifndef TSL_OBSERVABLE_HPP
#define TSL_OBSERVABLE_HPP

#include "observer.hpp"
#include "set.hpp"

using namespace std;

namespace TSL
{
	template <class T> class Observable :
		public virtual Object
	{
		public :
			//	protected constructor, see below
			virtual ~Observable ();
			virtual bool is_initialized () const;
			
			static const string get_class_name ();

			Set <Observer <T> > observers;
			
		protected :
			Observable ();
	};
}

#endif	//	TSL_OBSERVABLE_HPP
