#ifndef TSLRPG_HPP
#define TSLRPG_HPP 

#include "lost.hpp"

class Tslrpg:
	public ExampleApplication
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
