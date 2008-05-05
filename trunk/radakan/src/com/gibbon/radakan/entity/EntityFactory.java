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

package com.gibbon.radakan.entity;

import com.gibbon.radakan.error.ErrorReporter;
import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.xml.sax.SAXException;

/**
 * Basic XML parser for types.xml files
 * 
 * @author Momoko_Fan
 */
public final class EntityFactory {

    private List<Node> typeXmlNodes = new ArrayList<Node>();
    
    public EntityFactory(InputStream xmlTypes){
        try {
            DocumentBuilder builder = DocumentBuilderFactory.newInstance().newDocumentBuilder();
            Document doc = builder.parse(xmlTypes);
            
            Node typesNode = doc.getElementsByTagName("Types").item(0);
            Node typeNode = typesNode.getFirstChild();
            while (typeNode != null){
                if (typeNode.getNodeName().equals("Type")){
                    
                }   
                
                typeNode = typeNode.getNextSibling();
            }
        } catch (SAXException ex) {
            ErrorReporter.reportError("Error while parsing XML file", ex);
        } catch (IOException ex) {
            ErrorReporter.reportError("IO Error while reading input stream", ex);
        } catch (ParserConfigurationException ex) {
            ErrorReporter.reportError("Parser error", ex);
        }
    }
    
}
