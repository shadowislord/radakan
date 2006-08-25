#ifndef SL_ENTITY_HPP
#define SL_ENTITY_HPP

#include "object.hpp"

using namespace std;

//	This is the base class for all in-game visual classes

class Sl_Entity:
	public Object
{
	public:
		Sl_Entity (string new_name);
		virtual ~Sl_Entity ();
		virtual bool is_initialized () const;
};

#endif
