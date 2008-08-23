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

import java.io.IOException;
import java.util.HashSet;
import java.util.Set;

import com.jme.util.export.JMEExporter;
import com.jme.util.export.JMEImporter;
import com.radakan.entity.Entity;

/**
 * Defines some generic methods for the Unit interface to reduce boilerplate code
 * 
 * @author Momoko_Fan
 */
public abstract class AbstractUnit implements IUnit {

    private transient Set<IUnitEventListener> listeners = 
                         new HashSet<IUnitEventListener>();
    
    /**
     * Keep a reference to the currently attached entity
     */
    protected transient Entity entity;
    
    public void addEventListener(IUnitEventListener listener) {
        listeners.add(listener);
    }

    public void removeEventListener(IUnitEventListener listener) {
        listeners.remove(listener);
    }

    /**
     * Used by subclasses to notify event listeners of a UnitEvent
     * 
     * @param event The event that happened
     */
    protected void notifyListeners(UnitEvent event){
        for (IUnitEventListener listener : listeners)
            listener.onUnitEvent(event);
    }

    public void attach(Entity entity){
        this.entity = entity;
    }
    
    public void detach(){
        this.entity = null;
    }
    
    public abstract void update(float tpf);
    
    public Class<?> getClassTag() {
        return this.getClass();
    }

    public abstract void write(JMEExporter ex) throws IOException;

    public abstract void read(JMEImporter im) throws IOException;

}
