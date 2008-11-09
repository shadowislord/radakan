package com.jmex.simplephysics;

import com.jme.math.FastMath;
import com.jme.math.Vector3f;
import com.jme.scene.Spatial;

/**
 * <code>EllipseCollider</code> provides an implementation of a DynamicCollider using an Ellipsoid shape.
 * 
 * @author Lucas Goraieb
 */
public class EllipseCollider extends DynamicCollider {

	private Vector3f dimension = new Vector3f();

	private Vector3f spaceScale = new Vector3f(1,1,1);
	
	/**
	 * EllipseCollider's constructor
	 * @param location The starting position
	 * @param dimension The dimension of the ellipsoid
	 * @param spatial The visual representation (can be null)
	 */
	public EllipseCollider(Vector3f location, Vector3f dimension, Spatial spatial) {
		this.location.set(location);
		this.dimension.set(dimension);
		this.spaceScale.set(1 / dimension.x, 1 / dimension.y, 1 / dimension.z);
		this.spatial = spatial;
	}


	@Override
	protected Vector3f scaleFromSpace(Vector3f vector) {
		return vector.multLocal(dimension);
	}

	@Override
	protected Vector3f scaleToSpace(Vector3f vector) {
		return vector.multLocal(spaceScale);
	}
	
	@Override
	protected void updateLocation(Vector3f newLocation) {
		scaleFromSpace(location.set(newLocation));
		updateSpatial();
	}
	
	private final static Vector3f iePosA = new Vector3f();

	private static float ieDist;
	
	private final static Vector3f ieNA = new Vector3f();
	
	private final static Vector3f ieRA = new Vector3f();
	
	private final static Vector3f ieNB = new Vector3f();
	
	private static boolean ieEmbedded;
	
	protected float intersectEllipse(Vector3f center, Vector3f velocity,
			Vector3f ellipseLocation, Vector3f ellipseDimension, 
			Vector3f intersectionPoint, Vector3f intersectionNormal) {

		ieDist = center.distance(ellipseLocation)-1;
		if (velocity.length() > ieDist) {
			ieNA.set(velocity).normalizeLocal().multLocal(ieDist);
			iePosA.set(center).addLocal(ieNA);
			ieEmbedded = true;
		} else {
			iePosA.set(center).addLocal(velocity);
			ieEmbedded = false;
		}
		
		ieNA.set(ellipseLocation).subtractLocal(iePosA).divideLocal(ellipseDimension).normalizeLocal();
		ieRA.set(iePosA).addLocal(ieNA);
		ieNB.set(ieRA).subtractLocal(ellipseLocation).divideLocal(ellipseDimension).normalizeLocal();
		intersectionPoint.set(ellipseLocation).addLocal(ieNB.mult(ellipseDimension));
		intersectionNormal.set(iePosA).subtractLocal(ellipseLocation).divideLocal(ellipseDimension).normalizeLocal();
		ieDist = iePosA.distance(intersectionPoint)-1;
		if (!ieEmbedded) {
			if (ieDist < 0) {
				ieDist = 0;
			}
		}
		return ieDist;
	}
	
	private final static Plane itP = new Plane();

	private final static Vector3f itVa = new Vector3f();

	private final static Vector3f itVb = new Vector3f();
	
	protected float intersectTriangle(Vector3f center, Vector3f velocity,
			Vector3f triangleOne, Vector3f triangleTwo, Vector3f triangleThree,
			Vector3f intersectionPoint, Vector3f intersectionNormal) {
		itP.setPlanePoints(triangleOne, triangleTwo, triangleThree);
		if (itP.normal.dot(velocity.normalize()) <= 0.0f) {
			float t0, t1;
			boolean embedded = false;

			float signedDistanceToPlane = center.dot(itP.normal) + itP.constant;
			float normalDotVelocity = itP.normal.dot(velocity);

			if (normalDotVelocity == 0.0f) {
				// we are travelling exactly parrallel to the plane

				if (FastMath.abs(signedDistanceToPlane) >= 1.0f) {
					// no collision possible
					return Float.NaN;
				} else {
					// we are embedded
					t0 = 0;
					t1 = 1;
					embedded = true;
				}
			} else {
				t0 = (-1 - signedDistanceToPlane) / normalDotVelocity;
				t1 = (1 - signedDistanceToPlane) / normalDotVelocity;

				if (t0 > t1) {
					float temp = t1;
					t1 = t0;
					t0 = temp;
				}

				if (t0 > 1.0f || t1 < 0.0f) {
					// collision is out of this velocity range
					return Float.NaN;
				}

				// clamp the interval to [0, 1]
				t0 = Math.max(t0, 0.0f);
				t1 = Math.min(t1, 1.0f);
				boolean foundCollision = false;
				float t = 1.0f;

				if (!embedded) {
					itVa.set(center);
					itVa.subtractLocal(itP.normal);
					itVa.scaleAdd(t0, velocity, itVa);

					// test to see if the collision is on a triangle interior
					if (CollisionMath.isPointInTriangle(itVa, triangleOne, triangleTwo, triangleThree)) {
						foundCollision = true;
						t = t0;
						intersectionPoint.set(itVa);
						intersectionNormal.set(itP.normal);
					}

					float velocitySquaredLength = velocity.lengthSquared();

					// triangleOne
					float newT = CollisionMath.checkUnitSphereVertexCollision(center, velocity,
							velocitySquaredLength, triangleOne, t, intersectionPoint);
					if (!Float.isNaN(newT)) {
						t = newT;
						foundCollision = true;
					}

					// triangleTwo
					newT = CollisionMath.checkUnitSphereVertexCollision(center, velocity,
							velocitySquaredLength, triangleTwo, t, intersectionPoint);
					if (!Float.isNaN(newT)) {
						t = newT;
						foundCollision = true;
					}

					// triangleThree
					newT = CollisionMath.checkUnitSphereVertexCollision(center, velocity,
							velocitySquaredLength, triangleThree, t, intersectionPoint);
					if (!Float.isNaN(newT)) {
						t = newT;
						foundCollision = true;
					}

					newT = CollisionMath.checkUnitSphereEdgeCollision(center, velocity,
							velocitySquaredLength, triangleOne, triangleTwo, t, intersectionPoint);
					if (!Float.isNaN(newT)) {
						t = newT;
						foundCollision = true;
					}

					newT = CollisionMath.checkUnitSphereEdgeCollision(center, velocity,
							velocitySquaredLength, triangleTwo, triangleThree, t, intersectionPoint);
					if (!Float.isNaN(newT)) {
						t = newT;
						foundCollision = true;
					}

					newT = CollisionMath.checkUnitSphereEdgeCollision(center, velocity,
							velocitySquaredLength, triangleThree, triangleOne, t, intersectionPoint);
					if (!Float.isNaN(newT)) {
						t = newT;
						foundCollision = true;
					}

					if (foundCollision) {
						itVb.scaleAdd(t, velocity, center);
						intersectionNormal.set(itVb).subtractLocal(intersectionPoint)
								.normalizeLocal();
						return t;
					}
				}
			}
		}

		return Float.NaN;

	}

	protected void updateWorldBound(Vector3f velocity) {
		float minX, maxX, minY, maxY, minZ, maxZ;
		if (velocity.x > 0) {
			minX = location.x - dimension.x;
			maxX = location.x + velocity.x + dimension.x;
		} else {
			maxX = location.x + dimension.x;
			minX = location.x + velocity.x - dimension.x;
		}

		if (velocity.y > 0) {
			minY = location.y - dimension.y;
			maxY = location.y + velocity.y + dimension.y;
		} else {
			maxY = location.y + dimension.y;
			minY = location.y + velocity.y - dimension.y;
		}

		if (velocity.z > 0) {
			minZ = location.z - dimension.z;
			maxZ = location.z + velocity.z + dimension.z;
		} else {
			maxZ = location.z + dimension.z;
			minZ = location.z + velocity.z - dimension.z;
		}

		float xRad = (maxX - minX) / 2;
		float yRad = (maxY - minY) / 2;
		float zRad = (maxZ - minZ) / 2;
		worldSpaceBounds.radius = Math.max(Math.max(xRad, yRad), zRad);
		worldSpaceBounds.getCenter().set(minX + xRad, minY + yRad, minZ + zRad);		
	}

	public Vector3f getDimension() {
		return dimension;
	}

}
