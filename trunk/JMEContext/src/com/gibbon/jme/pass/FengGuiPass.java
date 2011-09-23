/*
 * Copyright (c) 2007, MFKARPG
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     - Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     - Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     - Neither the name of the Gibbon Entertainment nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY 'Gibbon Entertainment' "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL 'Gibbon Entertainment' BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
    //private FengJMEListener listener;
    //private FengJMEInputHandler handler;
    private String theme;
    
    public FengGuiPass(String theme, IGuiManager manager){
        super(PassType.POST_RENDER, "FengGUI-UserInterface");
        this.manager = manager;
        this.theme = theme;
    }
    
    //public InputHandler getHandler(){
    //    return handler;
    //}
    
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
        
        //this.handler = new FengJMEInputHandler(display);

        manager.create(display);
    }

    @Override
    public void cleanPass(JmeContext cx) {
        manager.destroy(display);
    }
    
    public Display getDisplay() {
    	return display;
    }

}
