#ifndef FRAMELISTENER_HPP
#define FRAMELISTENER_HPP

#include "object.hpp"

//	Class name 'FrameListener' is alreayd in use by Ogre.
class FrameLstr:
	public ExampleFrameListener,
	public Object
{
	public:
		FrameLstr (SceneManager * sceneMgr, RenderWindow * win, Camera * cam);
		bool frameStarted (const FrameEvent& evt);
		bool frameEnded (const FrameEvent& evt);

	private:
		SceneManager * mSceneMgr;
		float totaltime;
		Vector3 vec;

};


#endif	//	FRAMELISTENER_HPP
