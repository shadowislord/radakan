#ifndef BOUNDED_FLOAT_HPP
#define BOUNDED_FLOAT_HPP

#include "base.hpp"

using namespace std;

namespace Radakan
{
	namespace Mathematics
	{
		extern const float infinite;

		class Bounded_Float
		{
			public :
				Bounded_Float (float val, float min, float max);
				~Bounded_Float ();

				float get_value () const;
				
				void set_value (float);

				Bounded_Float operator= (float);
				Bounded_Float operator= (const Bounded_Float &);
				
				Bounded_Float operator+= (float);
				
				Bounded_Float operator*= (float);

				bool operator> (float) const;
				bool operator> (const Bounded_Float &) const;
				bool operator>= (float) const;
				bool operator>= (const Bounded_Float &) const;
				bool operator== (float) const;
				bool operator== (const Bounded_Float &) const;
				bool operator<= (float) const;
				bool operator<= (const Bounded_Float &) const;
				bool operator< (float) const;
				bool operator< (const Bounded_Float &) const;
				float operator+ (float) const;
				float operator+ (const Bounded_Float &) const;
				float operator- (float) const;
				float operator- (const Bounded_Float &) const;
				float operator* (float) const;
				float operator* (const Bounded_Float &) const;
				float operator/ (float) const;
				float operator/ (const Bounded_Float &) const;
				
				const float minimum;
				const float maximum;

			private :
				float value;
		};
	}
}

#endif	//	BOUNDED_FLOAT_HPP
