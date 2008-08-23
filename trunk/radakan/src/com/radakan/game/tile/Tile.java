/*
 * Radakan is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Radakan is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Radakan.  If not, see <http://www.gnu.org/licenses/>.
 */
package com.radakan.game.tile;

import com.jme.bounding.BoundingBox;
import com.jme.renderer.Renderer;
import com.jme.scene.Node;
import com.jme.scene.Spatial;
import com.jme.scene.TriMesh;
import com.jme.scene.VBOInfo;
import com.jme.system.DisplaySystem;
import com.jme.util.resource.ResourceLocatorTool;
import com.radakan.util.ErrorHandler;
import java.io.IOException;
import java.io.InputStream;
import java.net.URL;
import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.logging.Logger;

/**
 * @author Joshua Montgomery
 * @created Aug 18, 2008
 */
public class Tile extends Node
{
	private static final long serialVersionUID = 5468917005537121768L;

        private static final Logger logger = Logger.getLogger(Tile.class.getName());
        
        private Spatial terrain;
        
        public static int GROUP_SIZE = -1;
        public static int TILE_RESOLUTION = -1;
        public static int TILE_SIZE = -1;
        public static boolean USE_LIGHTMAP = false;
        
        public static final Map<String, TextureSet> TEXTURE_SETS = new HashMap<String, TextureSet>();
        
        /**
         * The tile location
         */
        public final int x, y;
        
        public Tile(int x, int y){
            super("TILE_"+x+"_"+y);
            this.x = x;
            this.y = y;
        }
        
        public void addObject(Spatial object){
            attachChild(object);
        }
        
        public void setTerrain(Spatial terrain){
            this.terrain = terrain;
            attachChildAt(terrain, 0);
        }
        
        public boolean load(){
            InputStream in = null;
            try {
                URL url = ResourceLocatorTool.locateResource("tile", getName() + ".xml");
                if (url == null) {
                    return false;
                }
                in = url.openStream();
                TileLoader loader = new TileLoader();
                loader.loadTile(x, y, in, this);
                
                int nObjects = getQuantity() - (terrain != null ? 1 : 0);
                System.out.println(getName() + " loaded.");
                //logger.finest(getName()+" loaded successfuly. # of objects "+nObjects+", has terrain "+(terrain != null));
                
                setModelBound(new BoundingBox());
                updateModelBound();
                
                return true;
            } catch (IOException ex) {
                ErrorHandler.reportError("Error while reading "+getName(), ex);
            } finally {
                try {
                    if (in != null)
                        in.close();
                } catch (IOException ex) {
                    ErrorHandler.reportError("Error while reading "+getName(), ex);
                }
            }
            
            return false;
        }
        
        public boolean unload(){
            System.out.println(getName() + " unloaded.");
            //logger.finest(getName() + " detached.");
            
            Renderer r = DisplaySystem.getDisplaySystem().getRenderer();
            if (r.supportsVBO()){
                // make sure to unload VBO if set
                if (terrain instanceof TriMesh){
                    TriMesh mesh = (TriMesh) terrain;
                    VBOInfo vbo = mesh.getVBOInfo();
                    if (vbo != null){
                        r.deleteVBO(vbo.getVBOVertexID());
                        r.deleteVBO(vbo.getVBOColorID());
                        r.deleteVBO(vbo.getVBOFogCoordsID());
                        r.deleteVBO(vbo.getVBOIndexID());
                        r.deleteVBO(vbo.getVBONormalID());
                        r.deleteVBO(vbo.getVBOTextureID(0));
                    }
                }
            }
            
            return true;
        }
        
}
