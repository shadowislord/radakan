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

package com.radakan.entity.unit;

import com.jme.util.export.Savable;
import com.radakan.entity.Entity;
import java.io.PrintStream;
import org.w3c.dom.Node;

/**
 * IUnit is the interface which all entity units must implement.
 * An entity unit provides functionality to the entity it's attached to.
 * Units can provide a visual model to an entity, make it be the influence
 * of physics, allow it to be controlled by the keyboard, 
 * or think rationally using AI.
 * 
 * @author Kirill Vainer
 */
public interface IUnit extends Savable{

    /**
     * Constants which must be returned by the getType() method.
     * No two units of the same type can be added to a single entity.
     */
    public static final int MODEL     = 0x01,
                            ANIMATION = 0x02,
                            PHYSICS   = 0x04,
                            MOVEMENT  = 0x08,
                            BEHAVIOR  = 0x10;
    
    /**
     * Exports the entity data to XML.
     * @param stream PrintStream to which to write the XML
     */
    public void exportXML(PrintStream stream);
    
    /**
     * Imports the unit from an XML node.
     * @param rootUnitNode
     */
    public void importXML(Node rootUnitNode);
       
    /**
     * Update this unit's state
     * Called every frame
     * @param tpf time per frame (1 / fps)
     */
    public void update(float tpf);
    
    /**
     * Attach this unit to an entity
     * This is a callback method used only by entities
     * Use Entity.attachUnit instead
     * 
     * @param entity The entity to attach to
     */
    public void attach(Entity entity);
    
    /**
     * Detach this unit from an entity
     * This is a callback method used only by entities
     * Use Entity.detachUnit instead
     */
    public void detach();
    
    /**Returns the type of unit.
     * 
     * @return Type of unit
     */
    public int getType();
}
