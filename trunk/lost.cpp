#include "lost.hpp"

LFrameListener::
	LFrameListener (SceneManager * sceneMgr, RenderWindow * win, Camera * cam): ExampleFrameListener (win, cam),
	mSceneMgr (sceneMgr)
{
	mSceneMgr = sceneMgr;
	totaltime = 0;
}

bool LFrameListener::frameStarted (const FrameEvent& evt)
{
	totaltime += evt.timeSinceLastFrame;
	bool ret = ExampleFrameListener::frameStarted(evt);
	return ret;
}

bool LFrameListener::frameEnded (const FrameEvent& evt)
{
	bool ret = ExampleFrameListener::frameEnded(evt);
	return ret;
}
