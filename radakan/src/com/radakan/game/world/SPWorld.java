package com.radakan.game.world;

import com.jme.renderer.Renderer;
import com.jme.system.GameSettings;
import com.jmex.simplephysics.Collider;
import com.jmex.simplephysics.CollisionScene;
import com.radakan.game.entity.Entity;
import com.radakan.game.entity.unit.SPPhysicsUnit;
import com.radakan.game.tile.Tile;

public class SPWorld extends GameWorldManager {

    protected CollisionScene physicsScene;
    
    public SPWorld(Renderer renderer, GameSettings settings){
        super(renderer, settings);
        physicsScene = new CollisionScene();
    }
    
    @Override
    public void updateGeometricState(float tpf, boolean initiator){
        super.updateGeometricState(tpf, initiator);
        physicsScene.update(tpf);
    }
    
    @Override
    public void addEntity(Entity ent, Tile t){
        super.addEntity(ent, t);
        SPPhysicsUnit physUnit = ent.getUnit(SPPhysicsUnit.class);
        physicsScene.attachDynamic(physUnit.getCollider());
    }
    
    @Override
    public void removeEntity(Entity ent, Tile t){
        super.removeEntity(ent, t);
        SPPhysicsUnit physUnit = ent.getUnit(SPPhysicsUnit.class);
        //physicsScene.removeDynamic(physUnit.getCollider());
    }
    
}
