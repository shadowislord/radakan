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

package com.gibbon.meshparser;

import org.w3c.dom.Node;

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
     * @return The attribute if its defined, or null.
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
