#ifndef TSL_QUIT_STATE_HPP
#define TSL_QUIT_STATE_HPP

#include "algorithm.hpp"
#include "singleton.hpp"

using namespace std;

namespace TSL
{
	class Game;

	class Quit_State :
		public Singleton <Quit_State>,
		public Algorithm <Game>
	{
		public :
			Quit_State ();
			virtual ~Quit_State ();
			virtual bool is_initialized () const;
			
			static const string get_class_name ();

			using Algorithm <Game> :: enter;
			using Algorithm <Game> :: exit;
	};
}

#endif	//	TSL_QUIT_STATE_HPP
