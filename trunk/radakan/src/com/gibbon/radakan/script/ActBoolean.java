package com.gibbon.radakan.script;

public class ActBoolean implements ActPrimitive {

    private final boolean val;
    
    private static final ActBoolean TRUE = new ActBoolean(true),
                                    FALSE = new ActBoolean(false);
    
    private ActBoolean(boolean val){
        this.val = val;
    }
    
    public boolean getValue(){
        return val;
    }

    public static ActBoolean valueOf(boolean bool){
        return bool ? TRUE : FALSE;
    } 
   
    public boolean equals(ActObject other) {
        if (other instanceof ActBoolean)
            return val == ((ActBoolean)other).val;
        
        throw new ActRuntimeException("Cannot compare boolean to "+other.getClass().getSimpleName());
    }
    
}
