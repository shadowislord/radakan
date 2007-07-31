#ifndef TSL_PLAY_STATE_HPP
#define TSL_PLAY_STATE_HPP

#include "strategy.hpp"
#include "gui.hpp"
#include "world.hpp"

using namespace std;

namespace TSL
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

				CEGUI :: Listbox * chat_window;
		};
	}
}

#endif	//	TSL_PLAY_STATE_HPP
