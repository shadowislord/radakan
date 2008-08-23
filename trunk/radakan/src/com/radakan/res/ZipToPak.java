package com.radakan.res;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;
import java.util.Enumeration;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;

/**
 * Converts ZIP file to PAK file
 * 
 * @author Kirill
 */
public class ZipToPak {

    public static void main(String[] args) throws IOException{
        if (args.length == 0)
            args = new String[]{ System.getProperty("user.dir") + "/data.zip" };
        
        File zipFile = new File(args[0]);
        File pakFile = new File(args[0]+".pak");
        pakFile.createNewFile();
        
        ZipFile zip = new ZipFile(zipFile, ZipFile.OPEN_READ);
        FileChannel fc = new FileOutputStream(pakFile).getChannel();
        
        int nFiles = 0;
        for (Enumeration<? extends ZipEntry> e = zip.entries(); e.hasMoreElements(); e.nextElement()){
            nFiles ++; 
        }
        
        ByteBuffer header = ByteBuffer.allocateDirect(4 + 12 * nFiles);
        header.putInt(nFiles);
        
        // total size of the result pak file
        int totalSize = header.capacity();
        
        for (Enumeration<? extends ZipEntry> e = zip.entries(); e.hasMoreElements();){
            ZipEntry entry = e.nextElement();
            int hash = entry.getName().hashCode();
            int size = (int) entry.getSize();
            int pointer = totalSize;
            
            totalSize += size;
            
            header.putInt(hash).putInt(pointer).putInt(size);
        }
        header.flip();
        
        // add header to pak file
        fc.write(header);
        
        // write file datas to pak file
        for (Enumeration<? extends ZipEntry> e = zip.entries(); e.hasMoreElements();){
            ZipEntry entry = e.nextElement();
            InputStream in = zip.getInputStream(entry);
            byte[] entryData = new byte[(int)entry.getSize()];
            in.read(entryData);
            
            ByteBuffer buf = ByteBuffer.wrap(entryData);
            fc.write(buf);
        }
        
        zip.close();
        fc.close();
    }
    
}
