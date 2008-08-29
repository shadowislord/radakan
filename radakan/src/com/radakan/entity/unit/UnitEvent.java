package com.radakan.entity.unit;

import com.radakan.entity.Entity;

public class UnitEvent {

    protected Object source;
    protected Entity entity;
    protected String type;
    
    public static final String ATTACH = "Attach",
                               DETACH = "Detach",
                               ENTITY_BIRTH   = "EntityBirth",
                               ENTITY_DISPOSE = "EntityDispose";
    
    public void setSource(Object source){
        this.source = source;
    }
    
    public void setEntity(Entity entity){
        this.entity = entity;
    }
    
    public Object getSource(){
        return source;
    }
    
    public Entity getEntity(){
        return entity;
    }
    
    public void setType(String type){
        this.type = type;
    }
    
    public String getType(){
        return type;
    }
    
    @Override
    public String toString(){
        return getClass().getSimpleName()+"(source="+source+", entity="+entity+", type="+type+")";
    }
    
}
