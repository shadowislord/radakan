package com.gibbon.radakan.res;

import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;
import java.nio.channels.ReadableByteChannel;


public interface FileSystem {

    /**
     * Opens an InputStream for the specified resource identifier.
     * 
     * @param mode The mode in which the data will be handled.
     * @param name The name of the resource.
     * @return An input stream which contains the data of the resource.
     * @throws java.io.IOException If an IO error occurs.
     */
    public InputStream openStream(AccessMode mode, String name) throws IOException;
    
    /**
     * Opens a Channel for the specified resource identifier
     * 
     * @param mode The mode in which the data will be handled.
     * @param name The name of the resource.
     * @return A channel which contains the data of the resource.
     * @throws java.io.IOException If an IO error occurs.
     */
    public ReadableByteChannel openChannel(AccessMode mode, String name) throws IOException;
    
    /**
     * Opens a ByteBuffer for the specified resource identifier.
     * This is mostly used for the AccessMode RANDOM since 
     * ByteBuffers allow random seeking and access.
     * 
     * @param mode
     * @param name
     * @return
     * @throws java.io.IOException
     */
    public ByteBuffer openBuffer(AccessMode mode, String name) throws IOException;
    
    /**
     * Returns true if the resource specified exists in the file system.
     * 
     * @param name The name of the resource
     * @return Wether the resource exists or not.
     */
    public boolean exists(String name);
    
}
