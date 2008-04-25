package com.gibbon.radakan.script;

public class ActLiteralExpression implements ActExpression {

    // literals are evaluated at compile time
    private ActObject value;
    
    public ActLiteralExpression(String literal){
        // string literal
        value = ActString.valueOf(literal);
    }
    
    public ActLiteralExpression(boolean literal){
        value = ActBoolean.valueOf(literal);
    }
    
    public ActObject resolve(ActScope scope) {
        return value;
    }

}
