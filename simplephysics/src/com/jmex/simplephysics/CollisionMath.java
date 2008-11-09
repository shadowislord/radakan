package com.jmex.simplephysics;

import com.jme.math.FastMath;
import com.jme.math.Vector3f;

/**
 * <code>CollisionMath</code> provides a collection of math methods used for collision solving.<br>
 * Implementation of algorithm found at: http://www.peroxide.dk/papers/collision/collision.pdf, thanks to Ruab
 * @author Lucas Goraieb
 */
public class CollisionMath {

	private final static Vector3f possA = new Vector3f();

	private final static Vector3f possB = new Vector3f();

	private final static Vector3f possC = new Vector3f();

	/**
	 * Checks to see if two points are on the same side of a line
	 * @param p1 First point
	 * @param p2 Second point
	 * @param line1 Line endpoint
	 * @param line2 Second line endpoint
	 * @return true if the points are on the same side of a line
	 */
	public static boolean pointsOnSameSide(Vector3f p1, Vector3f p2, Vector3f line1, Vector3f line2) {
		possA.set(line2).subtractLocal(line1);
		possC.set(possA);

		possB.set(p1).subtractLocal(line1);
		possA.crossLocal(possB);

		possB.set(p2).subtractLocal(line1);
		possC.crossLocal(possB);
		if (possA.dot(possC) >= 0)
			return true;
		return false;
	}
	
	/**
	 * Checks to see if a point is inside of a triangle
	 * 
	 * @param point The point to check
	 * @param triangleOne Triangle point 1
	 * @param triangleTwo Triangle point 2
	 * @param triangleThree Triangle point 3
	 * @return true if the point is inside the triangle
	 */
	public static boolean isPointInTriangle(Vector3f point, Vector3f triangleOne, Vector3f triangleTwo,
			Vector3f triangleThree) {
		if (pointsOnSameSide(point, triangleOne, triangleTwo, triangleThree)
				&& pointsOnSameSide(point, triangleTwo, triangleOne, triangleThree)
				&& pointsOnSameSide(point, triangleThree, triangleOne, triangleTwo))
			return true;
		return false;
	}
	
	
	
	private final static Vector3f cusvcA = new Vector3f();

	/**
	 * Checks the unit sphere against a vertex
	 * 
	 * @param sphereCenter unit sphere center (ellipse space)
	 * @param sphereVelocity unit sphere velocity (ellipse space)
	 * @param velocitySquared velocity squared (pre-computed to save time)
	 * @param vertex the vertex to check against
	 * @param t the maximum value to return
	 * @param intersectionPoint return intersection point
	 * @return the intersection distance
	 */
	public static float checkUnitSphereVertexCollision(Vector3f sphereCenter, Vector3f sphereVelocity,
			float velocitySquared, Vector3f vertex, float t, Vector3f intersectionPoint) {
		float a = velocitySquared;
		float b = 2 * sphereVelocity.dot(cusvcA.set(sphereCenter).subtractLocal(vertex));
		float c = cusvcA.set(vertex).subtractLocal(sphereCenter).lengthSquared() - 1.0f;
		float newT = getLowestRoot(a, b, c, t);
		if (!Float.isNaN(newT)) {
			intersectionPoint.set(vertex);
		}
		return newT;
	}

	/**
	 * Finds the lowest root of a quadratic equation
	 * 
	 * @param a quadratic variable A
	 * @param b quadratic variable B
	 * @param c quadratic variable C
	 * @param maxR maximum value to be returned
	 * @return the lowest root (if less than maxR) or Float.NaN
	 */
	private static float getLowestRoot(float a, float b, float c, float maxR) {
		float determinant = b * b - 4 * a * c;
		if (determinant < 0)
			return Float.NaN;

		float sqrtd = FastMath.sqrt(determinant);
		float r1 = (-b - sqrtd) / (2 * a);
		float r2 = (-b + sqrtd) / (2 * a);

		if (r1 > r2) {
			float temp = r2;
			r2 = r1;
			r1 = temp;
		}

		if (r1 > 0 && r1 < maxR) {
			return r1;
		}

		if (r2 > 0 && r2 < maxR) {
			return r2;
		}

		return Float.NaN;
	}

	private final static Vector3f cusecA = new Vector3f(), cusecB = new Vector3f();

	/**
	 * Check to see if the moving unit sphere collides with a triangle edge
	 * 
	 * @param sphereCenter Center of the unit sphere (ellipsoid space)
	 * @param sphereVelocity Velocity of the unit sphere (ellipsoid space)
	 * @param velocitySquared Velocity squared (passed in so it doesnt need to be re-computed)
	 * @param vertexA First edge vertex
	 * @param vertexB Second edge vertex
	 * @param t maximum possible intersection distance (based on other
	 *            collisions)
	 * @param intersectionPoint The intersection point returned
	 * @return the distance of the collision, or Float.NaN if none is found, or
	 *         if it is farther away that parameter t.
	 */
	public static float checkUnitSphereEdgeCollision(Vector3f sphereCenter, Vector3f sphereVelocity,
			float velocitySquared, Vector3f vertexA, Vector3f vertexB, float t,
			Vector3f intersectionPoint) {
		cusecA.set(vertexB).subtractLocal(vertexA); // edge vector from pt A to
		// pt B
		cusecB.set(vertexA).subtractLocal(sphereCenter); // vector from the
		// center of the
		// sphere to pt A
		float edgeSquaredLength = cusecA.lengthSquared(); // squared length of
		// the edge
		float edgeDotVelocity = cusecA.dot(sphereVelocity); // cos of the angle
		// between the
		// velocity and the
		// edge
		float edgeDotBaseToVertex = cusecA.dot(cusecB); // cos of the angle
		// between the center->A
		// vector and the edge

		float a = edgeSquaredLength * -velocitySquared + edgeDotVelocity * edgeDotVelocity; // -velocity
		// squared
		// *
		// edge
		// len
		// squared
		// +
		// angle
		// between
		// velocity
		// and
		// edge
		// squared
		float b = edgeSquaredLength * (2 * sphereVelocity.dot(cusecB)) - 2 * edgeDotVelocity
				* edgeDotBaseToVertex;
		float c = edgeSquaredLength * (1 - cusecB.lengthSquared()) + edgeDotBaseToVertex
				* edgeDotBaseToVertex;

		float newT = getLowestRoot(a, b, c, t);
		if (!Float.isNaN(newT)) {
			float f = (edgeDotVelocity * newT - edgeDotBaseToVertex) / edgeSquaredLength;

			if (f >= 0 && f < 1) {
				intersectionPoint.scaleAdd(f, cusecA, vertexA);
				return newT;
			}
		}

		return Float.NaN;
	}

}
