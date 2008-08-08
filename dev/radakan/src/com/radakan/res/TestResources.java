package com.radakan.res;

import com.jme.scene.Spatial;
import java.io.IOException;
import java.io.InputStream;
import java.util.logging.Level;
import java.util.logging.Logger;


public class TestResources {

    public static void main(String[] args) throws IOException{
        try {
//            FileSystem fileSystem = new ZipFileSystem(new File("data.zip"));
            FileSystem fileSystem = new DefaultFileSystem("icons");
            ResourceManager.setFileSystem(fileSystem);
			
            ResourceManager.registerLoader(InputStream.class, new RawResourceLoader());
            ResourceManager.registerLoader(Spatial.class, new ModelLoader());
			
            Thread.sleep(1750);
            ResourceManager.removeExpiredFromCache();
			System.err.println("Found: " + ResourceManager.loadResource(InputStream.class, "WT_logo.png"));
        } catch (InterruptedException ex) {
            Logger.getLogger(TestResources.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    
}
