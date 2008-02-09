#ifndef RADAKAN_STRATEGIES_BEHAVIOR_PLAYER_HPP
#define RADAKAN_STRATEGIES_BEHAVIOR_PLAYER_HPP

#include "mathematics/bounded_float.hpp"
#include "strategies/behaviors/behavior.hpp"

using namespace std;

namespace Radakan
{
	namespace Items
	{
		class Character;
	}
	
	namespace Messages
	{
		class Nothing;
	}
	
	namespace Strategies
	{
		namespace Behaviors
		{
			class Player :
				public Behavior,
				public Singleton <Player>
			{
				private :
					///	in radians
					static const float min_vertical_camera_angle;
					
					///	in radians
					static const float max_vertical_camera_angle;

				public :
					static string get_class_name ();

					Player (Reference <Items :: Character> new_character);
					virtual ~Player ();
					virtual bool is_initialized () const;

					///	The message is showed.
					virtual void call
						(Reference <Messages :: Communications :: Communication> message);

					virtual Reference <Behavior> transit
						(Reference <Messages :: Nothing> message);
			
					virtual bool evaluate_condition (const TiXmlElement * element);
					virtual bool evaluate_expression (const TiXmlAttribute * attribute);
					
					const Mathematics :: Vector_3D get_camera_position () const;
					const Mathematics :: Quaternion get_camera_orientation () const;

					void list_communication_options ();

				private :
					void load_communication_options (const TiXmlElement * element);
						
					float camera_distance;

					///	in radians
					Mathematics :: Bounded_Float vertical_camera_angle;
			};
		}
	}
}

#endif	//	RADAKAN_STRATEGIES_BEHAVIOR_PLAYER_HPP
