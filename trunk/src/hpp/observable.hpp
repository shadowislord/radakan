#ifndef RADAKAN_OBSERVABLE_HPP
#define RADAKAN_OBSERVABLE_HPP

#include "set.hpp"

using namespace std;

namespace Radakan
{
	template <class T> class Observer;

	///	Observable is an abstract base class for the observer pattern.
	template <class T> class Observable :
		public virtual Object
	{
		public :
			//	protected constructor, see below
			virtual ~Observable ();
			virtual bool is_initialized () const;
			
			static string get_class_name ();

			void call_observers (const Reference <Object> message = update);

			void register_observer (Reference <Observer <T> > observer);
			void drop_observer (Reference <Observer <T> > observer);

		protected :
			Observable ();

		private :
			Reference <Set <Observer <T> > > observers;
	};
}

#endif	//	RADAKAN_OBSERVABLE_HPP
