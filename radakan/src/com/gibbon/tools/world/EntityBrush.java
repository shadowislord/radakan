package com.gibbon.tools.world;

import com.gibbon.jme.context.JmeContext;
import com.gibbon.radakan.entity.Entity;
import com.gibbon.radakan.entity.EntityFactory;
import com.gibbon.radakan.entity.EntityFactory.EntityType;
import com.gibbon.radakan.entity.unit.ModelUnit;
import com.jme.math.Vector3f;
import com.jme.scene.SharedNode;
import com.jme.scene.Spatial;
import com.jme.scene.Spatial.LightCombineMode;
import com.jme.scene.Spatial.TextureCombineMode;
import com.jme.scene.TriMesh;
import com.jme.util.TextureManager;
import java.awt.event.MouseEvent;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Collection;
import java.util.List;

public class EntityBrush {

    private static EntityFactory factory;
    
    public static Collection<EntityType> loadEntityTypes() throws IOException{
        factory = new EntityFactory();
        
        File entitiesFile = new File(System.getProperty("user.dir")+"/entities/");
        for (File f : entitiesFile.listFiles()){
            if (f.getName().endsWith(".xml")){
                InputStream in = new FileInputStream(f);
                factory.load(in);
                in.close();
            }
        }
        
        
        
        return factory.getLoadedTypes();
    }
    
    public static void doMouseAction(TriMesh collided, Vector3f point, boolean drag){
        EditorState state = EditorState.getState();
        
        if (state.selectionMode && !drag){
            MouseEvent evt = EditorState.lastMouseEvent;
            
            if (collided.getName().startsWith("TERRAIN"))
                return;
            
            Spatial model = collided;
            while (!(model instanceof SharedNode)){
                model = model.getParent();
            }
            Entity ent = (Entity) model.getUserData("Entity");
            List<Entity> selection = state.selection;
            
            if (evt.isControlDown()){
                if (selection.contains(ent)){
                    // deselect
                    selection.remove(ent);
                }else{
                    // add select
                    selection.add(ent);
                }
            }else{
                // set selection
                selection.clear();
                selection.add(ent);
            }
        }else{
            if (!collided.getName().startsWith("TERRAIN"))
                return;

            Entity entity = factory.produce(state.entityType.name, "ENTITY");
            ModelUnit model = entity.getUnit(ModelUnit.class);

            //model.getModel().setLightCombineMode(LightCombineMode.CombineClosest);
            //model.getModel().setTextureCombineMode(TextureCombineMode.Replace);

            Tile t = (Tile) collided.getParent();
            t.worldToLocal(point, point);
            model.getModel().setLocalTranslation(point);
            t.attachChild(model.getModel());

            World.getWorld().update();
        }
    }
    
}
