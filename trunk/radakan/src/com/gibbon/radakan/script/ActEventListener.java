package com.gibbon.radakan.script;

import java.util.List;

public class ActEventListener implements ActObject {
    
    private ActFunction cond;
    private ActFunction func;
    private ActScope scope;
    
    public ActEventListener(ActScope scope, ActFunction cond, ActFunction func){
        this.cond = cond;
        this.func = func;
        this.scope = scope;
    }
    
    public void onEvent(List<ActObject> params){
        ActBoolean ab = (ActBoolean) cond.call(scope, params);
        if (ab.getValue())
            func.call(scope, params);
    }
            
}
