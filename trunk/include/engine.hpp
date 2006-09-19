#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "object.hpp"

using namespace std;

//	!!!	there's currently no use for this class

class Engine:
	public Object
{
	public:
		Engine (string new_name);
		virtual ~Engine ();
		virtual bool is_initialized () const;
};

#endif	//	ENGINE_HPP
