package com.radakan.entity.unit;

import com.jme.util.export.Savable;
import com.radakan.entity.Entity;
import java.io.PrintStream;
import org.w3c.dom.Node;

public interface IUnit extends Savable
{

    public void exportXML(PrintStream stream);
    
    public void importXML(Node rootEntityNode);
       
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
