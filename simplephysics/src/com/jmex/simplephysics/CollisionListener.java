package com.jmex.simplephysics;

import com.jme.math.Vector3f;

/**
 * <code>CollisionListener</code> provides an interface for Collision Listeners.
 * 
 * @author Lucas Goraieb
 */
public interface CollisionListener {

	/**
	 * This method is called when a collision is detected.
	 * @param collider The moving collider
	 * @param collided What the collider has collided with
	 * @param position Where the collision ocurred
	 * @param normal The collided's surface normal
	 */
	public void collided(Collider collider, Collider collided, Vector3f position, Vector3f normal, Vector3f velocity);

}
