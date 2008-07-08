package com.gibbon.tools.world;

import com.acarter.scenemonitor.SceneMonitor;
import com.gibbon.jme.context.JmeContext;
import com.jme.bounding.BoundingBox;
import com.jme.image.Texture;
import com.jme.light.DirectionalLight;
import com.jme.light.PointLight;
import com.jme.light.SpotLight;
import com.jme.math.Vector3f;
import com.jme.renderer.Camera;
import com.jme.renderer.ColorRGBA;
import com.jme.scene.Node;
import com.jme.scene.Spatial;
import com.jme.scene.TriMesh;
import com.jme.scene.shape.Box;
import com.jme.scene.state.CullState;
import com.jme.scene.state.CullState.Face;
import com.jme.scene.state.LightState;
import com.jme.scene.state.ZBufferState;
import com.jme.scene.state.ZBufferState.TestFunction;
import com.jme.util.export.InputCapsule;
import com.jme.util.export.JMEExporter;
import com.jme.util.export.JMEImporter;
import com.jme.util.export.OutputCapsule;
import java.io.IOException;
import java.util.ArrayList;

public class World extends Node {

    public static final int TILE_SIZE = 64;
    private static World instance;
    
    private TextureSet textureSet;
    private int lightmapRes = 256;
    private float tileScale;
    private int groupSize = 3;
    private int gridRes = 16;
    private transient boolean markers = true;
    private transient PointLight camLight;
    
    public World(int lightmapRes, int groupSize, int gridRes){
        this.lightmapRes = lightmapRes;
        this.groupSize = groupSize;
        this.gridRes = gridRes;
        
        loadRenderStates();

        updateFromState();
        update();
        
        instance = this;
    }
    
    public World(){
        loadRenderStates();
    }
    
    private void loadRenderStates(){
        JmeContext context = JmeContext.get();
        
        // set some default states for the root node
        ZBufferState zbuf = context.getRenderer().createZBufferState();
        zbuf.setFunction(TestFunction.LessThan);
        zbuf.setWritable(true);
        setRenderState(zbuf);

        CullState backFaces = context.getRenderer().createCullState();
        backFaces.setCullFace(Face.Back);
        setRenderState(backFaces);

        LightState ls = context.getRenderer().createLightState();
        ls.setSeparateSpecular(true);

        DirectionalLight dl = new DirectionalLight();
        dl.setEnabled(true);
        dl.setDirection(new Vector3f(0,0,-1).normalizeLocal());
        dl.setDiffuse(new ColorRGBA(0.6f, 0.6f, 0.3f, 1.0f));
        ls.attach(dl);

        DirectionalLight dl2 = new DirectionalLight();
        dl2.setEnabled(true);
        dl.setDirection(new Vector3f(1,0.1f,0).normalizeLocal());
        dl.setDiffuse(new ColorRGBA(0.4f, 0.4f, 0.8f, 1.0f));
        ls.attach(dl2);

        camLight = new PointLight();
        camLight.setEnabled(true);
        camLight.setDiffuse(ColorRGBA.lightGray);
        camLight.setSpecular(ColorRGBA.white);
        
        ls.attach(camLight);
        setRenderState(ls);
        
//        SceneMonitor.getMonitor().showViewer(true);
//        SceneMonitor.getMonitor().registerNode(this);
    }
    
    public static World getWorld(){
        return instance;
    }
    
    public int getGridResolution(){
        return gridRes;
    }
    
    public int getGroupSize(){
        return groupSize;
    }
    
    public int getTileSize() {
        return TILE_SIZE;
    }
    
    public int getGroupmapResolution(){
        return lightmapRes;
    }
    
    @Override
    public void updateWorldData(float tpf){
        super.updateWorldData(tpf);
        
        //SceneMonitor.getMonitor().updateViewer(tpf);
        Camera cam = JmeContext.get().getRenderer().getCamera();

        Vector3f temp = camLight.getLocation();
        temp.set(cam.getLeft()).multLocal(5.0f);
        temp.addLocal(cam.getLocation());
        camLight.setLocation(temp);
    }
    
    public void update(){
        updateGeometricState(0, true);
        updateRenderState();
    }
    
    public void showMarkers(boolean show){
        if (getChildren() != null){
            for (Spatial s: getChildren()){
                if (s.getName().startsWith("MARKER")){
                    s.setCullHint(show ? CullHint.Never : CullHint.Always);
                    s.setIsCollidable(show);
                }
            }
        }
        
        markers = show;
    }
    
    public Box createMarker(int x, int y){
        if (findTile(x,y) != null || findMarker(x,y) != null)
            return null;
        
        Box b = new Box("MARKER_"+x+"_"+y, 
                        new Vector3f(x * -TILE_SIZE - TILE_SIZE * 0.5f, 
                                     0.0f,
                                     y * -TILE_SIZE - TILE_SIZE * 0.5f), 
                        3, 0.5f, 3);
        b.setDefaultColor(ColorRGBA.red);
        b.setColorBuffer(null);
        
        b.setCullHint(markers ? CullHint.Never : CullHint.Always);
        b.setIsCollidable(markers);
        b.setLightCombineMode(LightCombineMode.Off);
        
        b.setModelBound(new BoundingBox());
        b.updateModelBound();
        
        attachChild(b);
        
        return b;
    }
    
    public boolean detachModel(Node model){
        Tile t = (Tile) model.getParent();
        if (t == null)
            return false;
        
        return t.detachChild(model) != -1;
    }
    
    public void createTile(int x, int y){
        Tile t = new Tile(x,y);
        t.setLocalTranslation(x * -TILE_SIZE, 0.0f, y * -TILE_SIZE);
        
        int groupX = (int) Math.floor((float)x / groupSize);
        int groupY = (int) Math.floor((float)y / groupSize);
        
        TileGroup group = findGroup(groupX, groupY);
        if (group == null)
            group = createGroup(groupX, groupY);
        
        group.attachChild(t);
        
        TriMesh grid = TerrainUtil.createGrid(gridRes, gridRes, 
                                              new Vector3f(-tileScale, 1f, -tileScale), 
                                              t);
        grid.setName("TERRAIN_"+x+"_"+y);
        t.setTerrain(grid);
        
        grid.setModelBound(new BoundingBox());
        grid.updateModelBound();
        
        grid.updateGeometricState(0, true);
        
        createMarker(x-1, y);
        createMarker(x+1, y);
        createMarker(x, y+1);
        createMarker(x, y-1);
    }
    
    public TileGroup createGroup(int x, int y){
        TileGroup group = new TileGroup(x,y);
        group.setTextureSet(textureSet, true);
        
        group.setModelBound(new BoundingBox());
        group.updateModelBound();
        
        attachChild(group);
        
        return group;
    }
    
    public Box findMarker(int x, int y){
        return (Box) getChild("MARKER_"+x+"_"+y);
    }
    
    public Tile findTile(int x, int y){
        int groupX = (int) Math.floor((float)x / groupSize);
        int groupY = (int) Math.floor((float)y / groupSize);
        Node group = findGroup(groupX, groupY);
        if (group == null)
            return null;
        
        return (Tile) group.getChild("TILE_"+x+"_"+y);
    }
    
    public TileGroup findGroup(int x, int y){
        return (TileGroup) getChild("GROUP_"+x+"_"+y);
    }
    
    public void setTextureSet(TextureSet set){
        this.textureSet = set;
    }
    
    public TextureSet getTextureSet(){
        return textureSet;
    }
    
    @Override
    public void write(JMEExporter ex) throws IOException {
        OutputCapsule cap = ex.getCapsule(this);
        cap.write(lightmapRes, "MapsResolution", 256);
        cap.write(tileScale, "TileScale", 0);
        cap.write(groupSize, "GroupSize", 3);
        cap.write(gridRes, "GridResolution", 16);
        
        ArrayList<TileGroup> groups = new ArrayList<TileGroup>();
        for (Spatial child : children){
            if (child.getName().startsWith("GROUP")){
                groups.add((TileGroup)child);
            }
        }
        cap.writeSavableArrayList(groups, "Groups", null);
        
    }
    
    @Override
    public void read(JMEImporter im) throws IOException {
        InputCapsule cap = im.getCapsule(this);
        lightmapRes = cap.readInt("MapsResolution", 256);
        tileScale = cap.readFloat("TileScale", 0);
        groupSize = cap.readInt("GroupSize", 3);
        gridRes = cap.readInt("GridResolution", 16);
        markers = false;
        textureSet = EditorState.getState().texSet;
        
        @SuppressWarnings("unchecked")
        ArrayList<TileGroup> groups = cap.readSavableArrayList("Groups", null);
        for (TileGroup group: groups){
            attachChild(group);
            for (Spatial child : group.getChildren()){
                Tile tile = (Tile)child;
                int x = tile.getX();
                int y = tile.getY();
                createMarker(x-1, y);
                createMarker(x+1, y);
                createMarker(x, y+1);
                createMarker(x, y-1);
            }
        }
        
        updateFromState();
        update();
        
        instance = this;
    }
    
    public void updateFromState() {
        EditorState state = EditorState.getState();
        
        tileScale = (float)TILE_SIZE / (gridRes-1.0f);
        if (markers != (state.editType == EditType.TILE)){
            showMarkers(state.editType == EditType.TILE);
        }
    }

}
