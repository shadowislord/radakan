#ifndef TSLRPG_HPP
#define TSLRPG_HPP 

#include "battle_engine.hpp"
#include "sl_frame_listener.hpp"


using namespace Ogre;

class Tslrpg:
//	public ExampleApplication,
	public Object
{
	public:
		Tslrpg ();
		~Tslrpg ();
		void run ();

	private:
		void create_scene ();

		Root * root;
		Camera * camera;
		SceneManager * scene_mgr;
		Sl_Frame_Listener * frame_listener;
		RenderWindow * window;
		InputReader * input_device;
		Character * player;
		Battle_Engine battle_engine;
};

#endif
