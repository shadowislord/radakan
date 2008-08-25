/*
 * Radakan is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Radakan is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Radakan.  If not, see <http://www.gnu.org/licenses/>.
 */

package com.radakan.util;

import com.jme.math.Quaternion;
import com.jme.math.Vector3f;
import com.jme.renderer.ColorRGBA;
import java.io.IOException;
import java.io.InputStream;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;

import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

/**
 * XML parsing utility methods
 */
public class XMLUtil {
    
    /**
     * Returns the first XML child tag with the specified name.
     * 
     * @param node The node to search children of
     * @param name The name of the node to search for, case-sensitive.
     * @return The child with the specified name, or null if none exists.
     */
    public static Node getChildNode(Node node, String name) {
        Node child = node.getFirstChild();
        while (child != null && !child.getNodeName().equals(name) ){
            child = child.getNextSibling();
        }
        return child;
    }
    
    public static Node loadDocument(InputStream in, String rootElementName) throws IOException{
        try {
            DocumentBuilder builder = DocumentBuilderFactory.newInstance().newDocumentBuilder();
            Document doc = builder.parse(in);

            NodeList list = doc.getElementsByTagName(rootElementName);
            if (list.getLength() == 0) {
                return null;
            }

            return list.item(0);
        } catch (ParserConfigurationException ex) {
            throw new IOException("Error occured while reading XML document: "+ex.getLocalizedMessage());
        } catch (SAXException ex) {
            throw new IOException("Error occured while reading XML document: "+ex.getLocalizedMessage());
        }
    }
    
    /**
     * Returns an attribute of the specified tag with the name provided.
     * 
     * @param node
     * @param name
     * @return The attribute if its defined, or null.
     */
    public static String getAttribute(Node node, String name, String defVal){
        Node att = node.getAttributes().getNamedItem(name);
        return att == null ? null : att.getNodeValue();
    }
    
    public static String getAttribute(Node node, String name){
        return getAttribute(node,name,null);
    }
    
    public static boolean getBoolAttribute(Node node, String name){
        return Boolean.parseBoolean(getAttribute(node,name));
    }
    
    public static boolean getBoolAttribute(Node node, String name, boolean defVal){
        String att = getAttribute(node, name);
        if (att == null) return defVal;
        return Boolean.parseBoolean(att);
    }
    
    public static float getFloatAttribute(Node node, String name){
        return Float.parseFloat(getAttribute(node,name,"0"));
    }
    
    public static float getFloatAttribute(Node node, String name, float defVal){
        String att = getAttribute(node, name);
        if (att == null) return defVal;
        return Float.parseFloat(att);
    }
    
    public static int getIntAttribute(Node node, String name, int defVal){
        String att = getAttribute(node, name);
        if (att == null) return defVal;
        return Integer.parseInt(att);
    }
    
    public static int getIntAttribute(Node node, String name){
        return Integer.parseInt(getAttribute(node,name));
    }
    
    public static float str2float(String str){
        return Float.parseFloat(str.trim());
    }
    
    public static Vector3f getVec3Attribute(Node node, String name){
        return getVec3Attribute(node, name, null);
    }
    
    public static Vector3f getVec3Attribute(Node node, String name, Vector3f defVal){
        String att = getAttribute(node, name);
        if (att == null)
            return defVal;
        
        String split[] = att.split(",");
        return new Vector3f(str2float(split[0]),
                            str2float(split[1]),
                            str2float(split[2]));
    }
    
    public static Quaternion getQuatAttribute(Node node, String name){
        return getQuatAttribute(node, name, null);
    }
    
    public static Quaternion getQuatAttribute(Node node, String name, Quaternion defVal){
        String att = getAttribute(node, name);
        if (att == null)
            return defVal;
        
        String split[] = att.split(",");
        return new Quaternion(str2float(split[0]),
                              str2float(split[1]),
                              str2float(split[2]),
                              str2float(split[3]));
    }
    
    public static ColorRGBA getRGBAAttribute(Node node, String name){
        return getRGBAAttribute(node, name, null);
    }
    
    public static ColorRGBA getRGBAAttribute(Node node, String name, ColorRGBA defVal){
        String att = getAttribute(node, name);
        if (att == null)
            return defVal;
        
        String split[] = att.split(",");
        return new ColorRGBA(str2float(split[0]),
                             str2float(split[1]),
                             str2float(split[2]),
                             str2float(split[3]));
    }
    
}
