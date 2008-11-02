package com.radakan.game.physics;

import com.jme.renderer.Renderer;
import com.jme.system.GameSettings;
import com.radakan.game.entity.unit.IPhysicsUnit;
import com.radakan.game.world.GameWorldManager;

public interface IPhysicsImpl {
    /**
     * Creates a World that supports physics handling,
     * using this Physics' implementation.
     * 
     * @param renderer Renderer object that will render the world
     * @param settings Game settings to extract various parameters
     * @return A World that will update physics handling for all entities
     * with IPhysicsUnit of it's implementation.
     */
    public GameWorldManager createPhysicsWorld(Renderer renderer, GameSettings settings);
    
    /**
     * @return a PhysicsUnit that uses this implementation for physics management.
     */
    public IPhysicsUnit createPhysicsUnit();
    
}
