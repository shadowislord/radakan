#include "framelistener.hpp"

FrameLstr::
	FrameLstr (SceneManager * sceneMgr, RenderWindow * win, Camera * cam): ExampleFrameListener (win, cam),
	mSceneMgr (sceneMgr),
	Object ("FrameListener")
{
	mSceneMgr = sceneMgr;
	totaltime = 0;
}

bool FrameLstr::frameStarted (const FrameEvent& evt)
{
	totaltime += evt.timeSinceLastFrame;
	bool ret = ExampleFrameListener::frameStarted(evt);
	return ret;
}

bool FrameLstr::frameEnded (const FrameEvent& evt)
{
	bool ret = ExampleFrameListener::frameEnded(evt);
	return ret;
}
