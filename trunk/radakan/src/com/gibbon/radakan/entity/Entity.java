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

package com.gibbon.radakan.entity;

import com.gibbon.radakan.entity.unit.AbstractUnit;
import com.gibbon.radakan.entity.unit.Unit;
import com.gibbon.radakan.entity.unit.UnitEvent;
import com.jme.util.export.InputCapsule;
import com.jme.util.export.JMEExporter;
import com.jme.util.export.JMEImporter;
import com.jme.util.export.OutputCapsule;
import java.io.IOException;
import java.util.ArrayList;

/**
 * An entity class which implements the component-entity architecture.
 * The entity is actually a unit container..
 * 
 * @author Momoko_Fan
 */
public final class Entity extends AbstractUnit {

    private String name;
    private ArrayList<Unit> units = new ArrayList<Unit>();
    
    public Entity(String name){
        this.name = name;
    }
    
    @SuppressWarnings("unchecked")
    public <T extends Unit> T getUnit(Class<T> clazz){
        for (Unit u : units)
            if (clazz.isInstance(u))
                return (T) u;
        
        return null;
    }
    
    public void attachUnit(Unit unit){
        units.add(unit);
        unit.attach(this);
        
        UnitEvent event = new UnitEvent();
        event.setSource(unit);
        event.setEntity(this);
        event.setType(UnitEvent.ATTACH);
        notifyListeners(event);
    }
    
    public void detachUnit(Unit unit){
        units.remove(unit);
        unit.detach();
        
        UnitEvent event = new UnitEvent();
        event.setSource(unit);
        event.setEntity(this);
        event.setType(UnitEvent.DETACH);
        notifyListeners(event);
    }
    
    public void birth(){
        UnitEvent event = new UnitEvent();
        event.setEntity(this);
        event.setSource(this);
        event.setType(UnitEvent.ENTITY_BIRTH);
        notifyListeners(event);
    }
    
    public void dispose(){
        for (Unit u : units)
            detachUnit(u);
        
        UnitEvent event = new UnitEvent();
        event.setEntity(this);
        event.setSource(this);
        event.setType(UnitEvent.ENTITY_DISPOSE);
        notifyListeners(event);
    }
    
    public void update(float tpf){
        for (Unit u : units)
            u.update(tpf);
    }

    @Override
    public void write(JMEExporter ex) throws IOException {
        OutputCapsule out = ex.getCapsule(this);
        out.write(name, "name", "");
        out.writeSavableArrayList(units, "units", null);
    }

    @Override
    @SuppressWarnings("unchecked")
    public void read(JMEImporter im) throws IOException {
        InputCapsule in = im.getCapsule(this);
        name = in.readString(name, "");
        units = in.readSavableArrayList("units", null);
    }
    
}
