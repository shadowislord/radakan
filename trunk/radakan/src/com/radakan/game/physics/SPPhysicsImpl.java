package com.radakan.game.physics;

import com.jme.renderer.Renderer;
import com.jme.system.GameSettings;
import com.radakan.entity.unit.IPhysicsUnit;
import com.radakan.entity.unit.SPPhysicsUnit;
import com.radakan.game.world.SPWorld;
import com.radakan.game.world.World;

public class SPPhysicsImpl implements IPhysicsImpl {

    public World createPhysicsWorld(Renderer renderer, GameSettings settings) {
        return new SPWorld(renderer, settings);
    }

    public IPhysicsUnit createPhysicsUnit() {
        return new SPPhysicsUnit();
    }
    
}
