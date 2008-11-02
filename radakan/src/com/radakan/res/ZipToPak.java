package com.radakan.res;

import com.radakan.game.res.PakFile;
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
            args = new String[]{ "data.zip" };
        
        File zipFile = new File(args[0]);
        File pakFile = new File(args[0]+".pak");
        pakFile.createNewFile();
        
        ZipFile zip = new ZipFile(zipFile, ZipFile.OPEN_READ);
        FileChannel fc = new FileOutputStream(pakFile).getChannel();
        
        PakFile pak = new PakFile();
        
        // write file datas to pak file
        for (Enumeration<? extends ZipEntry> e = zip.entries(); e.hasMoreElements();){
            ZipEntry entry = e.nextElement();
            InputStream in = zip.getInputStream(entry);
            pak.addEntry(entry.getName(), in, (int) entry.getSize());
            in.close();
        }
        
        zip.close();
        
        pak.save(fc);
        fc.close();
    }
    
}
