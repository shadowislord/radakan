#ifndef RADAKAN_STRATEGY_HPP
#define RADAKAN_STRATEGY_HPP

#include "resident.hpp"

using namespace std;

namespace Radakan
{
	namespace Strategies
	{

		///	I'm the abstract base class for the strategy pattern.
		///	For more information about the strategy pattern,
		///	see http://en.wikipedia.org/wiki/Strategy_pattern .
		///	Use me in combination with the Strategy_State_Machine class.
		class Strategy :
			public Resident <Strategy>
		{
			public :
				//	protected constructor, see below
				virtual ~Strategy ();
				virtual bool is_initialized () const;
				
				static const string get_class_name ();

				///	Don't call this method directly,
				///	let it be called by Strategy_State_Machine.
				///	I return myself to continue next time.
				///	I return NULL when I should be destructed.
				virtual Strategy * transit (const Object & message) = 0;
				
			protected :
				Strategy ();
		};
	}
}

#endif	//	RADAKAN_STRATEGY_HPP
