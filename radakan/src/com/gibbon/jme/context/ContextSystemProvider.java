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

package com.gibbon.jme.context;

import com.jme.system.DisplaySystem;
import com.jme.system.SystemProvider;
import com.jme.util.Timer;

/**
 * A dummy SystemProvider to glue original jME code with the JmeContext system
 * 
 * @author Momoko_Fan
 */
public class ContextSystemProvider implements SystemProvider {
    
    protected ContextDisplaySystem display;
    
    public ContextSystemProvider(ContextDisplaySystem display){
        this.display = display;
    }
    
    public String getProviderIdentifier() {
        return "CONTEXT";
    }

    public DisplaySystem getDisplaySystem() {
        if (display==null){
            display = new ContextDisplaySystem();
        }
        return display;
    }

    public void disposeDisplaySystem() {
        JmeContext.get().dispose();
    }

    public Timer getTimer() {
        return JmeContext.get().getTimer();
    }

    public void installLibs() {
    }
    
}
