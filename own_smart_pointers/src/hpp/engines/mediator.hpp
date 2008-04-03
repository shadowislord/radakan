#ifndef RADAKAN_ENGINES_MEDIATOR_HPP
#define RADAKAN_ENGINES_MEDIATOR_HPP

#include "singleton.hpp"

using namespace std;

namespace Radakan
{
	template <class T> class Observer;

	namespace Engines
	{
		
		///	TODO Add documentarion.
		class Mediator :
			public Singleton <Mediator>
		{
			public :
				Mediator ();
				virtual ~Mediator ();
				virtual bool is_initialized () const;

				static string get_class_name ();

				template <class T> void call_observers (Reference <T> message);
				template <class T> void register_observer
					(Reference <Observer <T> > observer);
		};
	}
}

#endif	//	RADAKAN_ENGINES_MEDIATOR_HPP
