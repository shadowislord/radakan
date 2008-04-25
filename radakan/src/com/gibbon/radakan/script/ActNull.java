package com.gibbon.radakan.script;

public class ActNull implements ActObject {

    private static final ActNull NULL = new ActNull();
    
    public static ActNull valueOf(){
        return NULL;
    }
    
    public boolean equals(ActObject other) {
        return other == NULL;
    }
    
}
