package com.gibbon.tools.world;

import com.jme.math.Vector3f;
import com.jme.scene.Spatial;
import com.jme.scene.Spatial.CullHint;
import com.jme.scene.TriMesh;
import com.radakan.entity.Entity;
import com.radakan.entity.EntityFactory;
import com.radakan.entity.EntityFactory.EntityType;
import com.radakan.entity.unit.ModelUnit;
import java.awt.event.MouseEvent;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Collection;
import java.util.List;

public class EntityBrush {

    public static EntityFactory factory;
    private static Vector3f startDrag = new Vector3f();
    
    public static Collection<EntityType> loadEntityTypes() throws IOException{
        factory = new EntityFactory();
        
        File entitiesFile = new File(System.getProperty("user.dir")+"/entities/");
		if(!entitiesFile.exists()){
			entitiesFile.mkdir();
		}
        for (File f : entitiesFile.listFiles()){
            if (f.getName().endsWith(".xml")){
                InputStream in = new FileInputStream(f);
                factory.load(in);
                in.close();
            }
        }
        
        
        
        return factory.getLoadedTypes();
    }
    
    public static void doMouseAction(int x, int y, boolean drag, boolean finish){
        EditorState state = EditorState.getState();
        MouseEvent evt = EditorState.lastMouseEvent;
        
        if (finish)
            return;
        
        Vector3f point = new Vector3f();
        if (state.selectionMode){
            if (drag && !evt.isControlDown()){
                TriMesh collided = PickUtils.findClickedObject(x, y, true, point);
                
                if (collided == null)
                    return;
                
                Vector3f offset = new Vector3f(point);
                offset.subtractLocal(startDrag);
                startDrag.addLocal(offset);
                
                for (Entity ent : EditorState.getState().selection){
                    Spatial model = ent.getUnit(ModelUnit.class).getModel();
                    model.getLocalTranslation().addLocal(offset);
                    model.updateWorldVectors();
                    float h = PickUtils.getTerrainHeight(World.getWorld(), model.getWorldTranslation(), null);
                    model.getLocalTranslation().y = h;
                    
                    model.updateGeometricState(0, true);
                }
            }else if (!drag){
                TriMesh collided = PickUtils.findClickedObject(x, y, false, point);
                
                if (collided == null || collided.getName().startsWith("TERRAIN"))
                    return;

                Spatial model = collided;
                while (!model.getName().startsWith("ENTITY")){
                    model = model.getParent();
                }
                Entity ent = (Entity) model.getUserData("Entity");
                List<Entity> selection = state.selection;

                if (evt.isControlDown()){
                    if (selection.contains(ent)){
                        // deselect
                        selection.remove(ent);
                        model.setCullHint(CullHint.Dynamic);
                    }else{
                        // add select
                        model.setCullHint(CullHint.Always);
                        selection.add(ent);
                    }
                }else{
                    PickUtils.findClickedObject(x, y, true, point);
                    
                    // set selection
                    for (Entity e : selection){
                        e.getUnit(ModelUnit.class).getModel().setCullHint(CullHint.Dynamic);
                    }
                    selection.clear();
                    model.setCullHint(CullHint.Always);
                    selection.add(ent);
                    startDrag.set(point);
                }
            }
        }else if (!drag){
            TriMesh collided = PickUtils.findClickedObject(x, y, true, point);
            
            if (collided == null) return;

            Entity entity = factory.produce(state.entityType.name, "ENTITY");
            ModelUnit model = entity.getUnit(ModelUnit.class);

            Tile t = (Tile) collided.getParent();
            t.worldToLocal(point, point);
            model.getModel().setLocalTranslation(point);
            t.attachChild(model.getModel());
            //World.getWorld().attachChild(model.getModel());
            
            World.getWorld().update();
            
        }
    }
    
}
