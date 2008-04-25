package com.gibbon.radakan.script;

public interface ActStatement {
    
    public enum ActStatementType {
        IF,
        ELSEIF,
        ELSE,
        DEFINE_VAR,
        DEFINE_FUNC,
        CALL,
        ASSIGNMENT,
        EXPRESSION,
        EVENT
    }
    
    public ActStatementType getType();
    
    public void execute(ActScope scope);
    
}
