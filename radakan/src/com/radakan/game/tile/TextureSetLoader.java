package com.radakan.game.tile;

import com.jme.image.Texture.MagnificationFilter;
import com.jme.image.Texture.MinificationFilter;
import com.jme.image.Texture2D;
import com.jme.util.TextureManager;
import com.radakan.game.tile.TextureSet.Detailmap;
import com.radakan.util.XMLUtil;
import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;
import org.w3c.dom.Node;

public class TextureSetLoader {

    public static TextureSet load(InputStream in, boolean useLightmap) throws IOException{
        Node n = XMLUtil.loadDocument(in, "textureset");
        if (n == null)
            return null;
        
        String tsetName = XMLUtil.getAttribute(n, "name");
        TextureSet set = new TextureSet(tsetName);
        List<Detailmap> detailmaps = new ArrayList<Detailmap>();
        
        Node textureNode = n.getFirstChild();
        while (textureNode != null){
            if (textureNode.getNodeName().equals("texture")){
                String name = XMLUtil.getAttribute(textureNode, "name");
                Detailmap map = new Detailmap(name);
                
                String diffuse = XMLUtil.getAttribute(textureNode, "diffuse");
                map.colormap = (Texture2D) TextureManager.loadTexture(diffuse, 
                                                                MinificationFilter.Trilinear, 
                                                                MagnificationFilter.Bilinear, 
                                                                0.0f, 
                                                                true);
                map.scale = XMLUtil.getFloatAttribute(textureNode, "scale");
                detailmaps.add(map);
            }
            
            textureNode = textureNode.getNextSibling();
        }
        
        set.setDetailmaps(detailmaps);
        set.generateShader(useLightmap);
        return set;
    }
    
}
