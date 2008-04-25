package com.gibbon.radakan.script;


public class ActReturnStatement implements ActStatement {

    private final ActExpression retExpr;
    
    public ActReturnStatement(){
    }
    
    public ActReturnStatement(ActExpression returnExpression){
        retExpr = returnExpression;
    }
    
    public ActStatementType getType() {
        return ActStatementType.RETURN;
    }

    public void execute(ActScope scope) {
        scope.set("ReturnValue", retExpr.resolve(scope) );
    }

}
