package com.gibbon.radakan.res;

import com.jme.scene.Spatial;
import java.io.File;
import java.io.InputStream;
import java.util.logging.Level;
import java.util.logging.Logger;


public class TestResources {

    public static void main(String[] args){
        try {
            FileSystem fileSystem = new ZipFileSystem(new File("data.zip"));
            ResourceManager.setFileSystem(fileSystem);
            ResourceManager.registerLoader(InputStream.class, new RawResourceLoader());
            ResourceManager.registerLoader(Spatial.class, new ModelLoader());

            Thread.sleep(1750);
            ResourceManager.removeExpiredFromCache();
        } catch (InterruptedException ex) {
            Logger.getLogger(TestResources.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    
}
