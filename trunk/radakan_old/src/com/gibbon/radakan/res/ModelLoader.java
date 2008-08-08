package com.gibbon.radakan.res;

import com.jme.scene.Spatial;
import com.jme.util.export.Savable;
import com.jme.util.export.binary.BinaryImporter;
import java.io.IOException;
import java.io.InputStream;

public class ModelLoader implements ResourceLoader<Spatial> {

    public Spatial load(String resourceName) throws IOException {
        InputStream in = ResourceManager.getFileSystem().openStream(AccessMode.PARSE, resourceName);
        Savable savable = BinaryImporter.getInstance().load(in);
        if (!(savable instanceof Spatial))
            throw new IOException(resourceName+" does not specifiy a model file");
        
        return (Spatial) savable;
    }

}
