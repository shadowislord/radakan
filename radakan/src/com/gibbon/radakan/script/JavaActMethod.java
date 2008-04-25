package com.gibbon.radakan.script;

import java.lang.reflect.Method;
import java.util.List;


public class JavaActMethod implements ActCallable {

    private final Method javaMethod;
    
    public JavaActMethod(Method javaMethod){
        this.javaMethod = javaMethod;
    }

    public ActObject call(ActScope scope, List<ActObject> params) {
        try {
            Class<?>[] paramTypes = new Class[params.size()];
            Object[] paramValues = new Object[params.size()];
            
            for (int i = 0; i < params.size(); i++) {
                ActObject param = params.get(i);
                if (param instanceof JavaActObject){
                    JavaActObject jo = (JavaActObject) param;
                    paramTypes[i] = jo.getJavaClass();
                    paramValues[i] = jo.getJavaObject();
                }else{
                    paramTypes[i] = param.getClass();
                    paramValues[i] = param;
                }
            }
            
            ActObject th = scope.get("This");
            if (th != null && !(th instanceof ActNull)){
                if (th instanceof JavaActObject)
            }
            
            return (ActObject) m.invoke(javaObj, params);
        } catch (IllegalAccessException ex) {
            throw new ActRuntimeException("Not allowed to call method: "+name+". "+ex.getLocalizedMessage());
        } catch (IllegalArgumentException ex) {
            throw new ActRuntimeException("Illegal argument specified: "+name+". "+ex.getLocalizedMessage());
        } catch (InvocationTargetException ex) {
            throw new ActRuntimeException("Exception occured at invocation: "+name+". "+ex.getLocalizedMessage());
        } catch (NoSuchMethodException ex) {
            throw new ActRuntimeException("Undefined method: "+name+". "+ex.getLocalizedMessage());
        } catch (SecurityException ex) {
            throw new ActRuntimeException("Security exception accessing: "+name+". "+ex.getLocalizedMessage());
        }
    }

}
