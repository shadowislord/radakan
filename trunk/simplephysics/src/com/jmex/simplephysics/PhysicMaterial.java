package com.jmex.simplephysics;

/**
 * <code>PhysicMaterial</code> defines the colliders physic properties.
 * 
 * @author Lucas Goraieb
 */
public class PhysicMaterial {

	float resistance = 0.05f;
	
	float mass = 1;
	
	float bounciness = 0;

	boolean bounceEnabled = true;
	
	float slopeFactor = 1;

	/**
	 * Convenience method to define the properties for static objects
	 * @param resistance Friction factor: 0 (no resistance) to 1 (max resistance)
	 * @param bounciness Bounce collision reaction factor (from 0 to 1)
	 * @param bounce Enable/disable bounce
	 */
	public void set(float resistance, float bounciness, boolean bounce) {
		set(resistance, 1, 0, bounciness,  bounce);
	}
	
	/**
	 * Convenience method do define all properties at once
	 * @param resistance Friction factor: 0 (no resistance) to 1 (max resistance)
	 * @param mass Object's mass (if lower or equal to 0, it turns to 1)
	 * @param slopeFactor Defines how object should slide down (from 0 to 1)
	 * @param bounciness Bounce collision reaction factor (from 0 to 1)
	 * @param bounce Enable/disable bounce
	 */
	public void set(float resistance, float mass, float slopeFactor, float bounciness, boolean bounce) {
		setResistance(resistance);
		setMass(mass);
		setSlopeFactor(slopeFactor);
		setBounciness(bounciness);
		setBounceEnabled(bounce);
	}

	/**
	 * Set the inclination factor to add gravity force.<br>
	 * If slopeFactor = 1 the object will always slide down<br>
	 * If slopeFactor = 0 the object will always "stick" to ground or wall, allowing climbing<br>
	 * Normally for a sliding wall use 0.7f as slopeFactor.
	 * @param slopeFactor Value between 0 and 1.
	 */
	public void setSlopeFactor(float slopeFactor) {
		if (slopeFactor > 1) {
			this.slopeFactor = 1;
		} else if (slopeFactor < 0) {
			this.slopeFactor = 0;
		} else {
			this.slopeFactor = slopeFactor;
		}
	}

	/**
	 * Enable or disable bounce reaction
	 * @param bounceEnabled
	 */
	public void setBounceEnabled(boolean bounceEnabled) {
		this.bounceEnabled = bounceEnabled;
	}

	/**
	 * The more bounciness the more the reaction 
	 * @param bounciness Bounce collision reaction factor (from 0 to 1)
	 */
	public void setBounciness(float bounciness) {
		if (bounciness > 1) {
			this.bounciness = 1;
		} else if (bounciness < 0) {
			this.bounciness = 0;
		} else {
			this.bounciness = bounciness;
		}
	}

	/**
	 * Object's mass 
	 * @param mass Object's mass (if lower or equal to 0, it turns to 1)
	 */
	public void setMass(float mass) {
		if (mass <= 0) {
			this.mass = 1;
		} else {
			this.mass = mass;
		}
	}

	/**
	 * Object's resistance
	 * @param resistance Friction factor: 0 (no resistance) to 1 (max resistance)
	 */
	public void setResistance(float resistance) {
		if (resistance < 0) {
			this.resistance = 0;
		} else if (resistance > 1) {
			this.resistance = 1;
		} else {
			this.resistance = resistance;
		}
	}

}
