#ifndef RADAKAN_OBSERVABLE_HPP
#define RADAKAN_OBSERVABLE_HPP

#include "observer.hpp"
#include "set.hpp"

using namespace std;

namespace Radakan
{

	///	Observable is an abstract base class for the observer pattern.
	template <class T> class Observable :
		public virtual Object
	{
		public :
			//	protected constructor, see below
			virtual ~Observable ();
			virtual bool is_initialized () const;
			
			static const string get_class_name ();

			void call_observers (const Object & message);

			void register_observer (Observer <T> & observer);
			void drop_observer (Observer <T> & observer);

		protected :
			Observable ();

		private :
			Set <Observer <T> > observers;
	};
}

#endif	//	RADAKAN_OBSERVABLE_HPP
