#ifndef RADAKAN_OPINION_HPP
#define RADAKAN_OPINION_HPP

#include "object.hpp"

using namespace std;

namespace Radakan
{
	namespace Items
	{
		namespace Characters
		{
			class Character;
		}
	}
	
	class Opinion :
		public Object
	{
		public :
			static string get_class_name ();

			Opinion (const Reference <Items :: Characters :: Character> & other);
			virtual ~Opinion ();
			virtual bool is_initialized () const;

			const string about;
			
			float like;
			float fear;
	};
}

#endif	// RADAKAN_OPINION_HPP
