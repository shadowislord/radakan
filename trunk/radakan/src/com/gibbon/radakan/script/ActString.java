package com.gibbon.radakan.script;

public class ActString implements ActPrimitive {

    private final String value;
    
    private ActString(String value){
        this.value = value;
    }
    
    public static ActString valueOf(String string){
        return new ActString(string);
    }
    
    public String getValue(){
        return value;
    }

    public boolean equals(ActObject other) {
        if (other instanceof ActString)
            return value.equals(((ActString)other).value);
        
        throw new ActRuntimeException("Cannot compare Act string to "+other.getClass().getSimpleName());
    }
    
}
