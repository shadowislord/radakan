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

import com.jme.input.KeyInput;
import com.jme.input.MouseInput;

import org.fenggui.Display;
import org.fenggui.FengGUI;
import org.fenggui.render.Binding;
import org.fenggui.render.lwjgl.LWJGLBinding;

import org.lwjgl.opengl.GL11;
import org.lwjgl.opengl.GL20;
import org.lwjgl.opengl.GLContext;

/**
 * FengGuiPass allows the use of the FengGUI library with jME seamlessly.<br/>
 * Please note that a InputPass in the same PassManager is required for FengGUI mouse/keyboard control to work.
 * 
 * @author Momoko_Fan
 */
public class FengGuiPass extends Pass {

    private Display display;
    private boolean shaders;
    private GuiManager manager;
    private FengJMEListener listener;
    
    public FengGuiPass(GuiManager manager){
        super(PassType.POST_RENDER, "FengGUI-UserInterface");
        this.manager = manager;
    }
    
    @Override
    public void runPass(JmeContext cx) {
        GL11.glMatrixMode(GL11.GL_TEXTURE);
        GL11.glLoadIdentity();
        if (shaders){
            GL20.glUseProgram(0);
        }

        manager.update(display, cx.getPassManager().getTPF());
        
        GL11.glTexParameteri(GL11.GL_TEXTURE_2D, GL11.GL_TEXTURE_MAG_FILTER, GL11.GL_LINEAR);
        GL11.glTexParameteri(GL11.GL_TEXTURE_2D, GL11.GL_TEXTURE_MIN_FILTER, GL11.GL_LINEAR);
        
        display.display();
    }

    @Override
    public void initPass(JmeContext cx) {
        shaders = GLContext.getCapabilities().GL_ARB_shader_objects;
        
        Binding binding = new LWJGLBinding();
        display = FengGUI.createDisplay(binding);
        this.listener = new FengJMEListener(display);
        
        KeyInput.get().addListener(listener);
        MouseInput.get().addListener(listener);
        
        manager.create(display);
    }

    @Override
    public void cleanPass(JmeContext cx) {
        manager.destroy(display);
    }

}
