/**
 * 
 */
package com.gibbon.meshparser;

import java.io.File;
import java.io.FileInputStream;


import com.jme.app.SimpleGame;
import com.jme.scene.Node;
import com.jme.util.resource.ResourceLocatorTool;
import com.jme.util.resource.SimpleResourceLocator;

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

        // Verifies scenegraph.
//		if (model != null) {
//			System.out.print(model.getName());
//			System.out.println(" " + model.getClass());
//			model.getClass().cast(model);
////			System.out.println("\t" + model.getQuantity());
//			for (Spatial sp : model.getChildren()) {
//				Node no = (Node) sp;
//				System.out.print("\n\t" + no.getName());
//				System.out.println(" " + model.getClass());
////				System.out.println("\t\t" + no.getQuantity());
//				for (Spatial sp1 : no.getChildren()) {
//					//Node no1 = (Node) sp1;
//					System.out.print("\n\t\t" + sp1.getName());
//					System.out.println(" " + sp1.getClass());
//				}
//			}
////			System.out.println(model.getControllerCount());
//		}

        // Load Ogre XML mesh file and material
        String uris1[] = {
            "C:\\Users\\Kirill\\Desktop\\xemna\\",
            "C:\\Users\\Kirill\\Desktop\\xemna\\xemnaHead.material",
            "C:\\Users\\Kirill\\Desktop\\xemna\\xemnaHead.mesh.xml"
        };

        String uris3[] = {
            "D:\\Test Anim Import\\",
            "D:\\Test Anim Import\\Scene.material",
            "D:\\Test Anim Import\\Cube.012.mesh.xml"
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
            MaterialLoader matLoader = new MaterialLoader();
            matLoader.load(new FileInputStream(matRes));

            OgreLoader meshLoader = new OgreLoader();
            meshLoader.setMaterials(matLoader.getMaterials());

            Node model = (Node) meshLoader.loadModel(meshRes.toURI().toURL(), false);

            rootNode.attachChild(model);
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
