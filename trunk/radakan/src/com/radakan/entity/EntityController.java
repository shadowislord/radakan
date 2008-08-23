package com.radakan.entity;

import com.jme.scene.Controller;

/**
 * 
 * 
 * @author Momoko_Fan
 */
public class EntityController extends Controller {

	private static final long serialVersionUID = 1399402719676167634L;
	private Entity entity;
    
    public EntityController(Entity entity){
        this.entity = entity;
    }
    
    public void update(float tpf) {
        entity.update(tpf);
    }

}
