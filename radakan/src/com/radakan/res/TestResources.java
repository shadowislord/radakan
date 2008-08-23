package com.radakan.res;

import com.jme.image.Texture;
import java.io.File;
import java.io.IOException;
import java.util.logging.Level;
import java.util.logging.Logger;

public class TestResources {

    public static void main(String[] args) throws IOException {
        try {
            //FileSystem fileSystem = new ZipFileSystem(new File(System.getProperty("user.dir"), "data.zip"));
            // ------------ SETTING UP RESOURCEMANAGER -------------
            IFileSystem fileSystem = new PakFileSystem(new File(System.getProperty("user.dir"), "data.pak"));
            ResourceManager.setFileSystem(fileSystem);

            ResourceManager.registerLoader(Texture.class, new TextureLoader());
            ResourceManager.registerLoader(String.class, new TextLoader());


            // ------------- THREADED LOADING TEST -----------------
            System.err.println("Trying threaded loading...");

            long nanos = System.nanoTime();
            ResourceManager.loadResourceLater(String.class, "ReadMe.txt");
            Thread.yield();
            while (!ResourceManager.isIdle()) {
                Thread.sleep(1);
            }
            String data = ResourceManager.aquireResource(String.class, "ReadMe.txt");
            float time = (System.nanoTime() - nanos) / 1000f;

            System.err.println("Threaded loading took " + time + " kilonanos");
            System.err.println("------------------------");

            // ------------ INSTANT LOADING TEST ------------------
            ResourceManager.removeFromCache("ReadMe.txt");
            data = null;

            System.err.println("Now trying instant loading..");
            nanos = System.nanoTime();
            data = ResourceManager.loadResource(String.class, "ReadMe.txt");
            time = (System.nanoTime() - nanos) / 1000f;

            System.err.println("Instant loading took " + time + " kilonanos");
            System.err.println("------------------------");


            // ----------- CACHE LOADING TEST ----------------------
            //data = null;
            System.err.println("Now trying loading from cache..");
            nanos = System.nanoTime();

            ResourceManager.loadResourceLater(String.class, "ReadMe.txt");
            while (!ResourceManager.isIdle()) {
                System.out.println("Doing work..");
                Thread.sleep(1);
            }
            data = ResourceManager.aquireResource(String.class, "ReadMe.txt");

            time = (System.nanoTime() - nanos) / 1000f;

            System.err.println("Cached loading took " + time + " kilonanos");
            System.err.println("------------------------");

            // ----------- AUTOMATIC RESOURCE DISPOSAL TEST -----
            data = null;
            System.err.println("Now trying resource garbage collection..");
            System.gc();
            Thread.sleep(1000);
            System.gc();
            System.err.println("Waited 1 second and invoked garbage collector");

            ResourceManager.removeExpiredFromCache();
            data = ResourceManager.aquireResource(String.class, "ReadMe.txt");
            if (data == null) {
                System.err.println("Resource has been garbage collected successfuly!");
            } else {
                System.err.println("Resource was not garbage collected :(");
            }
        } catch (InterruptedException ex) {
            Logger.getLogger(TestResources.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
}
