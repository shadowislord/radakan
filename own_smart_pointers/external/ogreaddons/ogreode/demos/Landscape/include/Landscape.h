#include "ExampleApplication.h"
#include "OgreOde_Core.h"
#include "OgreOde_Prefab.h"
#include "OgreOde_Loader.h"

class LandscapeFrameListener : public ExampleFrameListener, 
    public OgreOde::TerrainGeometryHeightListener, 
    public OgreOde::CollisionListener
{
public:
	LandscapeFrameListener(Ogre::RenderWindow* win, Ogre::Camera* cam, Ogre::Root* root);
	~LandscapeFrameListener();
    bool frameStarted(const Ogre::FrameEvent& evt);
	void changeCar();

private:
	void updateInfo();
	OgreOde::StepHandler *_stepper;
	OgreOde_Prefab::Vehicle *_vehicle;
	Ogre::RaySceneQuery *_ray_query;
    Ogre::Ray ray;
	char _drive;
    OgreOde_Loader::DotLoader *dotOgreOdeLoader;

    virtual Ogre::Real heightAt(const Ogre::Vector3& position);
    virtual bool collision(OgreOde::Contact* contact);

protected:
    Ogre::SceneManager *mSceneMgr;
    OgreOde::World *_world;
    OgreOde::TerrainGeometry *_terrain;
    Ogre::Real _time_step;
    size_t _average_num_query;
};



class LandscapeApplication : public ExampleApplication
{
public:
    LandscapeApplication();
    ~LandscapeApplication();

protected:
	virtual bool setup(void);
    virtual void chooseSceneManager(void);
	virtual void setupResources(void);
    virtual void createCamera(void);
    void createScene(void);
	void createFrameListener(void);


};

