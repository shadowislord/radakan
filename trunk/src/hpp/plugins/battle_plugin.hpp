#ifndef RADAKAN_BATTLE_PLUGIN_HPP
#define RADAKAN_BATTLE_PLUGIN_HPP

#include <boost/random/lognormal_distribution.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/random/linear_congruential.hpp>

#include "movable_model.hpp"
#include "play_state.hpp"
#include "player_character.hpp"

using namespace std;
using namespace boost;

namespace Radakan
{
	namespace Plugins
	{
		
		///	Battle_Plugin will soon be redesigned.
		class Battle_Plugin :
//			public Singleton <Battle_Plugin>,
			public Observer <Strategies :: Play_State>
		{
			public :
				Battle_Plugin ();
				virtual ~Battle_Plugin ();
				virtual bool is_initialized () const;
				
				static const string get_class_name ();

				virtual void call (const Object & message);

			private :
				const int max_distance;
				minstd_rand generator;
				uniform_real <> uniform_real_distribution;
				lognormal_distribution <> lognormal_real_distribution;
				variate_generator <minstd_rand&, uniform_real <> > uniform;
				variate_generator <minstd_rand&, lognormal_distribution <> > lognormal;
		};
	}
}

#endif	// RADAKAN_BATTLE_PLUGIN_HPP
