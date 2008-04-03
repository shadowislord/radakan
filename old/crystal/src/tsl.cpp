//	Definition of Tsl, the main application object.

#include "tsl.hpp"
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

Tsl ::
	Tsl () :
	Object ("The Scattered Lands")
{
	SetApplicationName (const_cast <char *> (this->c_str ()));
}

Tsl::
	~Tsl ()
{
}

void Tsl ::
	ProcessFrame ()
{
//	First get elapsed time from the virtual clock.
	csTicks elapsed_time = vc->GetElapsedTicks ();
//	Now rotate the camera according to keyboard state
	float speed = (elapsed_time / 1000.0) * (0.06 * 20);

	iCamera * cam = view->GetCamera();

	if (kbd->GetKeyState (CSKEY_SHIFT))
	{
//	If the user is holding down shift, the arrow keys will cause the camera to
//	strafe up, down, left or right from it's current position.
		if (kbd->GetKeyState (CSKEY_RIGHT))
		{
			cam->Move (CS_VEC_RIGHT * 4 * speed);
		}
		if (kbd->GetKeyState (CSKEY_LEFT))
		{
			cam->Move (CS_VEC_LEFT * 4 * speed);
		}
		if (kbd->GetKeyState (CSKEY_UP))
		{
			cam->Move (CS_VEC_UP * 4 * speed);
		}
		if (kbd->GetKeyState (CSKEY_DOWN))
		{
			cam->Move (CS_VEC_DOWN * 4 * speed);
		}
	}
	else
	{
//	left and right cause the camera to rotate on the global Y axis
//	page up and page down cause the camera to rotate on the _camera's_ X axis
//	up and down arrows cause the camera to go forwards and backwards
		if (kbd->GetKeyState (CSKEY_RIGHT))
		{
			rot_y += speed;
		}
		if (kbd->GetKeyState (CSKEY_LEFT))
		{
			rot_y -= speed;
		}
		if (kbd->GetKeyState (CSKEY_PGUP))
		{
			rot_x += speed;
		}
		if (kbd->GetKeyState (CSKEY_PGDN))
		{
			rot_x -= speed;
		}
		if (kbd->GetKeyState (CSKEY_UP))
		{
			cam->Move (CS_VEC_FORWARD * 4 * speed);
		}
		if (kbd->GetKeyState (CSKEY_DOWN))
		{
			cam->Move (CS_VEC_BACKWARD * 4 * speed);
		}
	}

//	We now assign a new rotation transformation to the camera. You can think of
//	the rotation this way: starting from the zero position, you first rotate
//	rot_y radians on your Y axis to get the first rotation. From there you
//	rotate rot_x radians on the your X axis to get the final rotation.
//	We multiply the individual rotations on each axis together to get a single
//	rotation matrix. The rotations are applied in right to left order.
	csMatrix3 rot = csXRotMatrix3 (rot_x) * csYRotMatrix3 (rot_y);
	csOrthoTransform orthtransf (rot, cam->GetTransform().GetOrigin ());
	cam->SetTransform (orthtransf);

//	Tell 3D driver we're going to display 3D things.
	if (! g3d->BeginDraw (engine->GetBeginDrawFlags() | CSDRAW_3DGRAPHICS))
	{
		return;
	}

//	Tell the camera to render into the frame buffer.
	view->Draw ();
}

void Tsl ::
	FinishFrame ()
{
	g3d->FinishDraw ();
	g3d->Print (0);
}

bool Tsl ::
	OnKeyboard (iEvent & ev)
{
//	We got a keyboard event.
	if (csKeyEventHelper::GetEventType (&ev) == csKeyEventTypeDown)
	{
//	The user pressed a key (as opposed to releasing it).
		utf32_char code = csKeyEventHelper::GetCookedCode(&ev);
		if (code == CSKEY_ESC)
		{
//	The user pressed escape, so terminate the application.	The proper way to
//	terminate a Crystal Space application is by broadcasting a csevQuit event.
//	That will cause the main run loop to stop.	To do so we retrieve the event
//	queue from the object registry and then post the event.
			csRef<iEventQueue> q = csQueryRegistry <iEventQueue>
														(GetObjectRegistry ());
			if (q.IsValid())
			{
				q->GetEventOutlet ()->Broadcast (csevQuit
														(GetObjectRegistry ()));
			}
		}
	}
	return false;
}

bool Tsl ::
	OnInitialize (int argc, char * argv [])
{
	csRef <iObjectRegistry> r = GetObjectRegistry ();

//	Load application-specific configuration file.
	if (! csInitializer::SetupConfigManager(r, NULL, GetApplicationName()))
	{
		return ReportError ("Failed to initialize configuration manager!");
	}

#ifdef USE_CEL
	celInitializer :: SetupCelPluginDirs (r);
#endif

//	RequestPlugins() will load all plugins we specify. In addition it will also
//	check if there are plugins that need to be loaded from the configuration
//	system (both the application configuration and CS or global configurations).
//	It also supports specifying plugins on the command line via the
//	--plugin=<plugin name> option.
	if (! csInitializer :: RequestPlugins(
		r,
		CS_REQUEST_VFS,
		CS_REQUEST_OPENGL3D,
		CS_REQUEST_ENGINE,
		CS_REQUEST_FONTSERVER,
		CS_REQUEST_IMAGELOADER,
		CS_REQUEST_LEVELLOADER,
		CS_REQUEST_REPORTER,
		CS_REQUEST_REPORTERLISTENER,
		CS_REQUEST_END
	))
	{
		return ReportError("Failed to initialize plugins!");
	}
	
//	"Warm up" the event handler so it can interact with the world
	csBaseEventHandler :: Initialize (GetObjectRegistry ());
 
//	Set up an event handler for the application. Crystal Space is fully event-
//	driven. Everything (except for this initialization) happens in response to
//	an event.
	if (! RegisterQueue (r, csevAllEvents (GetObjectRegistry ())))
	{
		return ReportError ("Failed to set up event handler!");
	}

	return true;
}

void Tsl ::
	OnExit ()
{
}

bool Tsl ::
	Application ()
{
	csRef <iObjectRegistry> r = GetObjectRegistry ();

//	Open the main system. This will open all the previously loaded plugins (i.e.
//	all windows will be opened).
	if (!OpenApplication (r))
	{
		return ReportError ("Error opening system!");
	}

//	Now get the pointer to various modules we need. We fetch them from the
//	object registry. The RequestPlugins() call we did earlier registered all
//	loaded plugins with the object registry. It is also possible to load plugins
//	manually on-demand.
	g3d = csQueryRegistry <iGraphics3D> (r);
	if (! g3d)
	{
		return ReportError ("Failed to locate 3D renderer!");
	}
	engine = csQueryRegistry <iEngine> (r);
	if (! engine)
	{
		return ReportError ("Failed to locate 3D engine!");
	}
	vc = csQueryRegistry <iVirtualClock> (r);
	if (! vc)
	{
		return ReportError ("Failed to locate Virtual Clock!");
	}
	kbd = csQueryRegistry <iKeyboardDriver> (r);
	if (! kbd)
	{
		return ReportError ("Failed to locate Keyboard Driver!");
	}
	loader = csQueryRegistry <iLoader> (r);
	if (! loader)
	{
		return ReportError ("Failed to locate Loader!");
	}

//	We need a View to the virtual world.
	view.AttachNew (new csView (engine, g3d));
	iGraphics2D * g2d = g3d->GetDriver2D ();
//	We use the full window to draw the world.
	view->SetRectangle (0, 0, g2d->GetWidth (), g2d->GetHeight ());

//	First disable the lighting cache. Our app is simple enough
//	not to need this.
	engine->SetLightingCacheMode (0);
	
//	These are used store the current orientation of the camera.
	rot_x = 0;
	rot_y = 0;
	create_room ();

	view->GetCamera ()->SetSector (room);
	view->GetCamera ()->GetTransform ().SetOrigin (csVector3 (0, 5, -3));

//	Start the default run/event loop. This will return only when some code, such
//	as OnKeyboard(), has asked the run loop to terminate.
	Run ();

	return true;
}

void Tsl ::
	create_room ()
{
//	Load the texture from the standard library.  This is located in
//	CS/data/standard.zip and mounted as /lib/std using the Virtual File System
//	(VFS) plugin.
	if (! loader->LoadTexture ("stone", "/lib/std/stone4.gif"))
	{
		ReportError ("Error loading 'stone4' texture!");
	}

	iMaterialWrapper * tm = engine->GetMaterialList ()->FindByName ("stone");

	room = engine->CreateSector ("room");
	csRef <iMeshWrapper> walls (engine->CreateSectorWallsMesh (room, "walls"));
	iMeshObject* walls_object = walls->GetMeshObject ();
	iMeshObjectFactory* walls_factory = walls_object->GetFactory ();
	csRef <iThingFactoryState> walls_state =
						scfQueryInterface <iThingFactoryState> (walls_factory);
	walls_state->AddInsideBox (csVector3 (-5, 0, -5), csVector3 (5, 20, 5));
	walls_state->SetPolygonMaterial (CS_POLYRANGE_LAST, tm);
	walls_state->SetPolygonTextureMapping (CS_POLYRANGE_LAST, 3);

	csRef <iLight> light;
	iLightList* light_list = room->GetLights ();

	light = engine->CreateLight (0, csVector3 (-3, 5, 0), 10,
															csColor (1, 0, 0));
	light_list->Add (light);

	light = engine->CreateLight (0, csVector3 (3, 5,  0), 10,
															csColor (0, 0, 1));
	light_list->Add (light);

	light = engine->CreateLight (0, csVector3 (0, 5, -3), 10,
															csColor (0, 1, 0));
	light_list->Add (light);

	engine->Prepare ();
}
