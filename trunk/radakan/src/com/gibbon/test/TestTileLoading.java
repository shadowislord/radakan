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

package com.gibbon.test;

import com.gibbon.jme.context.JmeContext;
import com.gibbon.jme.context.RenderPass;
import com.gibbon.jme.context.lwjgl.LWJGLContext;
import com.gibbon.meshparser.MaterialLoader;
import com.gibbon.radakan.config.ConfigFrame;
import com.gibbon.radakan.entity.*;
import com.gibbon.radakan.tile.TileLoader;
import com.gibbon.radakan.tile.TypeLoader;
import com.jme.light.PointLight;
import com.jme.math.Vector3f;
import com.jme.renderer.ColorRGBA;
import com.jme.scene.Node;
import com.jme.scene.Spatial;
import com.jme.scene.state.CullState;
import com.jme.scene.state.CullState.Face;
import com.jme.scene.state.LightState;
import com.jme.scene.state.ZBufferState;
import com.jme.scene.state.ZBufferState.TestFunction;
import com.jme.system.GameSettings;
import com.jme.system.PreferencesGameSettings;

import com.jme.util.resource.ResourceLocatorTool;
import com.jme.util.resource.SimpleResourceLocator;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.util.prefs.Preferences;

public class TestTileLoading {

    private static Spatial createObjects(JmeContext cx) {
        try {
            SimpleResourceLocator srl = new SimpleResourceLocator(new File("D:\\TileStore2\\").toURI());
            SimpleResourceLocator srl2 = new SimpleResourceLocator(new File("D:\\TileStore\\").toURI());
            
            ResourceLocatorTool.addResourceLocator(ResourceLocatorTool.TYPE_MODEL, srl);
            ResourceLocatorTool.addResourceLocator(ResourceLocatorTool.TYPE_TEXTURE, srl2);
            ResourceLocatorTool.addResourceLocator("tile", srl);
            
            TypeLoader tloader = new TypeLoader();
            tloader.load(new FileInputStream("D:\\TileStore2\\world4_types.xml"));

            MaterialLoader mloader = new MaterialLoader();
            mloader.load(new FileInputStream("D:\\TileStore2\\world4.material"));
            
            TileLoader loader = new TileLoader(false);
            loader.setMaterials(mloader.getMaterials());
            loader.setTypes(Entity2.Type.map);

            Node root = new Node("root");
            
            Spatial t00 = loader.loadTile(0, 0);
            Spatial t10 = loader.loadTile(-1, 0);
            Spatial t01 = loader.loadTile(0, -1);
            Spatial t11 = loader.loadTile(-1, -1);

            
            t00.setLocalTranslation(0, 0, 0);
            t10.setLocalTranslation(-64, 0, 0);
            t01.setLocalTranslation(0, -64, 0);
            t11.setLocalTranslation(-64, -64, 0);
            
            root.attachChild(t00);
            root.attachChild(t10);
            root.attachChild(t01);
            root.attachChild(t11);
            
            return root;
        } catch (FileNotFoundException ex) {
            ex.printStackTrace();
        }
        
        return null;
    }
    
    public static Spatial initRoot(){
        Node rootNode = new Node("rootNode");
        
        CullState cull = JmeContext.get().getRenderer().createCullState();
        cull.setCullFace(Face.Back);
        rootNode.setRenderState(cull);

        ZBufferState zbuf = JmeContext.get().getRenderer().createZBufferState();
        zbuf.setFunction(TestFunction.LessThanOrEqualTo);
        zbuf.setWritable(true);
        rootNode.setRenderState(zbuf);
        
        LightState lightState = JmeContext.get().getRenderer().createLightState();
        rootNode.setRenderState(lightState);
        
        //Setup lights
        PointLight light = new PointLight();
        light.setDiffuse(new ColorRGBA(1.0f, 1.0f, 9.0f, 1.0f));
        light.setAmbient(new ColorRGBA(0.5f, 0.5f, 0.5f, 1.0f));
        light.setLocation(new Vector3f(100, 100, 50));
        light.setEnabled(true);
        lightState.attach(light);

        //Add dummy objects to rootNode
        rootNode.attachChild(createObjects(JmeContext.get()));
		
        rootNode.updateGeometricState(0, true);
        rootNode.updateRenderState();
        
        return rootNode;
    }
    
    public static void main(String[] args) throws InterruptedException, InstantiationException{
        Logger.getLogger("").setLevel(Level.WARNING);
        
        GameSettings settings
                = new PreferencesGameSettings(Preferences.userRoot().node("test-jmecontext"));
        
        settings.set("title", "Test JmeContext -  Delete/PgDn for eye distance, Home/End for focal distance");
        
        ConfigFrame g = new ConfigFrame(settings);
        g.setVisible(true);
        g.waitFor();
        
        JmeContext cx = JmeContext.create(LWJGLContext.class, JmeContext.CONTEXT_WINDOW);
        cx.setSettings(settings);
        cx.start();
        cx.waitFor();
        
        cx.getPassManager().loadDefaultPasses();
        
        final RenderPass rp = new RenderPass();
        rp.add(initRoot());
        //final StereoRenderPass rp = new StereoRenderPass();
        //rp.setMode(StereoRenderPass.StereoMode.ANAGLYPH);
        //rp.setProjection(StereoRenderPass.ProjectionMode.SIMPLE_OFFSET);
        //rp.setEyeDistance(3.0f);
        
        cx.getPassManager().add(rp);
        
//        UpdatePass up = new UpdatePass(){
//            @Override
//            public void runPass(JmeContext cx){
//                super.runPass(cx);
//                float tpf = cx.getPassManager().getTPF();
//                
//                if (KeyInput.get().isKeyDown(KeyInput.KEY_DELETE)){
//                    rp.setEyeDistance(rp.getEyeDistance() + 0.1f);
//                }else if (KeyInput.get().isKeyDown(KeyInput.KEY_PGDN)){
//                    rp.setEyeDistance(rp.getEyeDistance() - 0.1f);
//                }
//                if (KeyInput.get().isKeyDown(KeyInput.KEY_HOME)){
//                    rp.setFocusDistance(rp.getFocusDistance() + 1);
//                }else if (KeyInput.get().isKeyDown(KeyInput.KEY_END)){
//                    rp.setFocusDistance(rp.getFocusDistance() - 1);
//                }
//            }
//        };
//        
//        cx.getPassManager().add(up);
    }
    
}
