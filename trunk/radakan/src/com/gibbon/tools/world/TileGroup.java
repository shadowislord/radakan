package com.gibbon.tools.world;

import com.jme.image.Image;
import com.jme.image.Image.Format;
import com.jme.image.Texture2D;
import com.jme.scene.Node;
import com.jme.scene.state.GLSLShaderObjectsState;
import com.jme.scene.state.RenderState;
import com.jme.scene.state.lwjgl.LWJGLShaderObjectsState;
import com.jme.util.export.InputCapsule;
import com.jme.util.export.JMEExporter;
import com.jme.util.export.JMEImporter;
import com.jme.util.export.OutputCapsule;
import com.jme.util.export.Savable;
import com.jme.util.geom.BufferUtils;
import java.io.IOException;
import java.nio.ByteBuffer;

public class TileGroup extends Node implements Savable {

    private int x, y;
    private Texture2D[] alphamaps;
    private TextureSet texSet = null;
    
    public static final Format ALPHAMAP_FORMAT = Format.Alpha8;
    
    public static Image createPaintableImage(int width, int height){
        ByteBuffer data = BufferUtils.createByteBuffer(Image.getEstimatedByteSize(ALPHAMAP_FORMAT) * width * height);
        Image image = new Image(ALPHAMAP_FORMAT, width, height, data);
        
        return image;
    }
    
    public TileGroup(int x, int y){
        super("GROUP_"+x+"_"+y);
        this.x = x;
        this.y = y;
        
        setTextureCombineMode(TextureCombineMode.Replace);
    }
    
    public TileGroup(){
        setTextureCombineMode(TextureCombineMode.Replace);
    }
    
    @Override
    public void read(JMEImporter im) throws IOException {
        super.read(im);
        
        InputCapsule cap = im.getCapsule(this);
        x = cap.readInt("GroupX", 0);
        y = cap.readInt("GroupY", 0);
        texSet = EditorState.texsetMap.get(cap.readString("TextureSet", null));
        Object[] maps = cap.readSavableArray("Alphamaps", null);
        alphamaps = new Texture2D[maps.length];
        for (int i = 0; i < maps.length; i++){
            alphamaps[i] = (Texture2D) maps[i];
        }
        setTextureSet(texSet, false);
        
        setName("GROUP_"+x+"_"+y);
    }
    
    @Override
    public void write(JMEExporter ex) throws IOException {
        // set STORE TEXTURE before writing the NODE
        // otherwise image will NOT be stored
        for (Texture2D alphamap : alphamaps){
            alphamap.setStoreTexture(true);
        }
        
        super.write(ex);
        
        OutputCapsule cap = ex.getCapsule(this);
        cap.write(x, "GroupX", 0);
        cap.write(y, "GroupY", 0);
        cap.write(texSet.toString(), "TextureSet", null);
        cap.write(alphamaps, "Alphamaps", null);
    }
    
    public boolean isTextureSet(){
        return alphamaps != null;
    }
    
    public void setTextureSet(TextureSet set, boolean replaceAlphamaps){
        if (set == null)
            return;
        
        texSet = set;
        if (replaceAlphamaps){
            alphamaps = set.applyMaterial(this);
        }else{
            // get list of dummy alphamaps
            Texture2D[] tempMaps = set.applyMaterial(this);
            
            // replace with this tilegroup's alphamaps
            for (int i = 0; i < tempMaps.length; i++){
                tempMaps[i].setImage(alphamaps[i].getImage());
            }
            alphamaps = tempMaps;
        }
    }
    
    public Texture2D getPaintableAlphamap(int i){
        Texture2D alphamap = alphamaps[i];

        if (TextureSet.isNilImage(alphamap.getImage())){
            // replace with paintable data
            int resolution = World.getWorld().getGroupmapResolution();
            Image image = createPaintableImage(resolution, resolution);
            alphamap.setImage(image);
        }
        
        return alphamap;
    }
    
    public int getAlphamapCount(){
        return alphamaps.length;
    }
    
    public Tile findTile(int x, int y){
        return (Tile) getChild("TILE_"+x+"_"+y);
    }
    
    public int getX() {
        return x;
    }
    
    public int getY() {
        return y;
    }
    
}
