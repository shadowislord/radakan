/*
SimpleScenes_BoxStack.h
----------------------
A reimplementation of the ODE box stacking (actually loads of different
objects) demo using Ogre and the OgreOde wrapper.
*/
#ifndef _SIMPLESCENES_BOXSTACK_H_
#define _SIMPLESCENES_BOXSTACK_H_

/*
The box stacking test extends the base test class
*/
class SimpleScenes_BoxStack:public SimpleScenes
{
public:
	// Standard constructor/destructor
    SimpleScenes_BoxStack(OgreOde::World *world) : SimpleScenes(world) {}
	virtual ~SimpleScenes_BoxStack(){}

	// Return our name for the test application to display
	virtual const String& getName()
	{
		static String name = "Test Box Stack";
		return name;
	}

	// Return a description of the keys that the user can use in this test
	virtual const String& getKeys()
	{
		static String keys = "Z - Sphere, X - Box, C - Capsule, G - Ragdoll, T - Trimesh";
		return keys;
	}

#if (OGRE_VERSION <  ((1 << 16) | (3 << 8) | 0))
    // Handle the user's key presses	
    virtual void frameEnded(Real time,InputReader* input)
    {
        // Do default key handling
        SimpleScenes::frameEnded(time,input);
#else

	virtual void frameEnded(Real time, OIS::Keyboard* input, OIS::Mouse* mouse)
	{
		// Do default processing
		SimpleScenes::frameEnded(time, input, mouse);
#endif
		OgreOde::Body* body = 0;
	
		// Create random objects, that method will stop them being created too often
		if (input->isKeyDown(KC_Z)) 
            body = createRandomObject(OgreOde::Geometry::Class_Sphere);
		else if (input->isKeyDown(KC_X)) 
            body = createRandomObject(OgreOde::Geometry::Class_Box);
		else if (input->isKeyDown(KC_C)) 
            body = createRandomObject(OgreOde::Geometry::Class_Capsule);
        else if (input->isKeyDown(KC_T)) 
            body = createRandomObject(OgreOde::Geometry::Class_TriangleMesh);
        else if (input->isKeyDown(KC_G)) 
            createRagDoll();
        
	}
};

#endif
