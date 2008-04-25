package com.gibbon.radakan.script;

import java.util.ArrayList;
import java.util.List;

public class ActFunction implements ActCallable {
    
    private Class<? extends ActObject> retType = null;
    
    private List<Class<? extends ActObject>> paramTypes = new ArrayList<Class<? extends ActObject>>();
    private List<String> paramNames = new ArrayList<String>();
    
    private List<ActStatement> code;
    
    
    public ActFunction() {
    }

    public ActObject call(ActScope scope, List<ActObject> params){
        ActObject retval = null;
        
        // set the parameters into the scope
        for (int i = 0; i < params.size(); i++){
            scope.set(paramNames.get(i), params.get(i));
        }
        
        // execute the code
        for (ActStatement stat : code){
            stat.execute(scope);
            
            // if encountered return statement, read return value
            if (stat instanceof ActReturnStatement){
                retval = scope.get("ReturnValue");
                scope.set("ReturnValue", null);
            }
        }
        
        // clear parameters from scope and link back to caller
        for (int i = 0; i < params.size(); i++){
            scope.set(paramNames.get(i), null);
        }
        
        return retval;
    }

    public int compareTo(ActObject other) {
        return this.hashCode() - other.hashCode();
    }
    
}
