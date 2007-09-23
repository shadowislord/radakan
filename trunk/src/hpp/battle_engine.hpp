#ifndef RADAKAN_BATTLE_ENGINE_HPP
#define RADAKAN_BATTLE_ENGINE_HPP

#include <boost/random/lognormal_distribution.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/random/linear_congruential.hpp>

#include "singleton.hpp"

using namespace std;
using namespace boost;

namespace Radakan
{
	namespace Engines
	{
		
		///	Battle_Engine will soon be redesigned.
		class Battle_Engine :
			public Singleton <Battle_Engine>
		{
			public :
				Battle_Engine ();
				virtual ~Battle_Engine ();
				virtual bool is_initialized () const;
				
				static const string get_class_name ();

				virtual void call (Reference <const Object> message = Object :: update);

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

#endif	// RADAKAN_BATTLE_ENGINE_HPP
