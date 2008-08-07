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

package com.gibbon.jme.util;

import org.fenggui.Display;

/**
 * GuiManager is provided to a FengGuiPass to create the GUI and handle callbacks.
 * 
 * @author Momoko_Fan
 */
public interface IGuiManager {

    public void create(Display display);
    public void destroy(Display display);
    public void update(Display display, float tpf);
    
}
