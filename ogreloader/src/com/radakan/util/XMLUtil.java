/*
 * Copyright (c) 2008, OgreLoader
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     - Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     - Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     - Neither the name of the Gibbon Entertainment nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY 'Gibbon Entertainment' "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL 'Gibbon Entertainment' BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
import org.w3c.dom.Element;
import org.xml.sax.SAXException;

import com.radakan.jme.mxml.OgreXmlFormatException;

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
        while (child != null) {
            if (child.getNodeName().equals(name)) {
                return child;
            }
            child = child.getNextSibling();
        }
        return null;
    }
    
    /**
     * @throws OgreXmlFormatException for high-level Ogre Format violations.
     * @throws IOException for any other parsing or I/O problems.
     */
    public static Node loadDocument(InputStream in, String rootElementName) throws IOException{
        try {
            DocumentBuilder builder = DocumentBuilderFactory.newInstance().newDocumentBuilder();
            Document doc = builder.parse(in);
            Element rootEl = doc.getDocumentElement();
            if (rootEl == null) {
                throw new OgreXmlFormatException(
                        "No root node in XML document, when trying to read '"
                        + rootElementName + "'");
            }
            if (rootEl.getTagName().equals(rootElementName)) {
                return rootEl;
            }
            throw new OgreXmlFormatException(
                    "Input XML file does not have required root element '"
                    + rootElementName + "'");
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
        return att == null ? defVal : att.getNodeValue();
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
        
        if (att.startsWith("#")){
            // parse HEX color
            att = att.substring(1);
            
            int rgb = Integer.parseInt(att, 16);
            if (att.length() == 6)
                rgb = (rgb << 8) | 0xFF;

            ColorRGBA color = new ColorRGBA();
            color.fromIntRGBA(rgb);
            return color;
        }else{
            String split[] = att.split(",");
            if (split.length == 3){
                return new ColorRGBA(str2float(split[0]),
                                     str2float(split[1]),
                                     str2float(split[2]),
                                     1.0f);
            }else{
                return new ColorRGBA(str2float(split[0]),
                                     str2float(split[1]),
                                     str2float(split[2]),
                                     str2float(split[3]));
            }
        }
    }
    
}
