package com.radakan.res;

import java.io.ByteArrayInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.channels.Channels;
import java.nio.channels.FileChannel;
import java.nio.channels.FileChannel.MapMode;
import java.nio.channels.ReadableByteChannel;
import java.util.HashMap;
import java.util.logging.Level;
import java.util.logging.Logger;


/**
 * File system that reads from PAK files.
 * PAK is an archive format. Currently it's very simple
 * files are stored uncompressed. 
 * Use ZipToPak utility to convert ZIP archives into PAK
 * 
 * @author Kirill Vainer
 */
public class PakFileSystem implements FileSystem {

    private HashMap<Integer, TableEntry> hashToPointer;
    private FileChannel pakFileData;
    private FileInputStream pakFileIn;
    
    private boolean useFileMapping = true;
    
    private static class TableEntry { 
        int pointer, size;
    }
    
    public PakFileSystem(File pakFile){
        try {
            pakFileIn = new FileInputStream(pakFile);
            pakFileData = pakFileIn.getChannel();
            
            ByteBuffer fileCount = ByteBuffer.allocateDirect(4);
            pakFileData.read(fileCount);
            int nFiles = fileCount.getInt(0);
            hashToPointer = new HashMap<Integer, TableEntry>(nFiles);

            ByteBuffer table = ByteBuffer.allocateDirect(nFiles * 12);
            pakFileData.read(table);
            table.rewind();
            
            //int curPos = 4;
            for (int i = 0; i < nFiles; i++){
                int hash = table.getInt();
                TableEntry entry = new TableEntry();
                entry.pointer = table.getInt();
                entry.size = table.getInt();
                hashToPointer.put(hash, entry);
                //curPos += 12; // hash + pointer + size
            }
        } catch (IOException ex) {
            Logger.getLogger(PakFileSystem.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    
    private TableEntry getEntry(String name) throws FileNotFoundException{
        int hash = name.hashCode();
        TableEntry entry = hashToPointer.get(hash);
        if (entry == null)
            throw new FileNotFoundException("File "+name+" cannot be found");
        
        return entry;
    }
    
    public InputStream openStream(AccessMode mode, String name) throws IOException {
        TableEntry entry = getEntry(name);
        
        byte[] data = new byte[entry.size];
        ByteBuffer buf = ByteBuffer.wrap(data);
        pakFileData.read(buf, entry.pointer);
        
        return new ByteArrayInputStream(data);
    }

    public ReadableByteChannel openChannel(AccessMode mode, String name) throws IOException {
        return Channels.newChannel(openStream(mode,name));
    }

    public ByteBuffer openBuffer(AccessMode mode, String name) throws IOException {
        // find entry in map
        TableEntry entry = getEntry(name);
        
        if (useFileMapping)
            return pakFileData.map(MapMode.READ_ONLY, entry.pointer, entry.size);
        else{
            ByteBuffer buf = ByteBuffer.allocateDirect(entry.size).order(ByteOrder.nativeOrder());
            pakFileData.read(buf, entry.pointer);
            return buf;
        }
    }

    public boolean exists(String name) {
        return hashToPointer.containsKey(name);
    }

}
