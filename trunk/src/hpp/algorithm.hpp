#ifndef TSL_ALGORITHM_HPP
#define TSL_ALGORITHM_HPP

#include "resident.hpp"

using namespace std;

namespace TSL
{
	namespace Algorithms
	{

		///	Algorithm is the abstract base class for all algorithms.
		class Algorithm :
			public Resident <Algorithm>
		{
			public :
				//	protected constructor, see below
				virtual ~Algorithm ();
				virtual bool is_initialized () const;
				
				static const string get_class_name ();

				///	I'll self-destruct, if appropriate.
				virtual void transit (const Object & message) = 0;
				
			protected :
				Algorithm ();
		};
	}
}

#endif	//	TSL_ALGORITHM_HPP
