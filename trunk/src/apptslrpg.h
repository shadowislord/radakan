/*
 * apptslrpg.h
 *
 * Declaration of AppTslrpg, the main application object.
 */

#ifndef __apptslrpg_h
#define __apptslrpg_h

#include "tslrpg.h"
#include <csutil/ref.h>
#include <csutil/csbaseeventh.h>
#include <cstool/csapplicationframework.h>
#include <iengine/engine.h>
#include <ivideo/graph3d.h>

class AppTslrpg :
  public csApplicationFramework, public csBaseEventHandler
{
private:
  /**
   * A reference to the 3D renderer plugin.
   */
  csRef<iGraphics3D> g3d;

  /**
   * A reference to the 3D engine plugin.
   */
  csRef<iEngine> engine;

  /**
   * Set up everything that needs to be rendered on screen.  This routine is
   * called from the event handler in response to a cscmdProcess broadcast
   * message.
   */
  virtual void ProcessFrame();

  /**
   * Finally, render the screen.  This routine is called from the event handler
   * in response to a cscmdFinalProcess broadcast message.
   */
  virtual void FinishFrame();

  /**
   * Handle keyboard events, such as key presses and releases.  This routine is
   * called from the event handler in response to a csevKeyboard event.
   */
  virtual bool OnKeyboard(iEvent&);

public:
  /**
   * Constructor.
   */
  AppTslrpg();

  /**
   * Destructor.
   */
  virtual ~AppTslrpg();

  /**
   * Final cleanup.
   */
  virtual void OnExit();

  /**
   * Main initialization routine.  This routine should set up basic facilities
   * (such as loading startup-time plugins, etc.).  In case of failure this
   * routine will return false.  You can assume that the error message has been
   * reported to the user.
   */
  virtual bool OnInitialize(int argc, char* argv[]);

  /**
   * Run the application.  Performs additional initialization (if needed), and
   * then fires up the main run/event loop.  The loop will fire events which
   * actually causes Crystal Space to "run".  Only when the program exits does
   * this function return.
   */
  virtual bool Application();
  
  // Declare the name of this event handler.
  CS_EVENTHANDLER_NAMES("application.tslrpg")
      
  /* Declare that we're not terribly interested in having events
     delivered to us before or after other modules, plugins, etc. */
  CS_EVENTHANDLER_NIL_CONSTRAINTS
};

#endif // __apptslrpg_h
