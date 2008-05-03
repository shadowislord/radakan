package com.gibbon.jme.context;

import com.gibbon.jme.context.JmeContext;
import com.gibbon.jme.context.Pass;
import com.gibbon.jme.context.PassType;

import com.jme.input.KeyInput;
import com.jme.input.KeyInputListener;

import com.jme.input.MouseInput;
import org.fenggui.Display;
import org.fenggui.FengGUI;
import org.fenggui.render.Binding;
import org.fenggui.render.lwjgl.AWTGLCanvasBinding;
import org.fenggui.render.lwjgl.LWJGLBinding;

import org.lwjgl.opengl.AWTGLCanvas;
import org.lwjgl.opengl.GL11;
import org.lwjgl.opengl.GL20;
import org.lwjgl.opengl.GLContext;

public class FengGuiPass extends Pass implements KeyInputListener {

    private Display display;
    private boolean shaders;
    
    public FengGuiPass(){
        super(PassType.POST_RENDER, "FengGUI-UserInterface");
    }
    
    @Override
    public void runPass(JmeContext cx) {
        GL11.glMatrixMode(GL11.GL_TEXTURE);
        GL11.glLoadIdentity();
        if (shaders){
            GL20.glUseProgram(0);
        }

        display.display();
    }

    @Override
    public void initPass(JmeContext cx) {
        shaders = GLContext.getCapabilities().GL_ARB_shader_objects;
        
        Binding binding = null;
        if (cx.getCanvas() != null){
            binding = new AWTGLCanvasBinding((AWTGLCanvas)cx.getCanvas());
        }else{
            binding = new LWJGLBinding();
        }
        
        display = FengGUI.createDisplay(binding);
        
        KeyInput.get().addListener(this);
        MouseInput.get();
    }

    @Override
    public void cleanPass(JmeContext cx) {
    }

    public void onKey(char keyChar, int keyNum, boolean pressed) {
        
    }

}
