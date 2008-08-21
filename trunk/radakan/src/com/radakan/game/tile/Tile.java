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

import com.jme.scene.Node;

/**
 * @author Joshua Montgomery
 * @created Aug 18, 2008
 */
public class Tile extends Node
{
	private static final long serialVersionUID = 5468917005537121768L;

        /**
         * flag: used in TileManager to mark which tiles to keep
         */
        boolean flag = false;
        
        public final int x, y;
        
        public Tile(int x, int y){
            this.x = x;
            this.y = y;
        }
        
}