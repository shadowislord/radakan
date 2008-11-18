package com.gibbon.tools.world;

import com.jme.math.FastMath;
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

    private static int id = 0;
    private static Vector3f startDrag = new Vector3f();

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

    public static void pickRandomValues() {
        EditorState state = EditorState.getState();

        if (state.randomRotation){
            state.nextAngle = FastMath.nextRandomFloat() * FastMath.TWO_PI;
            System.out.println("ANGLE: "+state.nextAngle);
        }
        if (state.randomScale){
            EntityType type = state.entityType;
            state.nextScale = type.scaleStart + FastMath.nextRandomFloat() * (type.scaleEnd - type.scaleStart);
            System.out.println("SCALE: "+state.nextScale);
        }
    }

    public static void doSelectionAction(int x, int y, boolean drag){
        EditorState state = EditorState.getState();
        MouseEvent evt = EditorState.lastMouseEvent;

        Vector3f point = new Vector3f();
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

                // XXX: Should use triangle normal here
                float h = PickUtils.getTerrainHeight(model.getWorldTranslation(), null, 0);
                model.getLocalTranslation().y = h;

                model.updateGeometricState(0, true);
            }
        }else if (!drag){
            TriMesh collided = PickUtils.findClickedObject(x, y, false, point, null);

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
    }

    public static void doPlacementAction(int x, int y, boolean drag){
        EditorState state = EditorState.getState();
        MouseEvent evt = EditorState.lastMouseEvent;

        Vector3f point = new Vector3f();

        if (state.entityType == null)
            return;

        if (!drag){
            Vector3f normal = new Vector3f();
            TriMesh collided = PickUtils.findClickedObject(x, y, true, point, normal);

            if (collided == null)
                return;

            GameEntityManager entityMan = Game.getEntityManager();
            Entity entity = entityMan.produce(state.entityType.name, state.entityType.name + (id++));
            ModelUnit model = entity.getUnit(ModelUnit.class);

            Tile t = (Tile) collided.getParent();
            t.worldToLocal(point, point);
            model.getModel().setLocalTranslation(point);
            t.attachChild(model.getModel());
            //World.getWorld().attachChild(model.getModel());

            model.getModel().setUserData("UpVector", normal);
            if (state.randomRotation){
                model.getModel().getLocalRotation().fromAngleAxis(state.nextAngle, normal);
            }

            if (state.randomScale){
                model.getModel().setLocalScale(state.nextScale);
            }

            World.getWorld().update();

            EditorState.lastPlacedEntity = entity;

            pickRandomValues();
        }else if (EditorState.lastPlacedEntity != null){
            TriMesh collided = PickUtils.findClickedObject(x, y, true, point, null);

            if (collided == null){
                EditorState.lookAtTarget = null;
                return;
            }
            ModelUnit model = EditorState.lastPlacedEntity.getUnit(ModelUnit.class);
            Vector3f modelPos = model.getModel().getWorldTranslation();

            Vector3f normal = new Vector3f();
            // find normal at point
            PickUtils.getTerrainHeight(modelPos, normal, PickUtils.CRASH_IF_NAN
                                                       | PickUtils.NORMAL_FETCH
                                                       | PickUtils.TERRAIN_NORMAL);
            EditorState.lookAtTarget = point.clone();

            float angle = FastMath.atan2(modelPos.x - point.x, modelPos.z - point.z);

            Vector3f upVec = (Vector3f) model.getModel().getUserData("UpVector");
            model.getModel().getLocalRotation().fromAngleAxis(angle, upVec);

            //point.y = model.getLocalTranslation().y;
            //model.lookAt(point, normal);

        }
    }

    public static void doMouseAction(int x, int y, boolean drag, boolean finish){
        EditorState state = EditorState.getState();
        MouseEvent evt = EditorState.lastMouseEvent;

        if (finish){
            EditorState.lastPlacedEntity = null;
            EditorState.lookAtTarget = null;
            return;
        }

        if (state.selectionMode){
            doSelectionAction(x, y, drag);
        }else{
            doPlacementAction(x, y, drag);
        }

    }
    
}
