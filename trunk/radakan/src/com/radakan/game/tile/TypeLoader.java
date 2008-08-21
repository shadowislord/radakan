/*
 * Radakan RPG is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Radakan RPG is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Radakan RPG.  If not, see <http://www.gnu.org/licenses/>.
 */

package com.radakan.game.tile;

import com.radakan.util.ErrorHandler;
import java.io.IOException;
import java.io.InputStream;
import java.util.HashMap;
import java.util.Map;
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
            ErrorHandler.reportError("XML reading error", ex);
        } catch (SAXException ex) {
            ErrorHandler.reportError("XML reading error", ex);
        } catch (IOException ex) {
            ErrorHandler.reportError("IO Error while reading tile", ex);
        }
    }
    
}