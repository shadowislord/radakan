/*
 * apptslrpg.cpp
 *
 * Definition of AppTslrpg, the main application object.
 */

#include "apptslrpg.h"
#include <cstool/initapp.h>
#include <csutil/csstring.h>
#include <csutil/event.h>
#include <csutil/sysfunc.h>
#include <csutil/syspath.h>
#include <iutil/event.h>
#include <iutil/eventq.h>
#include <iutil/vfs.h>
#ifdef USE_CEL
#include <celtool/initapp.h>
#endif

AppTslrpg::AppTslrpg() : csApplicationFramework()
{
  SetApplicationName("tslrpg");
}

AppTslrpg::~AppTslrpg()
{
}

void AppTslrpg::ProcessFrame()
{
  if (g3d->BeginDraw(engine->GetBeginDrawFlags() | CSDRAW_3DGRAPHICS))
  {
    // Draw frame.
  }
}

void AppTslrpg::FinishFrame()
{
  g3d->FinishDraw();
  g3d->Print(0);
}

bool AppTslrpg::OnKeyboard(iEvent& ev)
{
  // We got a keyboard event.
  if (csKeyEventHelper::GetEventType(&ev) == csKeyEventTypeDown)
  {
    // The user pressed a key (as opposed to releasing it).
    utf32_char code = csKeyEventHelper::GetCookedCode(&ev);
    if (code == CSKEY_ESC)
    {
      // The user pressed escape, so terminate the application.  The proper way
      // to terminate a Crystal Space application is by broadcasting a
      // csevQuit event.  That will cause the main run loop to stop.  To do
      // so we retrieve the event queue from the object registry and then post
      // the event.
      csRef<iEventQueue> q =
        csQueryRegistry<iEventQueue> (GetObjectRegistry());
      if (q.IsValid())
        q->GetEventOutlet()->Broadcast(csevQuit(GetObjectRegistry()));
    }
  }
  return false;
}

bool AppTslrpg::OnInitialize(int argc, char* argv[])
{
  iObjectRegistry* r = GetObjectRegistry();

  // Load application-specific configuration file.
  if (!csInitializer::SetupConfigManager(r,
      "/my/vfs/path/AppTslrpg.cfg", GetApplicationName()))
    return ReportError("Failed to initialize configuration manager!");

#ifdef USE_CEL
  celInitializer::SetupCelPluginDirs(r);
#endif

  // RequestPlugins() will load all plugins we specify.  In addition it will
  // also check if there are plugins that need to be loaded from the
  // configuration system (both the application configuration and CS or global
  // configurations).  It also supports specifying plugins on the command line
  // via the --plugin= option.
  if (!csInitializer::RequestPlugins(r,
	CS_REQUEST_VFS,
	CS_REQUEST_OPENGL3D,
	CS_REQUEST_ENGINE,
	CS_REQUEST_FONTSERVER,
	CS_REQUEST_IMAGELOADER,
	CS_REQUEST_LEVELLOADER,
	CS_REQUEST_REPORTER,
	CS_REQUEST_REPORTERLISTENER,
	CS_REQUEST_END))
    return ReportError("Failed to initialize plugins!");

  // "Warm up" the event handler so it can interact with the world
  csBaseEventHandler::Initialize(GetObjectRegistry());
 
  // Set up an event handler for the application.  Crystal Space is fully
  // event-driven.  Everything (except for this initialization) happens in
  // response to an event.
  if (!RegisterQueue (r, csevAllEvents(GetObjectRegistry())))
    return ReportError("Failed to set up event handler!");

  return true;
}

void AppTslrpg::OnExit()
{
}

bool AppTslrpg::Application()
{
  iObjectRegistry* r = GetObjectRegistry();

  // Open the main system. This will open all the previously loaded plugins
  // (i.e. all windows will be opened).
  if (!OpenApplication(r))
    return ReportError("Error opening system!");

  // Now get the pointer to various modules we need.  We fetch them from the
  // object registry.  The RequestPlugins() call we did earlier registered all
  // loaded plugins with the object registry.  It is also possible to load
  // plugins manually on-demand.
  g3d = csQueryRegistry<iGraphics3D> (r);
  if (!g3d)
    return ReportError("Failed to locate 3D renderer!");

  engine = csQueryRegistry<iEngine> (r);
  if (!engine)
    return ReportError("Failed to locate 3D engine!");

  // Start the default run/event loop.  This will return only when some code,
  // such as OnKeyboard(), has asked the run loop to terminate.
  Run();

  return true;
}
