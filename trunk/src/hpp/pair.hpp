#ifndef RADAKAN_PAIR_HPP
#define RADAKAN_PAIR_HPP

#include "slot.hpp"

using namespace std;

namespace Radakan
{
	template <typename T, class U> class Pair :
		public Slot <U>
	{
		public :
			static string get_class_name ();
			
			Pair (const T & new_first, const Reference <U> & second);
			Pair (const Pair <T, U> & other);
			virtual ~Pair ();

			const T first;
	};
}

#endif	//	RADAKAN_PAIR_HPP
