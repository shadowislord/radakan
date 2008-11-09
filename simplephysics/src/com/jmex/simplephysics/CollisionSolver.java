package com.jmex.simplephysics;

import java.util.HashMap;
import java.util.Map;
import java.util.Set;

import com.jme.math.Vector3f;
import com.jme.scene.TriMesh;

/**
 * <code>CollisionSolver</code> does the actual collsion and slide methods.<br>
 * Implementation of algorithm found at: http://www.peroxide.dk/papers/collision/collision.pdf, thanks to Ruab
 * @author Lucas Goraieb
 */
public class CollisionSolver {
	
	private IntersectionResult result = new IntersectionResult();
	
	private Map<Collider, Collision> collisionMap = new HashMap<Collider, Collision>();
	
	private float veryCloseDistance = 0.001f;

	private int recursionDepth = 5;

	// collision internal variables
	private Vector3f colliderSpaceCenter = new Vector3f();

	private Vector3f colliderSpaceVelocity = new Vector3f();

	private Vector3f colliderSpaceVelocityNormalized = new Vector3f();

	private Vector3f colliderSpaceVelocityBuffer = new Vector3f();

	private Vector3f colliderOriginalVelocity;
	
	public void setVeryCloseDistance(float dist) {
		veryCloseDistance = dist;
	}
	
	public void setRecursionDepth(int depth) {
		recursionDepth = depth;
	}
	
	/**
	 * Move, collide and slide the collider within the scene
	 * @param collider DynamicCollider
	 * @param scene CollisionScene
	 * @param timeFactor
	 * @param slide 
	 */
	public Vector3f move(DynamicCollider collider, CollisionScene scene, Vector3f velocity, float timeFactor) {
		collisionMap.clear();
		collider.scaleToSpace(colliderSpaceCenter.set(collider.location));
		colliderOriginalVelocity = velocity;
		collider.scaleToSpace(colliderSpaceVelocity.set(colliderOriginalVelocity)).multLocal(timeFactor);
		collideAndSlideEllipse(1, collider, scene, true);
		//collider.updateLocation(colliderSpaceCenter);
		for (Collision c : collisionMap.values()) {
			c.collider.onCollisionFrom(collider, c.collisionPoint, c.collisionNormal, colliderOriginalVelocity);
			collider.onCollidedWith(c.collider, c.collisionPoint, c.collisionNormal, colliderOriginalVelocity);
		}
		return colliderSpaceCenter;
	}

	private final static Vector3f v = new Vector3f(), newSourcePoint = new Vector3f(),
			destinationPoint = new Vector3f(), slidePlaneNormal = new Vector3f();

	private final static Vector3f displacementVector = new Vector3f(),
			newDestinationPoint = new Vector3f(), newVelocityVector = new Vector3f();

	private final static Plane slidingPlane = new Plane();

	private void collideAndSlideEllipse(int depth, DynamicCollider collider, CollisionScene scene, boolean slide) {
		if (depth > recursionDepth || colliderSpaceVelocity.length() < veryCloseDistance * 2)
			return;

		colliderSpaceVelocityNormalized.set(colliderSpaceVelocity);
		colliderSpaceVelocityNormalized.normalizeLocal();

		// extend the size of the ellipse space vector by just a bit, to give us
		// some
		// collision buffer
		colliderSpaceVelocityBuffer.set(colliderSpaceVelocityNormalized);
		colliderSpaceVelocityBuffer.multLocal(veryCloseDistance);
		colliderSpaceVelocity.addLocal(colliderSpaceVelocityBuffer);

		checkSpaceCollision(collider, scene);

		if (!result.hasCollision) {
			colliderSpaceCenter.addLocal(colliderSpaceVelocity).subtractLocal(
					colliderSpaceVelocityBuffer);
			return;
		}

		destinationPoint.set(colliderSpaceCenter).addLocal(colliderSpaceVelocity);
		v.set(colliderSpaceVelocity);

		if (result.nearestCollisionDistance == 0.0f)
			result.nearestCollisionDistance = -veryCloseDistance;

		float factorx = result.nearestCollisionDistance - veryCloseDistance;
		v.multLocal(factorx);
		newSourcePoint.set(colliderSpaceCenter).addLocal(v);

		/*Shold return here if don't need to slide*/
		if (!slide) {
			colliderSpaceCenter.set(newSourcePoint);
			return;
		}
		
		slidePlaneNormal.set(newSourcePoint).subtractLocal(result.collisionPoint).normalizeLocal();

		v.normalizeLocal().multLocal(veryCloseDistance);
		float factor = v.dot(slidePlaneNormal);
		if (factor != 0.0f)
			factor = veryCloseDistance / factor;
		displacementVector.set(v).multLocal(factor);

		newSourcePoint.addLocal(displacementVector);
		result.collisionPoint.addLocal(displacementVector);

		slidingPlane.setNormal(slidePlaneNormal);

		slidingPlane.setConstant(slidePlaneNormal.dot(result.collisionPoint));

		float dist = slidingPlane.pseudoDistance(destinationPoint);
		newDestinationPoint.scaleAdd(-dist, slidePlaneNormal, destinationPoint);
		newVelocityVector.set(newDestinationPoint).subtractLocal(result.collisionPoint);
		if (newVelocityVector.length() <= veryCloseDistance) {
			colliderSpaceCenter.set(newSourcePoint);
			return;
		}
		colliderSpaceCenter.set(newSourcePoint);
		colliderSpaceVelocity.set(newVelocityVector);
		collideAndSlideEllipse(depth + 1, collider, scene, slide);
	}

	private void resetResults() {
		result.nearestCollider = null;
		result.nearestCollisionDistance = 100000.0f;
		result.hasCollision = false;
	}
	
	private final static Vector3f cscA = new Vector3f(), cscB = new Vector3f(),
			cscC = new Vector3f(), cscD = new Vector3f(), cscE = new Vector3f();
	
	private final static Vector3f[] cscVertices = { new Vector3f(), new Vector3f(), new Vector3f()};
	
	private void checkSpaceCollision(DynamicCollider collider, CollisionScene scene) {
		resetResults();
		
		// Check for bounds and collect the triangles
		TriMesh mesh;
		collider.updateWorldBound(colliderOriginalVelocity);
		//collider.updateWorldBound(colliderSpaceVelocity);
		if (scene.intersect(collider.worldSpaceBounds)) {
			for (CollisionResult octreeResult : scene.getCollisionResult()) {
				mesh = octreeResult.getCollider().getMesh();
				Set<Integer> triList = octreeResult.getTriangles();
				for (Integer i : triList) {
					octreeResult.getMesh().getTriangle(i, cscVertices);
					mesh.localToWorld(cscVertices[0], cscA);
					mesh.localToWorld(cscVertices[1], cscB);
					mesh.localToWorld(cscVertices[2], cscC);
					
					// we now have a triangle in tempVa, tempVb, tempVc which is in
					// world coordinates

					// move the triangle into collision space
					collider.scaleToSpace(cscA);
					collider.scaleToSpace(cscB);
					collider.scaleToSpace(cscC);
					
					float intersectionDistance = collider.intersectTriangle(colliderSpaceCenter,
							colliderSpaceVelocity, cscA, cscB, cscC, cscD, cscE);

					// if we found a collision better than our current best, store
					// it instead
					if (intersectionDistance < result.nearestCollisionDistance) {
						result.nearestCollider = octreeResult.getCollider();
						result.nearestCollisionDistance = intersectionDistance;
						result.collisionPoint.set(cscD);
						result.hasCollision = true;
						
						Collision collision = collisionMap.get(octreeResult.getCollider());
						if (collision == null) {
							collision = new Collision();
							collision.collider = octreeResult.getCollider();
							collision.nearestCollisionDistance = intersectionDistance;
							collider.scaleFromSpace(collision.collisionPoint.set(cscD));
							collision.collisionNormal.set(cscE);
							collisionMap.put(collision.collider, collision);
						} else if (intersectionDistance < collision.nearestCollisionDistance) {
							collision.nearestCollisionDistance = intersectionDistance;
							collider.scaleFromSpace(collision.collisionPoint.set(cscD));
							collision.collisionNormal.set(cscE);
						}
					}
				}
			}
		}
		
		float intersectionDistance = Float.NaN;
		for (Collider c : scene.getDynamicColliders()) {
			if (c == collider) {
				continue;
			}
			if (c instanceof EllipseCollider) {
				EllipseCollider ec = (EllipseCollider) c;
				collider.scaleToSpace(cscA.set(ec.location));
				collider.scaleToSpace(cscB.set(ec.getDimension()));
				intersectionDistance = collider.intersectEllipse(colliderSpaceCenter,
						colliderSpaceVelocity, cscA, cscB, cscD, cscE);
				if (intersectionDistance > 0) { 
					continue; 
				} else {
					if (intersectionDistance == 0) {
						intersectionDistance = veryCloseDistance;
					}
					
					Collision collision = collisionMap.get(c);
					if (collision == null) {
						collision = new Collision();
						collision.collider = c;
						collision.nearestCollisionDistance = intersectionDistance;
						collider.scaleFromSpace(collision.collisionPoint.set(cscD));
						collision.collisionNormal.set(cscE);
						collisionMap.put(collision.collider, collision);
					} else if (intersectionDistance < collision.nearestCollisionDistance) {
						collision.nearestCollisionDistance = intersectionDistance;
						collider.scaleFromSpace(collision.collisionPoint.set(cscD));
						collision.collisionNormal.set(cscE);
					}
				}
			}
			if (intersectionDistance < result.nearestCollisionDistance) {
				result.nearestCollider = c;
				result.nearestCollisionDistance = intersectionDistance;
				result.collisionPoint.set(cscD);
				result.hasCollision = true;
			}
		}
	}

	private class IntersectionResult {
		
		Collider nearestCollider = null;
		
		float nearestCollisionDistance = 100000.0f;
	
		Vector3f collisionPoint = new Vector3f();
	
		boolean hasCollision = false;
		
	}
	
	private class Collision {
		
		Collider collider = null;
		
		float nearestCollisionDistance = 100000.0f;
	
		Vector3f collisionNormal = new Vector3f();

		Vector3f collisionPoint = new Vector3f();
	
	}
}
