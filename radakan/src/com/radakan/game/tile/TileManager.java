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

import com.jme.image.Image;

/**In Middle of creation
 * 
 * @author Joshua Montgomery
 * @created Aug 18, 2008
 */
public class TileManager
{
	private Tile tiles[][];
	
	public TileManager(int x,int y)
	{
		tiles = new Tile[x][y];
	}
	
	public boolean addTile(Tile tile,int x,int y)
	{
		if(!inTileBounds(x,y))
			return false;
		tiles[x][y] = tile;
		return true;
	}
	
	public Tile[] getTileGrid(int centerX,int centerY,int gridW,int gridH)
	{
		Tile grid[] = new Tile[gridW * gridH];
		//TODO: Build tile grid		                            
		return grid;
	}
	
	public Tile getTileAt(int x,int y)
	{
		if(!inTileBounds(x,y))
			return null;//tile doesn't exist
		return tiles[x][y];
	}
	
	public boolean inTileBounds(int x, int y)
	{
		if((x > tiles.length && x > -1) || (y > tiles[0].length && y > -1))
			return false;
		return true;
	}
	
	public void loadTileMap(Image tileMap)
	{
		//TODO: Split the image into individual tiles and store them.
	}
	

}
