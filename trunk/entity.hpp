#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "object.hpp"

using namespace std;

//	This is the base class for all in-game visual classes

class Tsl_Entity:
	public Object
{
	public:
		Tsl_Entity (string new_name);
		virtual ~Tsl_Entity ();
		virtual bool is_initialized () const;
};

#endif
