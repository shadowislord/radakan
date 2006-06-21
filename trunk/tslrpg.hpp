#ifndef TSLRPG_HPP
#define TSLRPG_HPP 

#include "framelistener.hpp"

class Tslrpg:
	public ExampleApplication,
	public Object
{
	public:
		Tslrpg ();
		~Tslrpg ();

	protected:
		virtual void createCamera ();
		virtual void createScene ();
		void createFrameListener ();
};

#endif
