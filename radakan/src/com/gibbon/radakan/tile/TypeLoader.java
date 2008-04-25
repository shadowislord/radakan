package com.gibbon.radakan.tile;

import com.gibbon.radakan.error.ErrorReporter;
import com.jme.math.Vector3f;
import java.io.IOException;
import java.io.InputStream;
import java.util.HashMap;
import java.util.Map;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;


public class TypeLoader {

    private Map<String, ItemType> typeMap = new HashMap<String, ItemType>();
    
    public Map<String, ItemType> getTypeMap(){
        return typeMap;
    }
    
    private Node getChildNode(Node node, String name) {
        Node child = node.getFirstChild();
        while (child != null && !child.getNodeName().equals(name) ){
            child = child.getNextSibling();
        }
        return child;
    }
    
    private String getAttribute(Node node, String name){
        Node att = node.getAttributes().getNamedItem(name);
        return att == null ? null : att.getNodeValue();
    }
    
    public void readType(Node type){
        ItemType it = new ItemType();
        it.type = getAttribute(type, "type");
        String solid = getAttribute(type, "solid");
        if (solid != null && solid.equals("true"))
            it.solid = true;
        else
            it.solid = false;
     
        Node mesh = getChildNode(type, "mesh");
        it.meshName = getAttribute(mesh, "file_name");
        
        typeMap.put(getAttribute(type, "name"), it);
    }
    
    public void load(InputStream in){
        try {
            DocumentBuilder builder = DocumentBuilderFactory.newInstance().newDocumentBuilder();
            Document doc = builder.parse(in);
            
            NodeList list = doc.getElementsByTagName("Prototypes");
            if (list.getLength() == 0){
                return;
            }
            
            Node type = list.item(0).getFirstChild();
            while (type != null){
                if (type.getNodeName().equals("item_prototype")){
                    readType(type);
                }
                
                type = type.getNextSibling();
            }
        } catch (ParserConfigurationException ex) {
            ErrorReporter.reportError("XML reading error", ex);
        } catch (SAXException ex) {
            ErrorReporter.reportError("XML reading error", ex);
        } catch (IOException ex) {
            ErrorReporter.reportError("IO Error while reading tile", ex);
        }
    }
    
}
