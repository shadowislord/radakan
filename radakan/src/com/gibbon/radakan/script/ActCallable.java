package com.gibbon.radakan.script;

import java.util.List;


public interface ActCallable extends ActObject {

    public ActObject call(ActScope scope, List<ActObject> params);
    
}
