package com.jmex.simplephysics;

import com.jme.scene.TriMesh;
import java.util.Set;


/**
 * <code>CollisionResult</code> stores the result of a collision.
 * 
 * @author Lucas Goraieb
 */
public class CollisionResult {

	private Set<Integer> triangles;
	
	private TriMesh mesh;
	
	private StaticCollider collider;
	
	public CollisionResult(StaticCollider collider, TriMesh mesh, Set<Integer> triangles) {
		this.collider = collider;
		this.mesh = mesh;
		this.triangles = triangles;
	}

	public TriMesh getMesh() {
		return mesh;
	}

	public Set<Integer> getTriangles() {
		return triangles;
	}

	public StaticCollider getCollider() {
		return collider;
	}
	
}
