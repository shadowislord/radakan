package com.gibbon.tools.world;

import com.jme.image.Image;
import com.jme.image.Image.Format;
import com.jme.image.Texture2D;
import com.jme.scene.Spatial;
import com.jme.scene.TriMesh;
import com.radakan.entity.Entity;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.PrintStream;
import java.nio.ByteBuffer;
import java.nio.FloatBuffer;
import java.util.ArrayList;
import java.util.List;
import javax.imageio.ImageIO;

public class WorldTileExporter {

    private File folder;
    
    public void exportTile(Tile t, TextureSet texSet, List<Integer> usedTextures){
        PrintStream stream = null;
        try{
            stream = new PrintStream(new File(folder, "tile_"+t.getX()+"_"+t.getY()+".xml"));
        } catch (FileNotFoundException ex){
        }
        
        stream.println("<tile>");
        
        String maps = usedTextures.toString();
        if (maps.length() > 2){
            maps = maps.substring(1, maps.length()-1);
            stream.println("    <terrain textureset=\""+texSet.toString()+"\"\n" +
                           "             usedmaps=\"" + maps + "\" >");
        }else{
            stream.println("    <terrain>");
        }
        // --- EXPORT Terrain
         
        
        TriMesh mesh = t.getTerrain();
        FloatBuffer vertBuf = mesh.getVertexBuffer();
        int size = World.getWorld().getGridResolution();
        for (int y = 0; y < size; y++){
            for (int x = 0; x < size; x++){
                float h = vertBuf.get( (y * size + x) * 3 + 1 );
                stream.print(h);
                //if (y != size - 1 && x != size - 1)
                    stream.print(" ");
            }
            stream.println("\n");
        }
        stream.println();
        stream.println("    </terrain>");
        
        for (Spatial spat : t.getChildren()){
            if (spat != t.getTerrain()){
                Entity ent = (Entity) spat.getUserData("Entity");
                ent.exportXML(stream);
            }
        }
        
        stream.println("</tile>");
        stream.close();
    }
    
    public void exportGroup(TileGroup g) throws IOException{
        List<Integer> usedTextures = new ArrayList<Integer>();
        
        if (g.isTextureSet()){
            for (int i = 0; i < g.getAlphamapCount(); i++){
                Texture2D tex = g.getPaintableAlphamap(i);
                if (!TextureSet.isNilImage(tex.getImage())){
                    Image img = tex.getImage();
                    if (img.getFormat() != Format.Alpha8)
                        throw new UnsupportedOperationException("Only ALPHA8 maps are supported");

                    // write alphamap data to grayscale image
                    ByteBuffer data = img.getData(0);
                    BufferedImage bImage = new BufferedImage(img.getWidth(), img.getHeight(), BufferedImage.TYPE_BYTE_GRAY);
                    boolean empty = true;
                    for (int y = 0; y < img.getHeight(); y++){
                        for (int x = 0; x < img.getWidth(); x++){
                            byte b = data.get(y * img.getWidth() + x);
                            int argb = b << 8 | b << 16 | b << 24;
                            if (argb > 0)
                                empty = false;

                            bImage.setRGB(x, y, argb);
                        }
                    }
                    if (!empty){
                        // export image to folder
                        File alphamapFile = new File(folder, "alpha_"+g.getX()+"_"+g.getY()+"_"+i+".png");
                        ImageIO.write(bImage, "png", alphamapFile);

                        usedTextures.add(i);
                    }
                }
            }
        }
        
        
        for (Spatial child : g.getChildren()){
            if (child instanceof Tile){
                Tile t = (Tile)child;
                exportTile(t, g.getTextureSet(), usedTextures);
            }
        }
    }
    
    public void export(World world, File folder) throws IOException {
        // types are already written in entities.xml file
        // Only need to copy tiles and models
        this.folder = folder;
        
        File worldMetaFile = new File(folder, "world.xml");
        PrintStream stream = new PrintStream(worldMetaFile);
        stream.println("<world>");
        
        
        
        for (Spatial spat : world.getChildren()){
            if (spat instanceof TileGroup){
                TileGroup g = (TileGroup) spat;
                exportGroup(g);
            }
        }
        
        stream.println("</world>");
        stream.close();
    }
    
}
