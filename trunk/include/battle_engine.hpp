#ifndef BATTLE_ENGINE_HPP
#define BATTLE_ENGINE_HPP

#include "character.hpp"
#include <boost/random/lognormal_distribution.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/random/linear_congruential.hpp>

using namespace std;
using namespace boost;

///	Battle_engine is called by the Frame_Listener.

class Battle_Engine:
	public Object
{
	public:
		Battle_Engine ();
		virtual ~Battle_Engine ();
		virtual bool is_initialized () const;
		string hit (Character * attacker, Character * defender);

	private:
		minstd_rand generator;
		uniform_real <> uniform_real_distribution;
		lognormal_distribution <> lognormal_real_distribution;
		variate_generator <minstd_rand&, uniform_real <> > uniform;
		variate_generator <minstd_rand&, lognormal_distribution <> > lognormal;
};

#endif	//	BATTLE_ENGINE_HPP
