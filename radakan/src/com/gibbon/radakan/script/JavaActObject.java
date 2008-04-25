package com.gibbon.radakan.script;

import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.List;

public class JavaActObject implements ActComparableObject {

    private Object javaObj;
    private Class<?> javaClass;
    
    public JavaActObject(Object javaObj){
        this.javaObj = javaObj;
        this.javaClass = javaObj.getClass();
    }
    
    public Object getJavaObject(){
        return javaObj;
    }
    
    public Class<?> getJavaClass(){
        return javaClass;
    }
    
    public ActObject get(String name){
        try {
            if (Character.isUpperCase(name.charAt(0))){
                name = Character.toLowerCase(name.charAt(0)) + name.substring(1);
            }
            
            Field f = javaClass.getField(name);
            Object val = f.get(javaObj);
            if (val instanceof ActObject) {
                return (ActObject) val;
            } else {
                return new JavaActObject(javaObj);
            }
        } catch (IllegalArgumentException ex) {
            throw new ActRuntimeException("Undefined field (?): "+name+". "+ex.getLocalizedMessage());
        } catch (IllegalAccessException ex) {
            throw new ActRuntimeException("Not allowed to access field: "+name+". "+ex.getLocalizedMessage());
        } catch (NoSuchFieldException ex) {
            throw new ActRuntimeException("Undefined field: "+name+". "+ex.getLocalizedMessage());
        } catch (SecurityException ex) {
            throw new ActRuntimeException("Security exception accessing: "+name+". "+ex.getLocalizedMessage());
        }
    }

    public ActObject call(String name, List<ActObject> params){
        try {
            Class<?>[] paramTypes = new Class[params.size()];
            Object[] paramValues = new Object[params.size()];
            for (int i = 0; i < params.size(); i++) {
                ActObject param = params.get(i);
                if (param instanceof JavaActObject){
                    JavaActObject jo = (JavaActObject) param;
                    paramTypes[i] = jo.javaClass;
                    paramValues[i] = jo.javaObj;
                }else{
                    paramTypes[i] = param.getClass();
                    paramValues[i] = param;
                }
            }
            Method m = javaClass.getMethod(name, paramTypes);
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

    public boolean equals(ActObject other) {
        if (other instanceof JavaActObject){
            JavaActObject jo = (JavaActObject) other;
            return this.javaObj.equals(jo.javaObj);
        }
        return false;
    }

    @SuppressWarnings("unchecked")
    public int compareTo(ActComparableObject other) {
        if (other instanceof JavaActObject){
            JavaActObject jo = (JavaActObject) other;
            Object th = this.javaObj;
            Object ot = jo.javaObj;
            if ((this.javaClass == jo.javaClass)
             || (th instanceof jo.javaClass) 
             || (ot == this.javaClass)){
                if (th instanceof Comparable && ot instanceof Comparable){
                    Comparable c1 = (Comparable) th;
                    Comparable c2 = (Comparable) ot;
                    return c1.compareTo(c2);
                }
            }
            return this.javaObj.hashCode() - jo.javaObj.hashCode();
        }
        throw new ActRuntimeException("Cannot compare Java object to Act object");
    }
    
}
