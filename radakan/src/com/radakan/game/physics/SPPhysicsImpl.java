package com.radakan.game.physics;

import com.jme.renderer.Renderer;
import com.jme.system.GameSettings;
import com.radakan.game.entity.unit.IPhysicsUnit;
import com.radakan.game.entity.unit.SPPhysicsUnit;
import com.radakan.game.world.GameSPWorldManager;
import com.radakan.game.world.GameWorldManager;

public class SPPhysicsImpl implements IPhysicsImpl {

    public GameWorldManager createPhysicsWorld(Renderer renderer, GameSettings settings) {
        return new GameSPWorldManager(renderer, settings);
    }

    public IPhysicsUnit createPhysicsUnit() {
        return new SPPhysicsUnit();
    }
    
}
