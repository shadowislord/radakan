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

import com.jme.scene.Spatial;
import com.jme.system.dummy.DummyDisplaySystem;
import com.jme.util.export.binary.BinaryExporter;
import com.jme.util.resource.ResourceLocatorTool;
import com.jme.util.resource.SimpleResourceLocator;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;


public class TestOgreImporter {

    public static void main(String[] args) throws IOException{
        String uris1[] = {
            "C:\\Users\\Kirill\\Desktop\\xemna\\",
            "C:\\Users\\Kirill\\Desktop\\xemna\\xemnaHead.material",
            "C:\\Users\\Kirill\\Desktop\\xemna\\xemnaHead.mesh.xml"
        };
        
        File root = new File(uris1[0]);
        File mat = new File(uris1[1]);
        File model = new File(uris1[2]);
        
        new DummyDisplaySystem();
        
        SimpleResourceLocator srl = new SimpleResourceLocator(root.toURI());
        ResourceLocatorTool.addResourceLocator(ResourceLocatorTool.TYPE_TEXTURE, srl);
        ResourceLocatorTool.addResourceLocator(ResourceLocatorTool.TYPE_MODEL, srl);
        
        MaterialLoader matLoader = new MaterialLoader();
        matLoader.load(new FileInputStream(mat));
        
        OgreLoader loader = new OgreLoader();
        loader.setMaterials(matLoader.getMaterials());
        Spatial spatial = loader.loadModel(model.toURI().toURL(), true);
        
        //SceneGraphDump.dump(spatial);
        
        BinaryExporter.getInstance().save(spatial, new File("D:\\ConverterModel.jme"));
    }
    
}
