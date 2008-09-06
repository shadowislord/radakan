package com.radakan.jme.hdrreader;

import com.jme.app.SimpleGame;
import com.jme.image.Texture;
import com.jme.image.Texture.MagnificationFilter;
import com.jme.image.Texture.MinificationFilter;
import com.jme.image.Texture2D;
import com.jme.scene.state.TextureState;
import com.jme.util.TextureManager;
import com.radakan.res.ResourceManager;
import com.radakan.res.TextureLoader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.net.MalformedURLException;
import java.util.logging.Handler;
import java.util.logging.Level;
import java.util.logging.LogRecord;
import java.util.logging.Logger;


public class RushLoader extends SimpleGame {

    public static void main(String[] args) {
        //TextureManager.registerHandler(".hdr", new HDRLoader());
        //TextureManager.registerHandler(".dds", new DDSLoader());

        ResourceManager.registerLoader(Texture.class, new TextureLoader());
        
        RushLoader app = new RushLoader();
        app.setConfigShowMode(ConfigShowMode.AlwaysShow);
        app.start(); 
    }

    /*
     * (non-Javadoc)
     * 
     * @see com.jme.app.SimpleGame#initGame()
     */
    protected void simpleInitGame() {
            display.setTitle("jME - HDR Loading");
            
            long totalReadMillis = 0;
            long totalUploadMillis = 0;
            
            File inputDir = new File("D:\\compressed\\");
            for (File f : inputDir.listFiles()){
            {
                if (f.isDirectory()) continue;
                //if (!f.getName().endsWith(".dds")) continue;
                
                InputStream in = null;
                try {
                    long inMillis = System.currentTimeMillis();
                    System.out.println("READING: " + f);
                    Texture t = TextureManager.loadTexture(f.toURI().toURL());
                    //in = new FileInputStream(f);
                    //Texture t = new Texture2D();
                    //t.setImage(DDSLoader.loadImage(in));
                    //in.close();
                    TextureState state = display.getRenderer().createTextureState();
                    t.setMinificationFilter(MinificationFilter.Trilinear);
                    t.setMagnificationFilter(MagnificationFilter.Bilinear);
                    state.setTexture(t);
                    long outMillis = System.currentTimeMillis();
                    totalReadMillis += outMillis - inMillis;

                    inMillis = System.currentTimeMillis();
                    state.load();
                    outMillis = System.currentTimeMillis();
                    totalUploadMillis += outMillis - inMillis;

                    TextureManager.clearCache();
                    TextureManager.doTextureCleanup();
                } catch (IOException ex) {
                    if (ex.getMessage().equals("Depth not supported")){
                        System.out.println("DEPTH MAP");
                    }
                }
            }
            }
            
            System.out.println("Read: "+totalReadMillis);
            System.out.println("Upload: "+totalUploadMillis);
    }
}