#ifndef RADAKAN_TOUGHT_HPP
#define RADAKAN_TOUGHT_HPP

#include "object.hpp"

using namespace std;

namespace Radakan
{
	namespace Items
	{
		class Character;
	}
	
	class Tought :
		public Object
	{
		public :
			Tought (Reference <const Items :: Character> other);
			virtual ~Tought ();
			virtual bool is_initialized () const;

			static const string get_class_name ();

			const string about;
			
			float like;
			float fear;
	};
}

#endif	// RADAKAN_TOUGHT_HPP
