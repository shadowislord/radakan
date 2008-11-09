package com.jmex.simplephysics;

import com.jme.math.Vector3f;

/**
 * <code>PhysicState</code> provides phisical state of the object.
 * 
 * @author Lucas Goraieb
 */
public class PhysicState {

	public PhysicMaterial material = new PhysicMaterial();
	
	private float lastResistanceFactor;
	
	private int lastResistanceCount;
	
	public Vector3f velocity = new Vector3f();
	
	private Vector3f force = new Vector3f(); 

	private Vector3f gravityForce = new Vector3f(); 

	private Vector3f movingVelocity = new Vector3f(); 

	private Vector3f collisionNormal = new Vector3f();
	
	private int lastCollisionCount;
	
	private float lastNormalDot;
	
	/**
	 * Set the force to be applied to velocity
	 * @param force Force vector
	 */
	public void setForce(Vector3f force) {
		this.force.set(force);
	}
	
	/**
	 * Set the force to be applied to velocity
	 * @param x x axis force
	 * @param y y axis force
	 * @param z z axis force
	 */
	public void setForce(float x, float y, float z) {
		this.force.set(x, y, z);
	}
	
	/**
	 * Adds to the current force
	 * @param force Force vector
	 */
	public void addForce(Vector3f force) {
		this.force.addLocal(force);
	}

	/**
	 * Adds to the current force
	 * @param x x axis force
	 * @param y y axis force
	 * @param z z axis force
	 */
	public void addForce(float x, float y, float z) {
		this.force.addLocal(x, y, z);
	}
	
	/**
	 * Define the gravity force for this state
	 * @param gravityForce
	 */
	public void setGravity(Vector3f gravityForce) {
		this.gravityForce.set(gravityForce);
	}
	
	/**
	 * Define the gravity force for this state
	 * @param x gravity force
	 * @param y gravity force
	 * @param z gravity force
	 */
	public void setGravity(float x, float y, float z) {
		this.gravityForce.set(x, y, z);
	}
	
	private static Vector3f mNewPosition = new Vector3f();
	
	protected void move(DynamicCollider collider, CollisionScene scene, float time) {
		applyForce(time);
		applyResistance();
		lastCollisionCount = 0;
		lastResistanceCount = 0;
		lastResistanceFactor = 0;
		mNewPosition.set(scene.collisionSolver.move(collider, scene, velocity, time));
		if (!((lastCollisionCount > 0) && (movingVelocity.length() < time))) {
			collider.updateLocation(mNewPosition);
		}
	}
	
	private static Vector3f acceleration = new Vector3f();

	private static Vector3f resultantForce = new Vector3f();

	private static Vector3f normalForce = new Vector3f();

	protected void applyForce(float time) {
		resultantForce.set(force).divideLocal(material.mass);
		if (lastCollisionCount > 0) {
			if (lastNormalDot < material.slopeFactor) {
				resultantForce.addLocal(gravityForce);
			}
			normalForce.set(collisionNormal).multLocal(resultantForce.length()*2);
			resultantForce.addLocal(normalForce);
		} else {
			resultantForce.addLocal(gravityForce);
		}
		acceleration.set(resultantForce).multLocal(time);
		velocity.addLocal(acceleration);
		
		resultantForce.set(force).divideLocal(material.mass);
		acceleration.set(resultantForce).multLocal(time);
		movingVelocity.addLocal(acceleration);
	}
	
	private static Vector3f arResistanceVelocity = new Vector3f();
	
	private static float arResistance;

	protected void applyResistance() {
		if (lastResistanceCount == 0) {
			arResistance = 0;
		} else {
			arResistance = (lastResistanceFactor) / (lastResistanceCount);
		}
		if (arResistance < material.resistance) {
			arResistance = material.resistance;
		}
		arResistanceVelocity.set(velocity).multLocal(arResistance);
		velocity.subtractLocal(arResistanceVelocity);

		arResistanceVelocity.set(movingVelocity).multLocal(arResistance*2);
		movingVelocity.subtractLocal(arResistanceVelocity);
	}

	private static Vector3f acResultant = new Vector3f();

	private static float acBounce;
	
	private static Vector3f acNormal = new Vector3f();

	protected void addCollision(Collider collided, Vector3f normal, Vector3f vel) {
		lastCollisionCount++;
		lastResistanceCount++;
		lastResistanceFactor += collided.getPhysicMaterial().resistance;
		
		collisionNormal.set(normal);
		lastNormalDot = -acNormal.set(gravityForce).normalizeLocal().dot(collisionNormal);
		
		if (collided.getPhysicMaterial().bounceEnabled && material.bounceEnabled) {
			acBounce = 1+((material.bounciness + collided.getPhysicMaterial().bounciness) * 0.5f);
			acResultant.set(normal).multLocal(vel.length() * acBounce);
			velocity.addLocal(acResultant);
		}
		
		if (collided instanceof DynamicCollider) {
			((DynamicCollider) collided).physicState.velocity.subtractLocal(acResultant.divideLocal(collided.getPhysicMaterial().mass));
			((DynamicCollider) collided).physicState.movingVelocity.set(((DynamicCollider) collided).physicState.velocity);
		}
		
	}
	
}
