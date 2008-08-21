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

import com.jme.math.Vector2f;
import com.jme.scene.Node;
import com.jme.scene.Spatial;

/**In Middle of creation
 * 
 * Changes by Kirill (MomokoFan)
 * - Now extends Node
 * - Some docs
 * 
 * @author Joshua Montgomery
 * @created Aug 18, 2008
 */
public class TileManager extends Node
{

        /** Create a new tile manager */
	public TileManager() {
	}
	
        /**
         * Returns the tile at position X, Y or null if no tile is available at the position
         */
	public Tile getTileAt(int x,int y)
	{
		return (Tile) getChild("TILE_"+x+"_"+y);
	}
	
        public void markFarTiles(){
            for (Spatial child : getChildren()){
                Tile tile = (Tile) child;
                Vector2f pos = new Vector2f(tile.x, tile.y);
                
                
            }
        }
        
        @Override
        public void updateWorldData(float tpf){
            
        }
        
	public void loadTile(int x, int y)
	{
		
	}
        
        
	

}
