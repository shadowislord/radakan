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

package com.radakan.entity;

import com.jme.util.export.InputCapsule;
import com.jme.util.export.JMEExporter;
import com.jme.util.export.JMEImporter;
import com.jme.util.export.OutputCapsule;
import com.jme.util.export.Savable;
import com.radakan.entity.unit.IUnitEventListener;
import com.radakan.entity.unit.Unit;
import com.radakan.entity.unit.IUnit;
import com.radakan.entity.unit.ModelUnit;
import com.radakan.entity.unit.UnitEvent;

import com.radakan.util.XMLUtil;
import java.io.IOException;
import java.io.PrintStream;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.Set;
import java.util.logging.Level;
import java.util.logging.Logger;
import org.w3c.dom.Node;

/**
 * An entity class which implements the component-entity architecture.
 * The entity is actually a unit container..
 * 
 * See: http://cowboyprogramming.com/2007/01/05/evolve-your-heirachy/
 * 
 * @author Kirill Vainer
 */
public final class Entity implements Savable
{
	private transient Set<IUnitEventListener> listeners = 
        new HashSet<IUnitEventListener>();


    /**
     * Name of the entity. This is a dynamic variable.
     */
    private String name;
    
    /**Determines which units the entity currently has.*/
    private int unitFlags;
    
    /**
     * Type of entity, given to EntityFactory to load a certain entity from XML definitions.
     */
    private String type;
    private ArrayList<Unit> units = new ArrayList<Unit>();
    
    public Entity(String name, String type){
        this.name = name;
        this.type = type;
    }

    public Entity(Node rootEntityNode) {
        importXML(rootEntityNode);
    }
    
    public String getName(){
        return name;
    }
    
    public String getType(){
        return type;
    }

    public void exportXML(PrintStream stream) {
        stream.println("    <entity name=\""+name+"\" type=\"" + type + "\">");
        for (IUnit u : units){
            u.exportXML(stream);
        }
        stream.println("    </entity>");
    }
    
    @SuppressWarnings("unchecked")
    public void importXML(Node rootEntityNode){
        name = XMLUtil.getAttribute(rootEntityNode, "name");
        type = XMLUtil.getAttribute(rootEntityNode, "type");
        
        Node childNode = rootEntityNode.getFirstChild();
        while (childNode != null){
            String childName = childNode.getNodeName();
            if (!childName.equals("#text")){
                try {
                    Class<? extends Unit> clazz = null;
                    if (childName.contains(".")){
                        clazz = (Class<? extends Unit>) Class.forName(childName);
                    }else{
                        clazz = (Class<? extends Unit>) Class.forName("com.radakan.entity.unit."+childName);
                    }

                    if (clazz == null){
                        childNode = childNode.getNextSibling();
                        continue;
                    }
                         
                    Unit u = clazz.newInstance();
                    attachUnit(u);
                    u.importXML(childNode);
                } catch (ClassNotFoundException ex) {
                    childNode = childNode.getNextSibling();
                    continue;
                } catch (InstantiationException ex){
                    ex.printStackTrace();
                } catch (IllegalAccessException ex){
                    ex.printStackTrace();
                }
            }
            
            childNode = childNode.getNextSibling();
        }
    }
    
    public ModelUnit getModel(){
        return getUnit(ModelUnit.class);
    }
    
    @SuppressWarnings("unchecked")
    public <T extends IUnit> T getUnit(Class<T> clazz){
        for (IUnit u : units)
            if (clazz.isInstance(u))
                return (T) u;
        
        return null;
    }
    
    public void attachUnit(Unit unit){
        units.add(unit);
        addUnitEventListener(unit);
        unit.attach(this);
               
        UnitEvent event = new UnitEvent();
        event.setSource(unit);
        event.setEntity(this);
        event.setType(UnitEvent.ATTACH);
        notifyListeners(event);
    }
    
    public void detachUnit(Unit unit){
        units.remove(unit);
        removeUnitEventListener(unit);
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
        for (IUnit u : units)
            u.update(tpf);
    }

    public void write(JMEExporter ex) throws IOException {
        OutputCapsule out = ex.getCapsule(this);
        out.write(name, "name", "");
        out.writeSavableArrayList(units, "units", null);
    }

    @SuppressWarnings("unchecked")
    public void read(JMEImporter im) throws IOException {
        InputCapsule in = im.getCapsule(this);
        name = in.readString(name, "");
        units = in.readSavableArrayList("units", null);
    }

	@Override
	public Class getClassTag() {
		// TODO Auto-generated method stub
		return null;
	}

   /**
   * Remove a UnitEventListener to stop being notified of events of this unit
   * 
   * @param listener The listener to remove
   */
   public void addUnitEventListener(IUnitEventListener listener) 
   {
	     listeners.add(listener);
   }
   
   /**
    * Add a new UnitEventListener to be notified of events happening to this unit
    * 
    * @param listener The listener to add
    */
   public void removeUnitEventListener(IUnitEventListener listener) 
   {
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
    
}
