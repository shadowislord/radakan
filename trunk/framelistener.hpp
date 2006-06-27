#ifndef FRAMELISTENER_HPP
#define FRAMELISTENER_HPP

#include "object.hpp"
#include <OgreKeyEvent.h>
#include <OgreEventListeners.h>
#include <OgreStringConverter.h>
#include <OgreException.h>

using namespace std;
using namespace Ogre;

//	Class name 'FrameListener' is alreayd in use by Ogre.
class FrameLstr:
	public FrameListener,
	public KeyListener,
//	public MouseListener,	//	doesn't work yet
	public Object
{
	public:
		// Constructor takes a RenderWindow because it uses that to determine input context
		FrameLstr (SceneManager * new_scene_mgr, RenderWindow * new_window,
			Camera * new_camera, bool useBufferedInputKeys,
			bool useBufferedInputMouse);	//	these bools are default false
    	virtual ~FrameLstr ();
		bool frameStarted (const FrameEvent& evt);
		bool frameEnded (const FrameEvent& evt);
		virtual bool processUnbufferedKeyInput(const FrameEvent& evt);
		bool processUnbufferedMouseInput(const FrameEvent& evt);
		void moveCamera();
		void showDebugOverlay(bool show);
		void switchMouseMode();
		void switchKeyMode();

		void keyClicked(KeyEvent* e);
		void keyPressed(KeyEvent* e);
		void keyReleased(KeyEvent* e);

	private:
		SceneManager * scene_mgr;
		float total_time;
		Vector3 vec;

		int scene_detail_index ;
		Real move_speed;
		Degree rotate_speed;
		Overlay* debug_overlay;

		void updateStats();

		EventProcessor* event_processor;
		InputReader* input_device;
		Camera* camera;

		Vector3 mTranslateVector;
		RenderWindow* window;
		bool mStatsOn;
		bool mUseBufferedInputKeys, mUseBufferedInputMouse, mInputTypeSwitchingOn;
		unsigned int mNumScreenShots;
		float mMoveScale;
		Degree mRotScale;
		// just to stop toggles flipping too fast
		Real mTimeUntilNextToggle ;
		Radian mRotX, mRotY;
		TextureFilterOptions mFiltering;
		int mAniso;
};

#endif	//	FRAMELISTENER_HPP
