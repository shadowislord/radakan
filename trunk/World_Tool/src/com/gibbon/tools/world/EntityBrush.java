package com.gibbon.tools.world;

import com.jme.math.Vector3f;
import com.jme.scene.Spatial;
import com.jme.scene.Spatial.CullHint;
import com.jme.scene.TriMesh;
import com.radakan.game.Game;
import com.radakan.game.entity.Entity;
import com.radakan.game.entity.GameEntityManager;
import com.radakan.game.entity.GameEntityManager.EntityType;
import com.radakan.game.entity.unit.ModelUnit;

import java.awt.event.MouseEvent;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Collection;
import java.util.List;
import java.util.logging.Logger;

public class EntityBrush {
	private static final Logger logger = Logger.getLogger(EntityBrush.class.getName());

    private static Vector3f startDrag = new Vector3f();
    private static int id = 0;
    
    public static Collection<EntityType> loadEntityTypes() throws IOException{
        GameEntityManager entityMan = Game.getEntityManager();
    	
    	File entitiesFile = new File(System.getProperty("user.dir")+"/entities/");
		if(!entitiesFile.exists()){
			entitiesFile.mkdir();
		}
        for (File f : entitiesFile.listFiles()){
            if (f.getName().endsWith(".xml")){
                InputStream in = new FileInputStream(f);
                entityMan.loadEntityTypes(in);
                in.close();
            }
        }
        
        return entityMan.getLoadedTypes();
    }
    
    public static void doMouseAction(int x, int y, boolean drag, boolean finish){
        EditorState state = EditorState.getState();
        MouseEvent evt = EditorState.lastMouseEvent;
        
        if (finish)
            return;
        
        Vector3f point = new Vector3f();
        if (state.selectionMode){
            if (drag && !evt.isControlDown()){
                TriMesh collided = PickUtils.findClickedObject(x, y, true, point, null);
                
                if (collided == null)
                    return;
                
                Vector3f offset = new Vector3f(point);
                offset.subtractLocal(startDrag);
                startDrag.addLocal(offset);
                
                for (Entity ent : EditorState.getState().selection){
                    Spatial model = ent.getUnit(ModelUnit.class).getModel();
                    model.getLocalTranslation().addLocal(offset);
                    model.updateWorldVectors();
                    float h = PickUtils.getTerrainHeight(model.getWorldTranslation(), null, 
                            PickUtils.NORMAL_FETCH |
                            PickUtils.CRASH_IF_NAN |
                            PickUtils.TERRAIN_NORMAL);
                    model.getLocalTranslation().y = h;
                    
                    model.updateGeometricState(0, true);
                }
            }else if (!drag){
                TriMesh collided = PickUtils.findClickedObject(x, y, false, point, null);
                
                if (collided == null || collided.getName().startsWith("TERRAIN"))
                    return;

                Spatial model = collided;
                
                // Changed logic since the naming structure has changed - geerzo
                while (model != null && model.getUserData("Entity") == null){
                //while (model != null && !model.getName().startsWith("ENTITY")){
                    model = model.getParent();
                }
                if (model == null)
                	return;
                
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
                    PickUtils.findClickedObject(x, y, true, point, null);
                    
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
            TriMesh collided = PickUtils.findClickedObject(x, y, true, point, null);
            
            if (collided == null)
                return;
            
            // If the entityType is null there is nothing to do.
            if (state.entityType == null)
                return;

            GameEntityManager entityMan = Game.getEntityManager();
            Entity entity = entityMan.produce(state.entityType.name, state.entityType.name + (id++) );
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
