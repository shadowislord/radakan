#include "framelistener.hpp"

FrameLstr::
	FrameLstr (SceneManager * new_scene_mgr, RenderWindow * new_window,
		Camera * new_camera, bool useBufferedInputKeys,
		bool useBufferedInputMouse):
//	mSceneMgr (new_scene_mgr),
	Object ("FrameListener")
{

	scene_mgr = new_scene_mgr;
	total_time = 0;
	camera = new_camera;
	window = new_window;
	mStatsOn = true;
	mNumScreenShots = 0;
	mTimeUntilNextToggle = 0;
	scene_detail_index = 0;
	mMoveScale = 0.0f;
	mRotScale = 0.0f;
	mTranslateVector = Vector3::ZERO;
	mAniso = 1;
	mFiltering = TFO_BILINEAR;

	//showDebugOverlay (false);

	debug_overlay = OverlayManager::getSingleton().getByName("Core/DebugOverlay");
	mUseBufferedInputKeys = useBufferedInputKeys;
	mUseBufferedInputMouse = useBufferedInputMouse;
	mInputTypeSwitchingOn = mUseBufferedInputKeys || mUseBufferedInputMouse;
	rotate_speed = 36;
	move_speed = 100;

	if (mInputTypeSwitchingOn)
	{
		event_processor = new EventProcessor();
		event_processor->initialise(window);
		event_processor->startProcessingEvents();
		event_processor->addKeyListener(this);
		input_device = event_processor->getInputReader();
	}
	else
	{
		input_device = PlatformManager::getSingleton().createInputReader();
		input_device->initialise(window,true, true);
	}
}

//virtual
	FrameLstr::
	~FrameLstr ()
{
	if (mInputTypeSwitchingOn)
	{
		delete event_processor;
	}
	else
	{
		PlatformManager::getSingleton().destroyInputReader( input_device );
	}
}

void FrameLstr::updateStats(void)
{
	static String currFps = "Current FPS: ";
	static String avgFps = "Average FPS: ";
	static String bestFps = "Best FPS: ";
	static String worstFps = "Worst FPS: ";
	static String tris = "Triangle Count: ";

	// update stats when necessary
	try
	{
		OverlayElement* guiAvg = OverlayManager::getSingleton().getOverlayElement("Core/AverageFps");
		OverlayElement* guiCurr = OverlayManager::getSingleton().getOverlayElement("Core/CurrFps");
		OverlayElement* guiBest = OverlayManager::getSingleton().getOverlayElement("Core/BestFps");
		OverlayElement* guiWorst = OverlayManager::getSingleton().getOverlayElement("Core/WorstFps");

		const RenderTarget::FrameStats& stats = window->getStatistics();

//		print << "FPS: " << stats.avgFPS << endl;

		guiAvg->setCaption(avgFps + StringConverter::toString(stats.avgFPS));
		guiCurr->setCaption(currFps + StringConverter::toString(stats.lastFPS));
		guiBest->setCaption(bestFps + StringConverter::toString(stats.bestFPS)
			+" "+StringConverter::toString(stats.bestFrameTime)+" ms");
		guiWorst->setCaption(worstFps + StringConverter::toString(stats.worstFPS)
			+" "+StringConverter::toString(stats.worstFrameTime)+" ms");

		OverlayElement* guiTris = OverlayManager::getSingleton().getOverlayElement("Core/NumTris");
		guiTris->setCaption(tris + StringConverter::toString(stats.triangleCount));

		OverlayElement* guiDbg = OverlayManager::getSingleton().getOverlayElement("Core/DebugText");
            guiDbg->setCaption(window->getDebugText());
        }
        catch (...)
        {
            // ignore
        }
    }



//    virtual
bool FrameLstr::processUnbufferedKeyInput(const FrameEvent& evt)
    {
        if (input_device->isKeyDown(KC_A))
        {
            // Move camera left
            mTranslateVector.x = -mMoveScale;
        }

        if (input_device->isKeyDown(KC_D))
        {
            // Move camera RIGHT
            mTranslateVector.x = mMoveScale;
        }

        /* Move camera forward by keypress. */
        if (input_device->isKeyDown(KC_UP) || input_device->isKeyDown(KC_W) )
        {
            mTranslateVector.z = -mMoveScale;
        }

        /* Move camera backward by keypress. */
        if (input_device->isKeyDown(KC_DOWN) || input_device->isKeyDown(KC_S) )
        {
            mTranslateVector.z = mMoveScale;
        }

        if (input_device->isKeyDown(KC_PGUP))
        {
            // Move camera up
            mTranslateVector.y = mMoveScale;
        }

        if (input_device->isKeyDown(KC_PGDOWN))
        {
            // Move camera down
            mTranslateVector.y = -mMoveScale;
        }

        if (input_device->isKeyDown(KC_RIGHT))
        {
            camera->yaw(-mRotScale);
        }
		
        if (input_device->isKeyDown(KC_LEFT))
        {
            camera->yaw(mRotScale);
        }

        if( input_device->isKeyDown( KC_ESCAPE) )
        {
            return false;
        }

		// see if switching is on, and you want to toggle 
        if (mInputTypeSwitchingOn && input_device->isKeyDown(KC_M) && mTimeUntilNextToggle <= 0)
        {
			switchMouseMode();
            mTimeUntilNextToggle = 1;
        }

        if (mInputTypeSwitchingOn && input_device->isKeyDown(KC_K) && mTimeUntilNextToggle <= 0)
        {
			// must be going from immediate keyboard to buffered keyboard
			switchKeyMode();
            mTimeUntilNextToggle = 1;
        }
        if (input_device->isKeyDown(KC_F) && mTimeUntilNextToggle <= 0)
        {
            mStatsOn = !mStatsOn;
            showDebugOverlay(mStatsOn);

            mTimeUntilNextToggle = 1;
        }
        if (input_device->isKeyDown(KC_T) && mTimeUntilNextToggle <= 0)
        {
            switch(mFiltering)
            {
            case TFO_BILINEAR:
                mFiltering = TFO_TRILINEAR;
                mAniso = 1;
                break;
            case TFO_TRILINEAR:
                mFiltering = TFO_ANISOTROPIC;
                mAniso = 8;
                break;
            case TFO_ANISOTROPIC:
                mFiltering = TFO_BILINEAR;
                mAniso = 1;
                break;
            default:
                break;
            }
            MaterialManager::getSingleton().setDefaultTextureFiltering(mFiltering);
            MaterialManager::getSingleton().setDefaultAnisotropy(mAniso);


            showDebugOverlay(mStatsOn);

            mTimeUntilNextToggle = 1;
        }

        if (input_device->isKeyDown(KC_SYSRQ) && mTimeUntilNextToggle <= 0)
        {
			char tmp[20];
			sprintf(tmp, "screenshot_%d.png", ++mNumScreenShots);
            window->writeContentsToFile(tmp);
            mTimeUntilNextToggle = 0.5;
			window->setDebugText(String("Wrote ") + tmp);
        }
		
		if (input_device->isKeyDown(KC_R) && mTimeUntilNextToggle <=0)
		{
			scene_detail_index = (scene_detail_index+1)%3 ;
			switch(scene_detail_index) {
				case 0 : camera->setPolygonMode(PM_SOLID) ; break ;
				case 1 : camera->setPolygonMode(PM_WIREFRAME) ; break ;
				case 2 : camera->setPolygonMode(PM_POINTS) ; break ;
			}
			mTimeUntilNextToggle = 0.5;
		}

        static bool displayCameraDetails = false;
        if (input_device->isKeyDown(KC_P) && mTimeUntilNextToggle <= 0)
        {
            displayCameraDetails = !displayCameraDetails;
            mTimeUntilNextToggle = 0.5;
            if (!displayCameraDetails)
                window->setDebugText("");
        }
        if (displayCameraDetails)
        {
            // Print camera details
            window->setDebugText("P: " + StringConverter::toString(camera->getDerivedPosition()) + " " +
                "O: " + StringConverter::toString(camera->getDerivedOrientation()));
        }

        // Return true to continue rendering
        return true;
    }

    bool FrameLstr::processUnbufferedMouseInput(const FrameEvent& evt)
    {
        /* Rotation factors, may not be used if the second mouse button is pressed. */

        /* If the second mouse button is pressed, then the mouse movement results in 
           sliding the camera, otherwise we rotate. */
        if( input_device->getMouseButton( 1 ) )
        {
            mTranslateVector.x += input_device->getMouseRelativeX() * 0.13;
            mTranslateVector.y -= input_device->getMouseRelativeY() * 0.13;
        }
        else
        {
            mRotX = Degree(-input_device->getMouseRelativeX() * 0.13);
            mRotY = Degree(-input_device->getMouseRelativeY() * 0.13);
        }


		return true;
	}

void FrameLstr::moveCamera()
{
	// Make all the changes to the camera
	// Note that YAW direction is around a fixed axis (freelook style) rather than a natural YAW (e.g. airplane)
	camera->yaw(mRotX);
	camera->pitch(mRotY);
	camera->moveRelative (mTranslateVector);
	}

    void FrameLstr::showDebugOverlay (bool show)
    {
        if (debug_overlay != NULL)
        {
            if (show)
            {
                debug_overlay->show ();
            }
            //else
           // {
                //debug_overlay->hide ();
            else
			{
				abort ();
			}
        }
    }

// Override frameStarted event to process that (don't care about frameEnded)
bool FrameLstr::frameStarted (const FrameEvent& evt)
{
	total_time += evt.timeSinceLastFrame;

	if (window->isClosed ())
	{
		return false;
	}

	if (! mInputTypeSwitchingOn)
	{
		input_device->capture();
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
		mTranslateVector = Vector3::ZERO;
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

bool FrameLstr::frameEnded(const FrameEvent& evt)
{
	updateStats();
	return true;
}

void FrameLstr::switchMouseMode ()
{
	mUseBufferedInputMouse = ! mUseBufferedInputMouse;
	input_device->setBufferedInput(mUseBufferedInputKeys, mUseBufferedInputMouse);
}
	
void FrameLstr::switchKeyMode ()
{
	mUseBufferedInputKeys = ! mUseBufferedInputKeys;
	input_device->setBufferedInput(mUseBufferedInputKeys, mUseBufferedInputMouse);
}

void FrameLstr::keyClicked (KeyEvent * e)
{
	if (e->getKeyChar() == 'm')
	{
		switchMouseMode();
	}
	else if (e->getKeyChar() == 'k')
	{
		switchKeyMode();
	}
}

void FrameLstr::keyPressed (KeyEvent * e)
{
}

void FrameLstr::keyReleased (KeyEvent * e)
{
}

