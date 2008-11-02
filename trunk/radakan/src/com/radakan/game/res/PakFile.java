package com.radakan.game.res;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.ByteBuffer;
import java.nio.channels.Channels;
import java.nio.channels.WritableByteChannel;
import java.util.HashMap;
import java.util.Map;

public class PakFile {

    private Map<String, ByteBuffer> entries = new HashMap<String, ByteBuffer>();

    public PakFile(){
    }
    
    public void addEntry(String name, ByteBuffer data){
        entries.put(name, data);
    }
    
    public void addEntry(String name, InputStream in, int size) throws IOException{
        byte[] b = new byte[size];
        in.read(b);
        ByteBuffer data = ByteBuffer.wrap(b);
        data.rewind();
        addEntry(name, data);
    }
    
    public void save(OutputStream out) throws IOException{
        save(Channels.newChannel(out));
    }
    
    public void save(WritableByteChannel out) throws IOException{
        // generate table
        int nFiles = entries.size();
        
        ByteBuffer header = ByteBuffer.allocateDirect(4 + nFiles * 12);
        header.putInt(nFiles);
        
        int totalSize = header.capacity();
        
        for (Map.Entry<String, ByteBuffer> entry : entries.entrySet()){
            int hash = entry.getKey().hashCode();
            int size = entry.getValue().capacity();
            int pointer = totalSize;
            
            header.putInt(hash).putInt(pointer).putInt(size);
            
            totalSize += size;
        }
        
        header.flip();
        out.write(header);
        
        // now write the entries
        for (Map.Entry<String, ByteBuffer> entry : entries.entrySet()){
            out.write(entry.getValue());
        }
    }
    
}
