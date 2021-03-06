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

package com.test.context;

import com.gibbon.jme.context.JmeContext;
import com.gibbon.jme.pass.RenderPass;
import com.jme.image.Texture;
import com.jme.light.PointLight;
import com.jme.math.FastMath;
import com.jme.math.Vector3f;
import com.jme.renderer.Camera;
import com.jme.renderer.ColorRGBA;
import com.jme.scene.Node;
import com.jme.scene.shape.RoundedBox;
import com.jme.scene.state.LightState;
import com.jme.scene.state.TextureState;
import com.jme.scene.state.ZBufferState;
import com.jme.scene.state.ZBufferState.TestFunction;
import com.jme.util.TextureManager;
import java.awt.Canvas;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author Momoko_Fan
 */
public class TestApplet extends javax.swing.JApplet {
 
	private static final long serialVersionUID = -6367494341791994136L;
	private JmeContext context;
    
    @Override
    public void init() {
        initComponents();
    }
    
    @Override
    public void start(){
            new Thread(new Runnable(){
                public void run(){
                    try {
                        context.waitFor();
                        
                        Node root = new Node("root");

                        RoundedBox box = new RoundedBox("Box");
                        box.setLocalScale(3);

                        root.attachChild(box);

                        LightState ls = context.getRenderer().createLightState();
                        PointLight pl = new PointLight();
                        pl.setSpecular(ColorRGBA.white);
                        pl.setDiffuse(ColorRGBA.yellow);
                        pl.setAmbient(ColorRGBA.darkGray);
                        pl.setEnabled(true);
                        pl.setLocation(new Vector3f(-5, 5, 5));
                        ls.attach(pl);
                        root.setRenderState(ls);

                        ZBufferState buf = context.getRenderer().createZBufferState();
                        buf.setFunction(TestFunction.LessThanOrEqualTo);
                        buf.setWritable(true);
                        root.setRenderState(buf);

                        TextureState ts = JmeContext.get().getRenderer().createTextureState();
                        Texture t = TextureManager.loadTexture(TestApplet.class.getClassLoader().getResource("jmetest/data/images/Monkey.jpg"));
                        ts.setTexture(t);
                        root.setRenderState(ts);

                        root.updateRenderState();
                        root.updateGeometricState(0, true);

                        context.getPassManager().add(new RotatingBox(root, ColorRGBA.red, true));
                    } catch (InterruptedException ex) {
                        Logger.getLogger(TestApplet.class.getName()).log(Level.SEVERE, null, ex);
                    }
                }
            }).start();
    }

    private static class RotatingBox extends RenderPass {
        
        private float angle = 0;
        private Node root;
        
        private ColorRGBA background;
        private boolean rotateScene = false;
        
        public RotatingBox(Node root, ColorRGBA bgColor, boolean rotateScene){
            super();
            background = bgColor;
            this.rotateScene = rotateScene;
            this.root = root;
            add(root);
        }
        
        @Override
        public void initPass(JmeContext context){
            super.initPass(context);
            
            context.getRenderer().setBackgroundColor(background);
            
            Camera cam = context.getRenderer().getCamera();
            if (rotateScene){
                cam.setLocation(new Vector3f(5,5,5));
            }else{
                // use a different angle if we are the 2nd canvas
                cam.setLocation(new Vector3f(5,7,2));
            }
            
            cam.lookAt(Vector3f.ZERO,Vector3f.UNIT_Y);
        }
        @Override
        public void doUpdate(JmeContext context){
            super.doUpdate(context);
            
            if (rotateScene){
                angle += FastMath.PI * context.getPassManager().getTPF();
                if (angle > FastMath.TWO_PI){
                    angle -= FastMath.TWO_PI;
                }
                root.getLocalRotation().fromAngles(angle,0,0);
            }
        }
    }
    
    private Canvas createCanvas() {
        try {
            context = JmeContext.create(JmeContext.getDefaultImplementorClassName(), JmeContext.CONTEXT_CANVAS);
            context.start();
            return context.getCanvas();
        } catch (InstantiationException ex) {
            Logger.getLogger(TestApplet.class.getName()).log(Level.SEVERE, null, ex);
        }
        
        return null;
    }
    
    /** This method is called from within the init() method to
     * initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is
     * always regenerated by the Form Editor.
     */
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        canvas = createCanvas();

        org.jdesktop.layout.GroupLayout layout = new org.jdesktop.layout.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(canvas, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 323, Short.MAX_VALUE)
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(canvas, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 325, Short.MAX_VALUE)
        );
    }// </editor-fold>//GEN-END:initComponents
    
    
    // Variables declaration - do not modify//GEN-BEGIN:variables
    private java.awt.Canvas canvas;
    // End of variables declaration//GEN-END:variables
    
}
