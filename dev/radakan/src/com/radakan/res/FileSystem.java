package com.radakan.res;

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
     * WARNING: Using bytebuffers for IO within java is slow.
     * Only use when interaction with native libraries is involved (e.g OpenGL).
     * 
     * @param mode
     * @param name
     * @return Buffer opened for the specified access mode
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
