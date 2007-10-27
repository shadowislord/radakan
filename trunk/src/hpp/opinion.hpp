#ifndef RADAKAN_OPINION_HPP
#define RADAKAN_OPINION_HPP

#include "object.hpp"

using namespace std;

namespace Radakan
{
	namespace Items
	{
		class Character;
	}
	
	class Opinion :
		public Object
	{
		public :
			Opinion (const Reference <Items :: Character> & other);
			virtual ~Opinion ();
			virtual bool is_initialized () const;

			static string get_class_name ();

			const string about;
			
			float like;
			float fear;
	};
}

#endif	// RADAKAN_OPINION_HPP
