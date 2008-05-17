/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package com.gibbon.meshparser;

import org.w3c.dom.Node;

/**
 *
 * @author Kirill
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
    
    /**
     * Returns an attribute of the specified tag with the name provided.
     * 
     * @param node
     * @param name
     * @return
     */
    public static String getAttribute(Node node, String name){
        Node att = node.getAttributes().getNamedItem(name);
        return att == null ? null : att.getNodeValue();
    }
    
    public static float getFloatAttribute(Node node, String name){
        return Float.parseFloat(getAttribute(node,name));
    }
    
    public static int getIntAttribute(Node node, String name){
        return Integer.parseInt(getAttribute(node,name));
    }
    
}
