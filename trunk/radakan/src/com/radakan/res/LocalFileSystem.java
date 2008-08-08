package com.radakan.res;

import com.jme.util.geom.BufferUtils;
import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;
import java.nio.MappedByteBuffer;
import java.nio.channels.FileChannel;
import java.nio.channels.FileChannel.MapMode;
import java.nio.channels.ReadableByteChannel;

/**
 * A FileSystem that accesses resources in the root directory on the local machine.
 */
public class LocalFileSystem implements FileSystem {

    /**
     * Flag to allow usage of file mapping.
     * File mapping improves IO performance for large files, since
     * they are mapped to the swap partition/file on supporting platforms.
     */
    private static final boolean ALLOW_FILE_MAPPING = true;
    
    /**
     * The root directory from which file paths are resolved.
     * Usually this is the working directory of the application.
     */
    private final File rootFile;
    
    public LocalFileSystem(){
        String root = System.getProperty("user.dir");
        if (root == null || root.equals(""))
            throw new RuntimeException("Cannot locate working directory");
        
        rootFile = new File(root);
    }
    
    private File getFileHandle(String path) throws IOException {
        File file = new File(rootFile, path);
        
        if (!file.exists())
            throw new FileNotFoundException("Cannot locate resource "+path);
        
        if (!file.canRead()){
            file.setReadable(true);
            if (!file.canRead())
                throw new IOException("Unable to access file "+path);
        }
        
        if (file.length() >= Integer.MAX_VALUE)
            throw new IOException("Cannot handle files larger than 4 GB");
        
        return file;
    }
    
    public InputStream openStream(AccessMode mode, String name) throws IOException {
        File file = getFileHandle(name);
        
        // Buffering is useless for random and raw access.
        // though it is difficult to imagine how one would do it
        // with 
        if (mode == AccessMode.PARSE || mode == AccessMode.STREAM)
            return new BufferedInputStream(new FileInputStream(file));
        else
            return new FileInputStream(file);
    }

    public ReadableByteChannel openChannel(AccessMode mode, String name) throws IOException {
        File file = getFileHandle(name);
        
        FileInputStream fin = new FileInputStream(file);
        return fin.getChannel();
    }

    public ByteBuffer openBuffer(AccessMode mode, String name) throws IOException {
        File file = getFileHandle(name);
        
        FileInputStream fin = new FileInputStream(file);
        FileChannel fc = fin.getChannel();
        if (mode != AccessMode.PARSE && ALLOW_FILE_MAPPING){
            MappedByteBuffer buf = fc.map(MapMode.READ_ONLY, 0, file.length());
            return buf;
        }else{
            // getFileHandle call makes sure file length is below 4 GB
            ByteBuffer buf = BufferUtils.createByteBuffer((int)file.length());
            return buf;
        }
    }

    public boolean exists(String name) {
        File file = new File(rootFile, name);
        return file.exists() && file.canRead();
    }

}
