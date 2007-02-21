#ifndef TSL_TSL_HPP
#define TSL_TSL_HPP

#include "algorithm_state_machine.hpp"
#include "representation.hpp"

using namespace std;

namespace tsl
{

	///	TSL is the main class. It manages the eninges and the sectors.

	class TSL :
		public Singleton <TSL>,
		public Algorithm_State_Machine <TSL>
	{
		public :
			TSL (string tsl_path, string ogre_media_path);
			~TSL ();
			virtual bool is_initialized () const;
			static string get_class_name ();

			void run ();
			void set_camera (Ogre :: Camera & new_camera);
			string get_FPS () const;

			///	in milliseconds
			const unsigned long & get_last_turn_lenght () const;

			Ogre :: RenderWindow * window;

		private :
			unsigned long last_turn_lenght;
			Ogre :: Root * root;
			Ogre :: Timer * turn_lenght_timer;
	};
}

#endif	//	TSL_TSL_HPP
