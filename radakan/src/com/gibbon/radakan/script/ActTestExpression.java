package com.gibbon.radakan.script;

public class ActTestExpression implements ActExpression {

    private ActExpression left, right;
    private TestOperation op;
    
    public enum TestOperation {
        EQUALS,
        NOT_EQUALS,
        LESS_THAN,
        GREATER_THAN,
        LESS_THAN_OR_EQUAL,
        GREATER_THAN_OR_EQUAL,
    }
    
    public ActObject resolve(ActScope scope) {
        ActObject leftVal = left.resolve(scope);
        ActObject rightVal = right.resolve(scope);
        switch (op){
            case EQUALS:
                return ActBoolean.valueOf(leftVal.equals(rightVal));
            case NOT_EQUALS:
                return ActBoolean.valueOf(!leftVal.equals(rightVal));
            default:
                if (leftVal instanceof ActComparableObject
                 && rightVal instanceof ActComparableObject){
                    ActComparableObject co1 = (ActComparableObject) leftVal;
                    ActComparableObject co2 = (ActComparableObject) rightVal;
                    switch (op){
                        case LESS_THAN:
                            return ActBoolean.valueOf(co1.compareTo(co2) < 0);
                        case GREATER_THAN:
                            return ActBoolean.valueOf(co1.compareTo(co2) > 0);
                        case LESS_THAN_OR_EQUAL:
                            return ActBoolean.valueOf(co1.compareTo(co2) <= 0);
                        case GREATER_THAN_OR_EQUAL:
                            return ActBoolean.valueOf(co1.compareTo(co2) >= 0);
                    }
                }else{
                    throw new ActRuntimeException("Cannot compare "+leftVal.getClass().getSimpleName()+" and "+rightVal.getClass().getSimpleName());
                }
        }
        
        throw new ActRuntimeException("Undefined operation");
    }

}
