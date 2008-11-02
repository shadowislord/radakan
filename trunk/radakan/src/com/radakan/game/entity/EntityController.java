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

package com.radakan.game.entity;

import com.jme.scene.Controller;

/**
 * This class is currently unused.
 * 
 * @author Momoko_Fan
 */
public class EntityController extends Controller {

	private static final long serialVersionUID = 1399402719676167634L;
	private Entity entity;
    
    public EntityController(Entity entity){
        this.entity = entity;
    }
    
    public void update(float tpf) {
        entity.update(tpf);
    }

}
