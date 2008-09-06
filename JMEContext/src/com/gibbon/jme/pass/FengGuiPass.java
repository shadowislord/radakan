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

package com.gibbon.jme.pass;

import org.fenggui.Display;
import org.fenggui.FengGUI;
import org.lwjgl.opengl.GL20;
import org.lwjgl.opengl.GLContext;

import com.gibbon.jme.context.JmeContext;
import com.gibbon.jme.util.FengJMEInputHandler;
import com.gibbon.jme.util.FengJMEListener;
import com.gibbon.jme.util.IGuiManager;
import com.jme.input.InputHandler;
import com.jme.input.KeyInput;
import com.jme.input.MouseInput;
import java.io.IOException;
import org.fenggui.FengGUIOptional;
import org.fenggui.binding.render.Binding;
import org.fenggui.binding.render.lwjgl.LWJGLBinding;
import org.fenggui.theme.DefaultTheme;
import org.fenggui.theme.XMLTheme;
import org.fenggui.theme.xml.IXMLStreamableException;

/**
 * FengGuiPass allows the use of the FengGUI library with jME seamlessly.<br/>
 * Please note that a InputPass in the same PassManager is required for FengGUI mouse/keyboard control to work.
 * 
 * @author Momoko_Fan
 */
public class FengGuiPass extends Pass {

    private Display display;
    private boolean shaders;
    private IGuiManager manager;
    private FengJMEListener listener;
    private FengJMEInputHandler handler;
    private String theme;
    
    public FengGuiPass(String theme, IGuiManager manager){
        super(PassType.POST_RENDER, "FengGUI-UserInterface");
        this.manager = manager;
        this.theme = theme;
    }
    
    public InputHandler getHandler(){
        return handler;
    }
    
    @Override
    public void runPass(JmeContext cx) {
//        GL11.glMatrixMode(GL11.GL_TEXTURE);
//        GL11.glLoadIdentity();
        if (shaders){
            GL20.glUseProgram(0);
        }
        cx.getRenderer().clearZBuffer();
        manager.update(display, cx.getPassManager().getTPF());
        display.display();
    }

    @Override
    public void initPass(JmeContext cx) {
        shaders = GLContext.getCapabilities().GL_ARB_shader_objects;
        
        Binding binding = new LWJGLBinding();
        display = FengGUI.createDisplay(binding);
        
        if (theme == null){
            FengGUI.setTheme(new DefaultTheme());
        }else{
            try{
                FengGUI.setTheme(new XMLTheme(theme));
            } catch (IOException ex){
                ex.printStackTrace();
            } catch (IXMLStreamableException ex){
                ex.printStackTrace();
            }
        }
        FengGUI.initPrototypes();
        FengGUIOptional.initOptional();
        
        this.handler = new FengJMEInputHandler(display);

        manager.create(display);
    }

    @Override
    public void cleanPass(JmeContext cx) {
        manager.destroy(display);
    }

}
