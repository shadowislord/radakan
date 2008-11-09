package com.jmex.simplephysics;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import com.jme.bounding.BoundingVolume;

/**
 * <code>CollisionScene</code> stores the list of Static and Dynamic objects in the scene.<br>
 * It also move and update every dynamic collider.<br>
 * This class is the representation of the collision scene and don't need to be the rendered scene.
 * 
 * @author Lucas Goraieb
 */
public class CollisionScene {
	
	private Set<StaticCollider> staticList = new HashSet<StaticCollider>();

	private Set<DynamicCollider> dynamicList = new HashSet<DynamicCollider>();
	
	private List<CollisionResult> collisionResult = new ArrayList<CollisionResult>();
	
	protected CollisionSolver collisionSolver = new CollisionSolver();

	/**
	 * Attach a static collider (like maps, terrains, walls) to the scene.<br>
	 * This is normally used to attach Meshes to be divided in octrees.
	 * @param collider Static collider to attach
	 * @return The attached collider for chained methods
	 */
	public StaticCollider attachStatic(StaticCollider collider) {
		staticList.add(collider);
		return collider;
	}
	
	/**
	 * Attach a dynamic collider (like players, enemies, rockets) to the scene.
	 * @param collider Dynamic collider to attach
	 * @return The attached collider for chained methods
	 */
	public DynamicCollider attachDynamic(DynamicCollider collider) {
		dynamicList.add(collider);
		return collider;
	}
	
	/**
	 * Build the octrees for the static colliders.<br>
	 * Must be called after all static colliders were attached.
	 */
	public void build() {
		for (StaticCollider collider : staticList) {
			collider.build();
		}
	}
	
	/**
	 * Used internally by the CollisionSolver to fill the collsion result list.
	 * @return true if intersected
	 */
	protected boolean intersect(BoundingVolume bv) {
		collisionResult.clear();
		for (StaticCollider collider : staticList) {
			collider.intersect(bv, collisionResult);
		}
		return collisionResult.size() > 0;
	}

	protected List<CollisionResult> getCollisionResult() {
		return collisionResult;
	}

	protected Set<DynamicCollider> getDynamicColliders() {
		return dynamicList;
	}
	
	/**
	 * Updates the collision scene by moving every attached dynamic collider.
	 * @param timeFactor Normally the Time per frame to adapt to diferent framerates
	 */
	public void update(float timeFactor) {
		for (DynamicCollider collider : dynamicList) {
			collider.getPhysicState().move(collider, this, timeFactor);
		}
	}

}
