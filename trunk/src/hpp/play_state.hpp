#ifndef RADAKAN_PLAY_STATE_HPP
#define RADAKAN_PLAY_STATE_HPP

#include "strategy.hpp"
#include "gui.hpp"
#include "world.hpp"

using namespace std;

namespace Radakan
{
	namespace Engines
	{
		class Game;
	}
	
	namespace Strategies
	{

		class Play_State :
			public Singleton <Play_State>,
			public Strategy
		{
			public :
				Play_State (Ogre :: SceneManager & scene_manager);
				virtual ~Play_State ();
				virtual bool is_initialized () const;
				
				static const string get_class_name ();

				virtual Strategy * transit (const Object & message);

			private :
				GUI & gui;
				
				Ogre :: Camera & camera;
		};
	}
}

#endif	//	RADAKAN_PLAY_STATE_HPP
