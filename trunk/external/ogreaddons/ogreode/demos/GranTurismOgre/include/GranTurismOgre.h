#include "ExampleApplication.h"
#include "OgreOde_Core.h"
#include "OgreOde_Prefab.h"
#include "OgreOde_Loader.h"

class GranTurismOgreFrameListener : public ExampleFrameListener
{
public:
    GranTurismOgreFrameListener(RenderWindow* win, Camera* cam,Real time_step,Root* root,OgreOde::World *world);
	~GranTurismOgreFrameListener();
	bool frameStarted(const FrameEvent& evt);
	void changeCar();

private:
	void updateInfo();
    OgreOde::StepHandler *_stepper;
    OgreOde::World *_world;
	OgreOde_Prefab::Vehicle *_vehicle;
	char _drive;
	OgreOde_Loader::DotLoader *dotOgreOdeLoader;
};



class GranTurismOgreApplication : public ExampleApplication,public OgreOde::CollisionListener
{
public:
    GranTurismOgreApplication();
    ~GranTurismOgreApplication();

protected:

    virtual void chooseSceneManager(void);
	virtual void setupResources(void);
    virtual void createCamera(void);
    void createScene(void);
	void createFrameListener(void);

	virtual bool collision(OgreOde::Contact* contact);

	protected:
		OgreOde::World *_world;
		OgreOde_Prefab::Vehicle *_vehicle;
		OgreOde::TriangleMeshGeometry *_track;

		Real _time_step;
};

