package com.jmex.simplephysics;

import java.util.HashSet;
import java.util.Set;


import com.jme.math.Vector3f;
import com.jme.scene.Spatial;

/**
 * <code>Collider</code> provides an abstract base class for colliders.
 * 
 * @author Lucas Goraieb
 */
public abstract class Collider {

	Set<CollisionListener> listeners = new HashSet<CollisionListener>();

	Vector3f location = new Vector3f();

	Spatial spatial;
	
	/**
	 * Adds a collision listener to this collider.
	 * @param listener CollisionListener
	 */
	public void addListener(CollisionListener listener) {
		listeners.add(listener);
	}

	protected void onCollisionFrom(Collider collider, Vector3f position, Vector3f normal, Vector3f velocity) {
		for (CollisionListener cl : listeners) {
			cl.collided(collider, this, position, normal, velocity);
		}
	}

	public abstract PhysicMaterial getPhysicMaterial();

	public Spatial getSpatial() {
		return spatial;
	}
	
	protected abstract Vector3f scaleToSpace(Vector3f vector);

	protected abstract Vector3f scaleFromSpace(Vector3f vector);

	public Vector3f getLocation() {
		return location;
	}

	public void setLocation(Vector3f location) {
		this.location.set(location);
	}
	
	public void setLocation(float x, float y, float z) {
		this.location.set(x, y, z);
	}	
}
