#ifndef RADAKAN_PLAY_STATE_HPP
#define RADAKAN_PLAY_STATE_HPP

#include "strategy.hpp"
#include "singleton.hpp"

using namespace std;

namespace Ogre
{
	class Camera;
	class SceneManager;
}

namespace Radakan
{
	class GUI;
	
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
				static string get_class_name ();

				Play_State (boost :: shared_ptr <Ogre :: SceneManager> scene_manager);
				virtual ~Play_State ();
				virtual bool is_initialized () const;
				
				virtual Reference <Strategy> transit (const Reference <Object> message);

			private :
				Reference <GUI> gui;
				
				boost :: scoped_ptr <Ogre :: Camera> camera;
		};
	}
}

#endif	//	RADAKAN_PLAY_STATE_HPP
