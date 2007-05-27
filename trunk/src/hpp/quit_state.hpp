#ifndef TSL_QUIT_STATE_HPP
#define TSL_QUIT_STATE_HPP

#include "algorithm.hpp"
#include "singleton.hpp"

using namespace std;

namespace TSL
{
	namespace Engines
	{
		class Game;
	}

	namespace Algorithms
	{

		///	Quit_State is the algorithm for the game when quiting.
		class Quit_State :
			public Singleton <Quit_State>,
			public Algorithm <Engines :: Game>
		{
			public :
				Quit_State ();
				virtual ~Quit_State ();
				virtual bool is_initialized () const;
				
				static const string get_class_name ();

				virtual Algorithm <Engines :: Game> & transit (Engines :: Game & owner, const Object & message);

				using Algorithm <Engines :: Game> :: enter;
				using Algorithm <Engines :: Game> :: exit;
		};
	}
}

#endif	//	TSL_QUIT_STATE_HPP
