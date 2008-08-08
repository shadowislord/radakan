package com.radakan.res;

/**
 * An interface for ResourceHandlers.
 * 
 * Resource handling is done after the resource is loaded,
 * generally in order to use the resource in some context.
 * 
 * @author Daniel
 * @param T The type of the resource data.
 */
public interface ResourceHandler<T> {

    /**
     * A callback method which is called when
     * loading of resource is done.
     * 
     * @param resourceName The resource which finished loading.
     * @param data The resource data.
     */
    public void handle(String resourceName, T data);
    
    /**
     * @return ResourceHandler should return true
     * if the <code>handle()</code> method
     * should run in the rendering thread.
     * If false, the handle method will
     * be called in the loading thread.
     */
    public boolean runInRender();
    
}
