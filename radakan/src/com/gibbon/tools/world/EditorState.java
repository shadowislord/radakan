package com.gibbon.tools.world;

import com.gibbon.radakan.entity.Entity;
import com.gibbon.radakan.entity.EntityFactory.EntityType;
import com.jme.bounding.BoundingSphere;
import com.jme.util.export.InputCapsule;
import com.jme.util.export.JMEExporter;
import com.jme.util.export.JMEImporter;
import com.jme.util.export.OutputCapsule;
import com.jme.util.export.Savable;
import java.awt.event.MouseEvent;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class EditorState implements Savable {


    private static EditorState state = new EditorState();
    public static transient Map<String, TextureSet> texsetMap = new HashMap<String, TextureSet>();
    public static transient WorldCameraHandler handler;
    public static transient MouseEvent lastMouseEvent;
    
    EditType editType = EditType.TILE;

    // terrain + brush
    BrushType brushType = BrushType.RAISE;
    int brushSize = 5;
    float brushStrength = 1.0f;
    BoundingSphere sphere = new BoundingSphere();
    
    // texture
    int textureIndex = -1;
    TextureSet texSet = null;
    
    // entity
    boolean selectionMode = false;
    EntityType entityType;
    List<Entity> selection = new ArrayList<Entity>();
    
    public static EditorState getState(){
        return state;
    }
    
    public static void setState(EditorState newState){
        state = newState;
    }

    public void write(JMEExporter ex) throws IOException {
        OutputCapsule cap = ex.getCapsule(this);
        cap.write(editType, "EditType", EditType.TILE);
        cap.write(brushType, "BrushType", BrushType.RAISE);
        cap.write(brushSize, "BrushSize", 5);
        cap.write(brushStrength, "BrushStrength", 1.0f);
        cap.write(textureIndex, "TextureIndex", -1);
        if (texSet != null) cap.write(texSet.toString(), "TextureSet", null);
        cap.write(World.getWorld(), "World", null);
    }

    public void read(JMEImporter im) throws IOException {
        InputCapsule cap = im.getCapsule(this);
        editType = cap.readEnum("EditType", EditType.class, EditType.TILE);
        brushType = cap.readEnum("BrushType", BrushType.class, BrushType.RAISE);
        brushSize = cap.readInt("BrushSize", 5);
        brushStrength = cap.readFloat("BrushStrength", 1.0f);
        textureIndex = cap.readInt("TextureIndex", -1);
        String texSetName = cap.readString("TextureSet", null);
        texSet = texsetMap.get(texSetName);
        cap.readSavable("World", null); // <- automatically initializes world to be default
        // WorldTool frame automatically updates based on this data..
    }

    public Class getClassTag() {
        return EditorState.class;
    }
    
}
