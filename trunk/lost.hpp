#ifndef LOST_HPP
#define LOST_HPP

#include "object.hpp"

class LFrameListener:
	public ExampleFrameListener
{
	private:
		SceneManager* mSceneMgr;
		float totaltime;
		Vector3 vec;

	public:
		LFrameListener(SceneManager *sceneMgr, RenderWindow* win, Camera* cam);
		bool frameStarted(const FrameEvent& evt);
		bool frameEnded(const FrameEvent& evt);

};


#endif	//	LOST_HPP
