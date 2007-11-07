
#ifndef Bounded_Float_HPP
#define Bounded_Float_HPP

using namespace std;

namespace Radakan
{

	namespace Mathematics
	{

		class Bounded_Float
		{
			public :
                Bounded_Float(float max);
                Bounded_Float(float val,float min,float max);
                ~Bounded_Float();

                float get_min() const;
                float get_max() const;
				float get_val() const;
				
                void set_val(float);
                void set_min(float);
                void set_max(float);


				Bounded_Float operator=(float);
                Bounded_Float operator=(const Bounded_Float &);
                Bounded_Float operator+(float);
                Bounded_Float operator+(const Bounded_Float &);
                Bounded_Float operator-(float);
                Bounded_Float operator-(const Bounded_Float &);
                Bounded_Float operator*(float);
                Bounded_Float operator*(const Bounded_Float &);
                Bounded_Float operator/(float);
                Bounded_Float operator/(const Bounded_Float &);



			private :
                float min;
                float max;
                float val;

		};
	}
}

#endif	//	Bounded_Float_HPP
