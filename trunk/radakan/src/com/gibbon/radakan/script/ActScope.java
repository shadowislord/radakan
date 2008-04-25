package com.gibbon.radakan.script;

import java.util.List;

public interface ActScope {

    public void set(String name, ActObject value);
    public ActObject get(String name);
    public ActObject call(ActScope rootScope, String name, List<ActObject> params);
    public ActScope mergeWith(ActScope other);
    
}
