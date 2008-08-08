package com.gibbon.tools.world;

import com.gibbon.jme.context.JmeContext;
import com.gibbon.radakan.error.ErrorReporter;
import com.gibbon.tools.ModelLoader;
import com.jme.math.Vector3f;
import com.jme.scene.Spatial;
import com.jme.scene.TriMesh;
import com.jme.scene.state.TextureState;
import com.jme.util.TextureManager;
import java.awt.Cursor;
import java.io.File;
import java.io.IOException;
import java.net.MalformedURLException;
import javax.swing.JFileChooser;
import javax.swing.JFrame;

public class TileBrush {

    private static SkyPass skyPass = null;
    
    public static void doMouseAction(int x, int y, boolean drag, boolean finish){
        if (drag)
            return;
        
        Vector3f point = new Vector3f();
        TriMesh collided = PickUtils.findClickedObject(x, y, false, point, null);
        
        if (collided != null && collided.getName().startsWith("MARKER")){
            String[] split = collided.getName().split("_");
            int tileX = Integer.parseInt(split[1]);
            int tileY = Integer.parseInt(split[2]);

            World.getWorld().detachChild(collided);
            World.getWorld().createTile(tileX, tileY);
            World.getWorld().update();
        }
    }

    public static void previewLightmaps() {
        if (World.getWorld() != null){
            for (Spatial model : World.getWorld().getChildren()){
                if (model.getName().startsWith("GROUP")){
                    TileGroup group = (TileGroup) model;
//                    group.applyLightmap();
                }
            }
        }
    }

    public static void selectSkyTexture(JFrame frame) {
        //FileNameExtensionFilter filter = new FileNameExtensionFilter("Radakan World (*.world)", "world" );
        JFileChooser selector = new JFileChooser();
        
        selector.resetChoosableFileFilters();
        //chooser.addChoosableFileFilter(filter);
        try{
            if (selector.showOpenDialog(frame) == JFileChooser.APPROVE_OPTION){
                File f = selector.getSelectedFile();

                frame.setCursor(Cursor.getPredefinedCursor(Cursor.WAIT_CURSOR));

                Spatial sky = null;
                if (skyPass != null){
                    sky = skyPass.getSky();
                }else{
                    skyPass = new SkyPass();
                    sky = ModelLoader.loadMeshModel(new File(System.getProperty("user.dir"),
                                                    "/models/Skydome.mesh.xml"), false);
                    if (sky == null)
                        return;
                    
                    skyPass.setSky(sky);
                    JmeContext.get().getPassManager().add(skyPass);
                }

                TextureState ts = JmeContext.get().getRenderer().createTextureState();
                ts.setTexture(TextureManager.loadTexture(f.toURI().toURL(), true));
                sky.setRenderState(ts);
                sky.updateRenderState();

                frame.setCursor(Cursor.getDefaultCursor());
            }
        } catch (MalformedURLException ex){
            ErrorReporter.reportError("Invalid URL", ex);
        } catch (IOException ex){
            ErrorReporter.reportError("Error while reading mesh file", ex);
        } finally {
            frame.setCursor(Cursor.getDefaultCursor());
        }
    }
    
}
