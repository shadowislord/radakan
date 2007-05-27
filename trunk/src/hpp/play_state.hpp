#ifndef TSL_PLAY_STATE_HPP
#define TSL_PLAY_STATE_HPP

#include "algorithm.hpp"
#include "gui.hpp"
#include "world.hpp"

using namespace std;

namespace TSL
{
	namespace Engines
	{
		class Game;
	}
	
	namespace Algorithms
	{

		class Play_State :
			public Singleton <Play_State>,
			public Algorithm <Engines :: Game>,
			public Observable <Play_State>
		{
			public :
				Play_State (Ogre :: SceneManager & scene_manager, string tsl_path);
				virtual ~Play_State ();
				virtual bool is_initialized () const;
				
				static const string get_class_name ();

				virtual Algorithm <Engines :: Game> & transit (Engines :: Game & owner, const Object & message);

			private :
				virtual void enter (Engines :: Game & owner);
				
				GUI & gui;
				
				Ogre :: Camera & camera;
		};
	}
}

#endif	//	TSL_PLAY_STATE_HPP
