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

package com.test.graphics.mesh.parser;

import java.io.File;
import java.io.FileInputStream;


import com.jme.app.SimpleGame;
import com.jme.scene.Node;
import com.jme.scene.Spatial;
import com.jme.scene.shape.Quad;
import com.jme.util.resource.ResourceLocatorTool;
import com.jme.util.resource.SimpleResourceLocator;
import com.radakan.graphics.mesh.parser.SceneLoader;

/**
 * @author Marco Frisan
 *
 */
public class TestLoadJMEModel extends SimpleGame {

    @Override
    protected void simpleInitGame() {
        // Disable lights. Use only diffuse ambient light.
//		lightState.setEnabled(false);

        // Loads the model.
//		String uris[] = {
//				"/Volumes/DocumentiD2/Ender/Documents/Art/Test/Blender/test_simple_bone_to_ogre/Cube.jme",
//				"/Users/ender/Desktop/TempDownloads/Ogre3D/ms3d2xml/media/model.jme"
//		};
//		Node model = null;
//		try {
//			model = (Node) BinaryImporter.getInstance().load(new File(uris[1]));
////			model.setModelBound(new BoundingSphere());
////            model.updateModelBound();
//            
////            MaterialState ms = display.getRenderer().createMaterialState();
////            ms.setAmbient(ColorRGBA.gray);
////            ms.setDiffuse(ColorRGBA.blue);
////            model.setRenderState(ms);
////            model.updateRenderState();
//            
//			rootNode.attachChild(model);
//		} catch (IOException ioe) {
//			System.out.println(ioe.getMessage());
//		}

        // Load Ogre XML mesh file and material
        String uris1[] = {
            "C:\\Users\\Kirill\\Desktop\\xemna\\",
            "C:\\Users\\Kirill\\Desktop\\xemna\\xemnaHead.material",
            "C:\\Users\\Kirill\\Desktop\\xemna\\xemnaHead.mesh.xml"
        };

        String uris3[] = {
            "E:\\RADAKAN\\data\\tiles\\",
            "E:\\RADAKAN\\data\\tiles\\area.material",
            "E:\\RADAKAN\\data\\tiles\\myscene.scene"
        };

        File root = new File(uris3[0]);
        File matRes = new File(uris3[1]);
        File meshRes = new File(uris3[2]);
        
        SimpleResourceLocator srl = new SimpleResourceLocator(root.toURI());
        ResourceLocatorTool.addResourceLocator(
                ResourceLocatorTool.TYPE_TEXTURE, srl);
        ResourceLocatorTool.addResourceLocator(ResourceLocatorTool.TYPE_MODEL,
                srl);

        try {
//            MaterialLoader matLoader = new MaterialLoader();
//            matLoader.load(new FileInputStream(matRes));
//
//            OgreLoader meshLoader = new OgreLoader();
//            meshLoader.setMaterials(matLoader.getMaterials());
//
//            Node model = (Node) meshLoader.loadModel(meshRes.toURI().toURL(), false);

            SceneLoader loader = new SceneLoader();
            loader.load(new FileInputStream(meshRes));
            
            rootNode.attachChild(loader.getScene());
            
            rootNode.updateGeometricState(0, true);
            rootNode.updateRenderState();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void main(String[] args) {
        TestLoadJMEModel app = new TestLoadJMEModel();
        app.setConfigShowMode(ConfigShowMode.ShowIfNoConfig);
        app.start();
    }
}
