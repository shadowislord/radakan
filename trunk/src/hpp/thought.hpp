#ifndef RADAKAN_THOUGHT_HPP
#define RADAKAN_THOUGHT_HPP

#include "object.hpp"

using namespace std;

namespace Radakan
{
	namespace Items
	{
		class Character;
	}
	
	class Thought :
		public Object
	{
		public :
			Thought (const Reference <Items :: Character> other);
			virtual ~Thought ();
			virtual bool is_initialized () const;

			static string get_class_name ();

			const string about;
			
			float like;
			float fear;
	};
}

#endif	// RADAKAN_THOUGHT_HPP
