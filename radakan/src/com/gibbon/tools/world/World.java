package com.gibbon.tools.world;

import com.gibbon.jme.context.JmeContext;
import com.jme.bounding.BoundingBox;
import com.jme.image.Texture.MagnificationFilter;
import com.jme.image.Texture.MinificationFilter;
import com.jme.image.Texture.WrapMode;
import com.jme.light.DirectionalLight;
import com.jme.light.PointLight;
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
import com.jme.scene.state.TextureState;
import com.jme.scene.state.ZBufferState;
import com.jme.scene.state.ZBufferState.TestFunction;
import com.jme.util.TextureManager;
import java.io.File;
import java.net.MalformedURLException;
import java.net.URL;

public class World extends Node {

    public static final int TILE_SIZE = 64;
    
    private static World instance;
    
    private int groupSize = 3;
    private int gridRes = 16;
    private boolean markers = true;
    private PointLight camLight;
    
    public World(){
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

        updateFromState();
        update();
        
        instance = this;
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
    
    @Override
    public void updateWorldData(float tpf){
        super.updateWorldData(tpf);
        
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
                }
            }
        }
        
        markers = show;
    }
    
    public void applyTileMaterials(Tile t){
        try {
            TextureManager.COMPRESS_BY_DEFAULT = false;
            
            TextureState ts = JmeContext.get().getRenderer().createTextureState();
            URL url = new File("data\\images\\Monkey.jpg").toURI().toURL();
            ts.setTexture(TextureManager.loadTexture(url, MinificationFilter.BilinearNoMipMaps, MagnificationFilter.Bilinear, 1.0f, true));
            ts.getTexture().setWrap(WrapMode.Repeat);
            t.setRenderState(ts);
        } catch (MalformedURLException ex) {
            ex.printStackTrace();
        }
    }
    
    public void createMarker(int x, int y){
        if (findTile(x,y) != null || findMarker(x,y) != null)
            return;
        
        Box b = new Box("MARKER_"+x+"_"+y, 
                        new Vector3f(x * TILE_SIZE + TILE_SIZE * 0.5f, 
                                     0.0f,
                                     y * TILE_SIZE + TILE_SIZE * 0.5f), 
                        3, 0.5f, 3);
        b.setDefaultColor(ColorRGBA.red);
        b.setColorBuffer(null);
        
        b.setCullHint(markers ? CullHint.Never : CullHint.Always);
        b.setLightCombineMode(LightCombineMode.Off);
        
        b.setModelBound(new BoundingBox());
        b.updateModelBound();
        
        attachChild(b);
    }
    
    public void createTile(int x, int y){
        Tile t = new Tile(x,y);
        t.setLocalTranslation(x * TILE_SIZE, 0.0f, y * TILE_SIZE);
        
        float scale = (float)TILE_SIZE / (gridRes-1.0f);
        
        TriMesh grid = TerrainUtil.createGrid(gridRes, gridRes, 
                                              new Vector3f(scale, 1f, scale), 
                                              t);
        grid.setName("TERRAIN_"+x+"_"+y);
        t.setTerrain(grid);
        
        int groupX = (int) Math.ceil((float)x / groupSize);
        int groupY = (int) Math.ceil((float)y / groupSize);
        
        Node group = findGroup(groupX, groupY);
        if (group == null)
            group = createGroup(groupX, groupY);
        
        group.attachChild(t);
        
        applyTileMaterials(t);
        
        grid.setModelBound(new BoundingBox());
        grid.updateModelBound();
        
        grid.updateGeometricState(0, true);
        
        createMarker(x-1, y);
        createMarker(x+1, y);
        createMarker(x, y+1);
        createMarker(x, y-1);
    }
    
    public Node createGroup(int x, int y){
        Node group = new Node("GROUP_"+x+"_"+y);
        
        group.setModelBound(new BoundingBox());
        group.updateModelBound();
        
        attachChild(group);
        
        return group;
    }
    
    public Box findMarker(int x, int y){
        return (Box) getChild("MARKER_"+x+"_"+y);
    }
    
//    public Tile findTile(int x, int y){
//        return (Tile) getChild("TILE_"+x+"_"+y);
//    }

    public Tile findTile(int x, int y){
        int groupX = (int) Math.ceil((float)x / groupSize);
        int groupY = (int) Math.ceil((float)y / groupSize);
        Node group = findGroup(groupX, groupY);
        if (group == null)
            return null;
        
        return (Tile) group.getChild("TILE_"+x+"_"+y);
    }
    
    public Node findGroup(int x, int y){
        return (Node) getChild("GROUP_"+x+"_"+y);
    }
    
    public void updateFromState() {
        EditorState state = EditorState.getState();
        
        if (markers != (state.editType == EditType.TILE)){
            showMarkers(state.editType == EditType.TILE);
        }
    }
}
