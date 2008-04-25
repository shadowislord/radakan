package com.gibbon.radakan.script;

import com.gibbon.mfkarpg.entity.Entity;
import java.util.List;


public class ActEntity  {

    private final Entity jentity;
    
    public ActEntity(Entity jentity){
        this.jentity = jentity;
        
    }
    
    public ActObject get(String name) {
        return mapping.get(name);
    }

    public ActObject call(String name, List<ActObject> params) {
        ActObject ao = mapping.get(name);
        if (!(ao instanceof ActFunction))
            throw new ActRuntimeException("Specified name "+name+" is not a function");
        
        ActFunction af = (ActFunction) ao;
        return af.call(name, params);
    }
    
}
