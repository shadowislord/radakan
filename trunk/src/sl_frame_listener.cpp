#include "sl_frame_listener.hpp"

Sl_Frame_Listener ::
	Sl_Frame_Listener
		(Ogre :: RenderWindow * new_window,
		Ogre :: Camera * new_camera,
		bool useBufferedInputKeys,
		bool useBufferedInputMouse) :
//	mSceneMgr (new_scene_mgr),
	Object ("Frame Listener")
{
	assert (Object :: is_initialized ());

	total_time = 0;
	camera = new_camera;
	window = new_window;
	mStatsOn = true;
	mNumScreenShots = 0;
	mTimeUntilNextToggle = 0;
	scene_detail_index = 0;
	mMoveScale = 0.0f;
	mRotScale = 0.0f;
	mTranslateVector = Ogre :: Vector3 :: ZERO;
	mAniso = 1;
	mFiltering = Ogre :: TFO_BILINEAR;

	//showDebugOverlay (false);

	debug_overlay = Ogre :: OverlayManager :: getSingleton().getByName
														("Core/DebugOverlay");
	mUseBufferedInputKeys = useBufferedInputKeys;
	mUseBufferedInputMouse = useBufferedInputMouse;
	mInputTypeSwitchingOn = mUseBufferedInputKeys || mUseBufferedInputMouse;
	rotate_speed = 36;
	move_speed = 100;

	if (mInputTypeSwitchingOn)
	{
		event_processor = new Ogre :: EventProcessor ();
		event_processor -> initialise(window);
		event_processor -> startProcessingEvents ();
		event_processor -> addKeyListener (this);
		input_device = event_processor -> getInputReader ();
	}
	else
	{
		input_device = Ogre :: PlatformManager :: getSingleton ().createInputReader ();
		input_device -> initialise (window, true, true);
	}
	
	assert (is_initialized ());
}

//	virtual
Sl_Frame_Listener ::
	~Sl_Frame_Listener ()
{
	assert (is_initialized ());
	
	if (mInputTypeSwitchingOn)
	{
		delete event_processor;
	}
	else
	{
		Ogre :: PlatformManager :: getSingleton ().destroyInputReader
																(input_device);
	}
}

void Sl_Frame_Listener ::
	updateStats ()
{
	assert (is_initialized ());
	
	/*static string currFps = "Current FPS: ";
	static string avgFps = "Average FPS: ";
	static string bestFps = "Best FPS: ";
	static string worstFps = "Worst FPS: ";
	static string tris = "Triangle Count: ";

	// update stats when necessary
	try
	{
		Ogre :: OverlayElement * guiAvg = Ogre :: OverlayManager :: getSingleton ().getOverlayElement ("Core/AverageFps");
		Ogre :: OverlayElement * guiCurr = Ogre :: OverlayManager :: getSingleton ().getOverlayElement ("Core/CurrFps");
		Ogre :: OverlayElement* guiBest = Ogre :: OverlayManager :: getSingleton ().getOverlayElement ("Core/BestFps");
		Ogre :: OverlayElement* guiWorst = Ogre :: OverlayManager :: getSingleton ().getOverlayElement ("Core/WorstFps");

		const Ogre :: RenderTarget :: FrameStats & stats = window -> getStatistics ();

//		print << "FPS: " << stats.avgFPS << endl;

		guiAvg -> setCaption
			(avgFps + Ogre :: StringConverter :: toString (stats.avgFPS));
		guiCurr -> setCaption
			(currFps + Ogre :: StringConverter :: toString (stats.lastFPS));
		guiBest -> setCaption
			(bestFps + Ogre :: StringConverter :: toString (stats.bestFPS)
			+ " " + Ogre :: StringConverter :: toString (stats.bestFrameTime)
			+ " ms");
		guiWorst -> setCaption
			(worstFps + Ogre :: StringConverter :: toString (stats.worstFPS)
			+ " " + Ogre :: StringConverter :: toString (stats.worstFrameTime)
			+ " ms");

		Ogre :: OverlayElement * guiTris = Ogre :: OverlayManager :: getSingleton ().getOverlayElement ("Core/NumTris");
		guiTris -> setCaption (tris + Ogre :: StringConverter :: toString (stats.triangleCount));

		Ogre :: OverlayElement * guiDbg = Ogre :: OverlayManager :: getSingleton().getOverlayElement ("Core/DebugText");
		guiDbg -> setCaption (window -> getDebugText ());
	}
	catch (...)
	{
		// ignore
	}*/
}

//	virtual
bool Sl_Frame_Listener ::
	processUnbufferedKeyInput (const Ogre :: FrameEvent & evt)
{
	assert (is_initialized ());

	if (input_device -> isKeyDown (Ogre :: KC_A))
	{
		// Move camera left
		mTranslateVector.x = -mMoveScale;
	}

	if (input_device -> isKeyDown (Ogre :: KC_D))
	{
		// Move camera RIGHT
		mTranslateVector.x = mMoveScale;
	}

	// Move camera forward by keypress.
	if (input_device -> isKeyDown (Ogre :: KC_UP)
									|| input_device -> isKeyDown (Ogre :: KC_W))
	{
		mTranslateVector.z = -mMoveScale;
	}

	// Move camera backward by keypress.
	if (input_device -> isKeyDown (Ogre :: KC_DOWN)
									|| input_device -> isKeyDown (Ogre :: KC_S))
	{
		mTranslateVector.z = mMoveScale;
	}

	if (input_device -> isKeyDown (Ogre :: KC_PGUP))
	{
		// Move camera up
		mTranslateVector.y = mMoveScale;
	}

	if (input_device -> isKeyDown (Ogre :: KC_PGDOWN))
	{
		// Move camera down
		mTranslateVector.y = -mMoveScale;
	}

	if (input_device -> isKeyDown (Ogre :: KC_RIGHT))
	{
		camera -> yaw(-mRotScale);
	}
		
	if (input_device -> isKeyDown (Ogre :: KC_LEFT))
	{
		camera -> yaw(mRotScale);
	}

	if( input_device -> isKeyDown (Ogre :: KC_ESCAPE))
	{
		return false;
	}

	static bool displayCameraDetails = false;

	if (mTimeUntilNextToggle <= 0)
	{
		if (mInputTypeSwitchingOn)
		{
			// see if switching is on, and you want to toggle
			if (input_device -> isKeyDown (Ogre :: KC_M))
			{
				switchMouseMode();
				mTimeUntilNextToggle = 1;
			}
			if (input_device -> isKeyDown (Ogre :: KC_K))
			{
				// must be going from immediate keyboard to buffered keyboard
				switchKeyMode();
				mTimeUntilNextToggle = 1;
			}
		}
		
		if (input_device -> isKeyDown (Ogre :: KC_F))
		{
			mStatsOn = ! mStatsOn;
			showDebugOverlay (mStatsOn);
			mTimeUntilNextToggle = 1;
		}
		
		if (input_device -> isKeyDown (Ogre :: KC_T))
		{
			switch (mFiltering)
			{
				case Ogre :: TFO_BILINEAR :
				{
					mFiltering = Ogre :: TFO_TRILINEAR;
					mAniso = 1;
					break;
				}
				case Ogre :: TFO_TRILINEAR :
				{
					mFiltering = Ogre :: TFO_ANISOTROPIC;
					mAniso = 8;
					break;
				}
				case Ogre :: TFO_ANISOTROPIC :
				{
					mFiltering = Ogre :: TFO_BILINEAR;
					mAniso = 1;
					break;
				}
				default:
				{
					break;
				}
			}
			Ogre :: MaterialManager :: getSingleton ().setDefaultTextureFiltering (mFiltering);
			Ogre :: MaterialManager :: getSingleton().setDefaultAnisotropy (mAniso);

			showDebugOverlay (mStatsOn);

			mTimeUntilNextToggle = 1;
		}

		if (input_device -> isKeyDown (Ogre :: KC_SYSRQ))
		{
			char tmp [20];
			sprintf (tmp, "screenshot_%d.png", ++ mNumScreenShots);
			window -> writeContentsToFile (tmp);
			mTimeUntilNextToggle = 0.5;
			window -> setDebugText (string ("Wrote ") + tmp);
		}
		
		if (input_device -> isKeyDown (Ogre :: KC_R))
		{
			scene_detail_index = (scene_detail_index + 1) % 3;
			switch (scene_detail_index)
			{
				case 0 :
				{
					camera -> setPolygonMode (Ogre :: PM_SOLID);
					break;
				}
				case 1 :
				{
					camera -> setPolygonMode (Ogre :: PM_WIREFRAME);
					break;
				}
				case 2 :
				{
					camera -> setPolygonMode (Ogre :: PM_POINTS);
					break;
				}
			}
			mTimeUntilNextToggle = 0.5;
		}

		if (input_device -> isKeyDown (Ogre :: KC_P))
		{
			displayCameraDetails = ! displayCameraDetails;
			mTimeUntilNextToggle = 0.5;
			if (!displayCameraDetails)
			{
				window -> setDebugText ("");
			}
		}
	}
	
	if (displayCameraDetails)
	{
		// Print camera details
		window -> setDebugText("P: " + Ogre :: StringConverter :: toString (camera -> getDerivedPosition ()) + " " + "O: " + Ogre :: StringConverter :: toString (camera -> getDerivedOrientation ()));
	}

	// Return true to continue rendering
	return true;
}

bool Sl_Frame_Listener ::
	processUnbufferedMouseInput (const Ogre :: FrameEvent & evt)
{
	assert (is_initialized ());
	
	// Rotation factors, may not be used if the second mouse button is pressed.

	// If the second mouse button is pressed, then the mouse movement results in
	//   sliding the camera, otherwise we rotate.
	if(input_device -> getMouseButton (1))
	{
		mTranslateVector.x += input_device -> getMouseRelativeX() * 0.13;
		mTranslateVector.y -= input_device -> getMouseRelativeY() * 0.13;
	}
	else
	{
		mRotX = Ogre :: Degree (- input_device -> getMouseRelativeX() * 0.13);
		mRotY = Ogre :: Degree (- input_device -> getMouseRelativeY() * 0.13);
	}
	return true;
}

void Sl_Frame_Listener ::
	moveCamera ()
{
	assert (is_initialized ());
	
	// Make all the changes to the camera
	// Note that YAW direction is around a fixed axis (freelook style) rather than a natural YAW (e.g. airplane)
	camera -> yaw (mRotX);
	camera -> pitch (mRotY);
	camera -> moveRelative (mTranslateVector);
}

void Sl_Frame_Listener ::
	showDebugOverlay (bool show)
{
	assert (is_initialized ());
	
		if (debug_overlay != NULL)
		{
			if (show)
			{
				debug_overlay -> show ();
			}
			//else
		   // {
				//debug_overlay -> hide ();
			else
			{
				abort ();
			}
		}
	}

// Override frameStarted event to process that (don't care about frameEnded)
bool Sl_Frame_Listener ::
	frameStarted (const Ogre :: FrameEvent & evt)
{
	assert (is_initialized ());
	
	total_time += evt.timeSinceLastFrame;

	if (window -> isClosed ())
	{
		return false;
	}

	if (! mInputTypeSwitchingOn)
	{
		input_device -> capture ();
	}

	if (! mUseBufferedInputMouse || ! mUseBufferedInputKeys)
	{
		// one of the input modes is immediate, so setup what is needed for immediate mouse/key movement
		if (mTimeUntilNextToggle >= 0)
		{
			mTimeUntilNextToggle -= evt.timeSinceLastFrame;
		}

		// If this is the first frame, pick a speed
		if (evt.timeSinceLastFrame == 0)
		{
			mMoveScale = 1;
			mRotScale = 0.1;
		}
		// Otherwise scale movement units by time passed since last frame
		else
		{
			// Move about 100 units per second,
			mMoveScale = move_speed * evt.timeSinceLastFrame;
			// Take about 10 seconds for full rotation
			mRotScale = rotate_speed * evt.timeSinceLastFrame;
		}
		mRotX = 0;
		mRotY = 0;
		mTranslateVector = Ogre :: Vector3 :: ZERO;
	}

	if (mUseBufferedInputKeys)
	{
	// no need to do any processing here, it is handled by event processor and
	// you get the results as KeyEvents
	}
	else
	{
		if (processUnbufferedKeyInput (evt) == false)
		{
			return false;
		}
	}
	
	if (mUseBufferedInputMouse)
	{
		// no need to do any processing here, it is handled by event processor and
		// you get the results as MouseEvents
	}
	else
	{
		if (processUnbufferedMouseInput (evt) == false)
		{
			return false;
		}
	}

	if (! mUseBufferedInputMouse || ! mUseBufferedInputKeys)
	{
	// one of the input modes is immediate, so update the movement vector
		moveCamera();
	}

	return true;
}

bool Sl_Frame_Listener ::
	frameEnded (const Ogre :: FrameEvent & evt)
{
	assert (is_initialized ());
	
	updateStats ();
	
	return true;
}

void Sl_Frame_Listener ::
	switchMouseMode ()
{
	assert (is_initialized ());
	
	mUseBufferedInputMouse = ! mUseBufferedInputMouse;
	input_device -> setBufferedInput (mUseBufferedInputKeys, mUseBufferedInputMouse);
}
	
void Sl_Frame_Listener ::
	switchKeyMode ()
{
	assert (is_initialized ());
	
	mUseBufferedInputKeys = ! mUseBufferedInputKeys;
	input_device -> setBufferedInput (mUseBufferedInputKeys, mUseBufferedInputMouse);
}

void Sl_Frame_Listener ::
	keyClicked (Ogre :: KeyEvent * e)
{
	assert (is_initialized ());
	
	if (e -> getKeyChar() == 'm')
	{
		switchMouseMode();
	}
	else if (e -> getKeyChar() == 'k')
	{
		switchKeyMode();
	}
}

void Sl_Frame_Listener ::
	keyPressed (Ogre :: KeyEvent * e)
{
	assert (is_initialized ());
}

void Sl_Frame_Listener ::
	keyReleased (Ogre :: KeyEvent * e)
{
	assert (is_initialized ());
}
