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

package com.radakan.game.entity.unit;

import com.radakan.game.entity.Entity;

public class UnitEvent {

    protected Object source;
    protected Entity entity;
    protected String type;
    
    public static final String ATTACH = "Attach",
                               DETACH = "Detach",
                               ENTITY_BIRTH   = "EntityBirth",
                               ENTITY_DISPOSE = "EntityDispose";
    
    public void setSource(Object source){
        this.source = source;
    }
    
    public void setEntity(Entity entity){
        this.entity = entity;
    }
    
    public Object getSource(){
        return source;
    }
    
    public Entity getEntity(){
        return entity;
    }
    
    public void setType(String type){
        this.type = type;
    }
    
    public String getType(){
        return type;
    }
    
    @Override
    public String toString(){
        return getClass().getSimpleName()+"(source="+source+", entity="+entity+", type="+type+")";
    }
    
}
