package com.radakan.res;

import com.jme.image.Image;
import com.jme.image.Texture;
import com.jme.image.Texture.MagnificationFilter;
import com.jme.image.Texture.MinificationFilter;
import com.jme.image.Texture2D;
import com.jme.image.Texture3D;
import com.jme.image.TextureCubeMap;
import com.jme.util.TextureManager;
import java.io.IOException;
import java.io.InputStream;

public class TextureLoader implements IResourceLoader<Texture> {

    private static final MinificationFilter defaultMinFilter = MinificationFilter.Trilinear;
    private static final MagnificationFilter defaultMagFilter = MagnificationFilter.Bilinear;
    private static final boolean defaultFlip = true;
    
    public Texture load(String resourceName) throws IOException {
        // determine extension
        int index = resourceName.lastIndexOf('.');
        if (index > 0){
            String ext = resourceName.substring(index);
            
            InputStream in = ResourceManager.getFileSystem().openStream(AccessMode.PARSE, resourceName);
            Image img = TextureManager.loadImage(ext, in, defaultFlip);
            in.close();
            
            Texture tex;
            if (img.getData().size() == 6){
                tex = new TextureCubeMap();
            }else if (img.getData().size() == 1){
                tex = new Texture2D();
            }else{
                tex = new Texture3D();
            }
            tex.setImage(img);
            tex.setMagnificationFilter(defaultMagFilter);
            tex.setMinificationFilter(defaultMinFilter);
            tex.setImageLocation(resourceName);
            
            return tex;
        }
        
        throw new IOException("Cannot determine extension of image "+resourceName);
    }

    public boolean useCache() {
        return true;
    }
}
