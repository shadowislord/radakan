#ifndef RADAKAN_ENGINES_MEDIATOR_IMPLEMENTATION_HPP
#define RADAKAN_ENGINES_MEDIATOR_IMPLEMENTATION_HPP

#include "observable.hpp"
#include "singleton.hpp"

namespace Radakan
{
	namespace Messages
	{
		class Button_Event;
		
		namespace Communications
		{
			class Communication;
		}
		
		class List_Event;
		class List_Update;
		class Nothing;
	}

	namespace Engines
	{
		
		//	TODO Add documentarion.
		class Mediator_Implementation :
			public Observable <Messages :: Button_Event>,
			public Observable <Messages :: Communications :: Communication>,
			public Observable <Messages :: List_Event>,
			public Observable <Messages :: List_Update>,
			public Observable <Messages :: Nothing>,
			public Singleton <Mediator_Implementation>
		{
			public :
				Mediator_Implementation ();
				virtual ~Mediator_Implementation ();
				virtual bool is_initialized () const;

				static string get_class_name ();
		};
	}
}

#endif	//	RADAKAN_ENGINES_MEDIATOR_IMPLEMENTATION_HPP
