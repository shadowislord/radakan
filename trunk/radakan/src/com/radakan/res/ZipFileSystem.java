package com.radakan.res;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;
import java.nio.channels.Channels;
import java.nio.channels.ReadableByteChannel;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;

public class ZipFileSystem implements IFileSystem {

    protected ZipFile zipFile;
    
    public ZipFileSystem(File file){
        try {
            zipFile = new ZipFile(file, ZipFile.OPEN_READ);
        } catch (IOException ex) {
            ex.printStackTrace();
        }
    }
    
    public InputStream openStream(AccessMode mode, String name) throws IOException {
        ZipEntry entry = zipFile.getEntry(name);
        if (entry == null || entry.isDirectory())
            throw new IOException("Invalid resource identifier specified: "+name);
        
        return zipFile.getInputStream(entry);
    }

    public ReadableByteChannel openChannel(AccessMode mode, String name) throws IOException  {
        return Channels.newChannel(openStream(mode,name));
    }

    public ByteBuffer openBuffer(AccessMode mode, String name) throws IOException {
        ZipEntry entry = zipFile.getEntry(name);
        if (entry == null || entry.isDirectory())
            throw new IOException("Invalid resource identifier specified: "+name);
        
        ByteBuffer bb = ByteBuffer.allocateDirect((int)entry.getSize());
        InputStream in = zipFile.getInputStream(entry);
        return bb;
    }

    public boolean exists(String name) {
        return zipFile.getEntry(name) != null;
    }

    
    
}
