package com.radakan.game.tile;

import com.jme.image.Texture.MagnificationFilter;
import com.jme.image.Texture.MinificationFilter;
import com.jme.image.Texture2D;
import com.jme.scene.state.FogState;
import com.jme.util.TextureManager;
import com.radakan.game.tile.TextureSet.Detailmap;
import com.radakan.util.XMLUtil;
import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.logging.Logger;
import org.w3c.dom.Node;

import static com.radakan.util.XMLUtil.*;

public class TextureSetLoader {

    private static final Logger logger = Logger.getLogger(TextureSetLoader.class.getName());
    
    public static Map<String, TextureSet> load(InputStream in, boolean useLightmap, boolean useFog) throws IOException{
        Map<String, TextureSet> textureSets = new HashMap<String, TextureSet>();
        
        Node tsetListXMLNode = loadDocument(in, "texturesets");
        Node tsetXMLNode = tsetListXMLNode.getFirstChild();
        while (tsetXMLNode != null){
            if (tsetXMLNode.getNodeName().equals("textureset")){
                TextureSet set = load(tsetXMLNode, useLightmap, useFog);
                textureSets.put(set.toString(), set);
            }
            
            tsetXMLNode = tsetXMLNode.getNextSibling();
        }
        
        return textureSets;
    }
    
    public static TextureSet load(Node tsetXMLNode, boolean useLightmap, boolean useFog) throws IOException{
        TextureSet set = new TextureSet(getAttribute(tsetXMLNode, "name"));
        List<Detailmap> detailmaps = new ArrayList<Detailmap>();
        
        Node textureXMLNode = tsetXMLNode.getFirstChild();
        while (textureXMLNode != null){
            if (textureXMLNode.getNodeName().equals("texture")){
                String name = XMLUtil.getAttribute(textureXMLNode, "name");
                Detailmap map = new Detailmap(name);
                
                String diffuse = XMLUtil.getAttribute(textureXMLNode, "diffuse");
                map.colormap = (Texture2D) TextureManager.loadTexture(diffuse, 
                                                                MinificationFilter.Trilinear, 
                                                                MagnificationFilter.Bilinear, 
                                                                0.0f, 
                                                                true);
                map.scale = XMLUtil.getFloatAttribute(textureXMLNode, "scale");
                detailmaps.add(map);
            }
            
            textureXMLNode = textureXMLNode.getNextSibling();
        }
        
        set.setDetailmaps(detailmaps);
        set.generateShader(useLightmap, useFog);
        return set;
    }
    
}
