#include <Ogre.h>
#include <OIS.h>

#include "character.hpp"

Player_Character::Player_Character (String char_name, Ogre::Entity * new_ogre_entity, SceneManager * ogre_scene_mgr)
{
	// Setup member references
	name = char_name;
	entity = new_ogre_entity;
	scene_mgr = ogre_scene_mgr;

	// Setup node structure to handle a 3rd person camera
	main_node = scene_mgr -> getRootSceneNode () -> createChildSceneNode (name);
	sight_node = main_node -> createChildSceneNode (name + "_sight", Vector3 (0, 0, 200));
	camera_node = main_node -> createChildSceneNode (name + "_camera", Vector3 (0, 100, -300));

	// Add the entity to the main node
	main_node -> attachObject (entity);
}

Player_Character::~Player_Character ()
{
	main_node -> detachAllObjects ();
	main_node -> removeAndDestroyAllChildren ();
	scene_mgr -> destroySceneNode (name);
	scene_mgr -> destroyEntity (entity);
}

void Player_Character::update (Real elapsed_time, OIS::Keyboard * input)
{
	// Handle movement
	if (input -> isKeyDown (OIS::KC_W)) {
		main_node -> translate (main_node->getOrientation () * Vector3 (0, 0, 100 * elapsed_time));
	}
	if (input -> isKeyDown (OIS::KC_S)) {
		main_node -> translate (main_node->getOrientation () * Vector3 (0, 0, -50 * elapsed_time));
	}
	if (input -> isKeyDown (OIS::KC_A)) {
		main_node -> yaw (Radian (2 * elapsed_time));
	}
	if (input -> isKeyDown (OIS::KC_D)) {
		main_node -> yaw (Radian (-2 * elapsed_time));
	}
}