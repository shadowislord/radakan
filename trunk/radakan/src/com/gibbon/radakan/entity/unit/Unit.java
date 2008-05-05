package com.gibbon.radakan.entity.unit;

import com.gibbon.radakan.entity.Entity;
import com.jme.util.export.Savable;

public interface Unit extends Savable {

    /**
     * Add a new UnitEventListener to be notified of events happening to this unit
     * 
     * @param listener The listener to add
     */
    public void addEventListener(UnitEventListener listener);
    
    /**
     * Remove a UnitEventListener to stop being notified of events of this unit
     * 
     * @param listener The listener to remove
     */
    public void removeEventListener(UnitEventListener listener);
    
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
}
