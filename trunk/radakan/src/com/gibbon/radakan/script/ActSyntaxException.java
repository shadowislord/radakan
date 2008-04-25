package com.gibbon.radakan.script;


public class ActSyntaxException extends RuntimeException {
    
    public ActSyntaxException(int lineNum, String message) {
        super("At "+lineNum+": "+message);
    }
    
}
