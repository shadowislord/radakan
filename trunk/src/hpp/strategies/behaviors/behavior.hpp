#ifndef RADAKAN_STRATEGIES_BEHAVIORS_BEHAVIOR_HPP
#define RADAKAN_STRATEGIES_BEHAVIORS_BEHAVIOR_HPP

#include "observer.hpp"
#include "strategies/strategy.hpp"

using namespace std;

class TiXmlAttribute;
class TiXmlDocument;
class TiXmlElement;

namespace Radakan
{
	namespace Items
	{
		class Character;
		class Item;
	}
	
	namespace Messages
	{
		namespace Communications
		{
			class Communication;
		}

		class Nothing;
	}
	
	namespace Strategies
	{
		namespace Behaviors
		{
			class Behavior :
				public Strategy <Behavior, Messages :: Nothing>,
				public Observer <Messages :: Communications :: Communication>
			{
				public :
					static string get_class_name ();
					
					Behavior (Reference <Items :: Character> new_character);
					virtual ~Behavior ();
					virtual bool is_initialized () const;
					
					virtual void call
						(Reference <Messages :: Communications :: Communication>
							message) = 0;
						
					virtual Reference <Behavior> transit
						(Reference <Messages :: Nothing> message) = 0;

					virtual bool evaluate_condition (const TiXmlElement * element) = 0;
					virtual bool evaluate_expression (const TiXmlAttribute * attribute) = 0;
					
					Reference <Items :: Character> character;

					///	At least one of the targets shouldn't target anything.
					Pointer <Items :: Character> character_target;
					Pointer <Items :: Item> item_target;
			};
		}
	}
}

#endif	//	RADAKAN_STRATEGIES_BEHAVIORS_BEHAVIOR_HPP
