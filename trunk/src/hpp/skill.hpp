#ifndef RADAKAN_SKILL_HPP
#define RADAKAN_SKILL_HPP

#include "object.hpp"
#include "mathematics/bounded_float.hpp"

using namespace std;

namespace Radakan
{
	
	class Skill :
		public Object
	{
		public :
			static string get_class_name ();

			static const float default_experience;

			Skill (string new_name);
			virtual ~Skill ();
			virtual bool is_initialized () const;

			void add_experience (float additional = 1);

			float get_value () const;

		private :
			Mathematics :: Bounded_Float experience;
	};
}

#endif	// RADAKAN_SKILL_HPP
