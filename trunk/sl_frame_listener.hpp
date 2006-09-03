#ifndef SL_FRAME_LISTENER_HPP
#define SL_FRAME_LISTENER_HPP

#include "object.hpp"
#include <Ogre.h>
#include <OgreKeyEvent.h>
#include <OgreEventListeners.h>
#include <OgreStringConverter.h>
#include <OgreException.h>

using namespace std;

class Sl_Frame_Listener:
	public Ogre :: FrameListener,
	public Ogre :: KeyListener,
//	public Ogre :: MouseListener,	//	doesn't work (yet)
	public Object
{
	public:
		// constructor takes a RenderWindow because it uses that to determine input context
		Sl_Frame_Listener
			(Ogre :: RenderWindow * new_window,
			Ogre :: Camera * new_camera,
			bool useBufferedInputKeys,
			bool useBufferedInputMouse);	//	these bools are default false
    	virtual ~Sl_Frame_Listener ();
		virtual bool frameStarted (const Ogre :: FrameEvent & evt);
		virtual bool frameEnded (const Ogre :: FrameEvent & evt);
		virtual bool processUnbufferedKeyInput (const Ogre :: FrameEvent & evt);
		bool processUnbufferedMouseInput (const Ogre :: FrameEvent & evt);
		void moveCamera ();
		void showDebugOverlay (bool show);
		void switchMouseMode ();
		void switchKeyMode ();

		void keyClicked (Ogre :: KeyEvent* e);
		void keyPressed (Ogre :: KeyEvent* e);
		void keyReleased (Ogre :: KeyEvent* e);

	private:
		float total_time;
		Ogre :: Vector3 vec;

		int scene_detail_index ;
		Ogre :: Real move_speed;
		Ogre :: Degree rotate_speed;
		Ogre :: Overlay * debug_overlay;

		void updateStats();

		Ogre :: EventProcessor * event_processor;
		Ogre :: InputReader * input_device;
		Ogre :: Camera * camera;

		Ogre :: Vector3 mTranslateVector;
		Ogre :: RenderWindow * window;
		bool mStatsOn;
		bool mUseBufferedInputKeys;
		bool mUseBufferedInputMouse;
		bool mInputTypeSwitchingOn;
		unsigned int mNumScreenShots;
		float mMoveScale;
		Ogre :: Degree mRotScale;
		// just to stop toggles flipping too fast
		Ogre :: Real mTimeUntilNextToggle ;
		Ogre :: Radian mRotX, mRotY;
		Ogre :: TextureFilterOptions mFiltering;
		int mAniso;
};

#endif	//	SL_FRAME_LISTENER_HPP
