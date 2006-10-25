#ifndef CHARACTER_HPP
#define CHARACTER_HPP

using namespace Ogre;
using namespace std;

// Generic Character class
class Character
{

public:

	// Updates the character (movement, animation, etc...)
	virtual void update (Ogre::Real elapsed_time, OIS::Keyboard * input) = 0;

	SceneNode * get_sight_node () { return sight_node; }
	SceneNode * get_camera_node () { return camera_node; }
	Vector3 get_world_position () { return main_node -> getWorldPosition (); }

protected:

	Entity * entity; // Character entity

	SceneNode * main_node; // Main character node
	SceneNode * sight_node; // "Sight" node - The character is supposed to be looking here
	SceneNode * camera_node; // Node for the chase camera

	SceneManager * scene_mgr;

	// dead or alive?
	bool dead;

	// exp = experience
	int agility_exp;
	int beauty_exp;
	int constitution_exp;
	int equolence_exp;
	int intelligence_exp;
	int quickness_exp;
	int strength_exp;
	int willpower_exp;

};

// Specialization of the Character class
class Player_Character : public Character
{

protected:

	String name;

public:

	Player_Character (String char_name, Ogre::Entity * new_ogre_entity, SceneManager * ogre_scene_mgr);
	~Player_Character ();

	void update (Real elapsed_time, OIS::Keyboard * input);

	// Change visibility
	void set_visible (bool visible) { main_node -> setVisible (visible); }

};

#endif