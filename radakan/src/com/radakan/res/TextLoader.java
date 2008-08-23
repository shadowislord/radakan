package com.radakan.res;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.CharBuffer;

/**
 * Loads TXT files and similars (e.g shaders)
 * 
 * @author Kirill Vainer
 */
public class TextLoader implements IResourceLoader<String> {

    public String load(String resourceName) throws IOException {
        ByteBuffer buf = ResourceManager.getFileSystem().openBuffer(AccessMode.PARSE, resourceName);
        byte[] data = new byte[buf.capacity()];
        buf.get(data);
        
        return new String(data);
    }

    public boolean useCache() {
        return true;
    }

}
