package com.radakan.res;

import java.io.IOException;

/**
 * An interface for resource loaders.
 * 
 * ResourceLoader must retrieve the resource data
 * based on a resource identifier.
 * ResourceLoaders should use the ResourceManager
 * class for accessing the filesystem.
 * 
 * @author Momoko_Fan
 * @param T The resource data type.
 */
public interface ResourceLoader<T> {

    /**
     * Called when a resource of type T needs to be loaded.
     * 
     * @param resourceName The resource name of the resource which should be loaded.
     * @return The resource data.
     * @throws java.io.IOException If an error occured while parsing or reading the file.
     */
    public T load(String resourceName) throws IOException ;

}
