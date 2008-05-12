package com.gibbon.tools;

import com.gibbon.jme.context.JmeContext;
import com.gibbon.jme.context.RenderPass;
import com.gibbon.jme.context.lwjgl.LWJGLCanvas;
import com.gibbon.radakan.error.ErrorReporter;
import com.gibbon.tools.SpecialStateRenderPass.SpecialState;
import com.jme.app.SimpleGame;
import com.jme.math.Vector3f;
import com.jme.renderer.ColorRGBA;
import com.jme.scene.shape.Box;
import com.jme.scene.state.BlendState;
import com.jme.scene.state.BlendState.DestinationFunction;
import com.jme.scene.state.BlendState.SourceFunction;
import com.jme.scene.state.TextureState;
import com.jme.util.TextureManager;
import java.awt.Canvas;
import java.io.File;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.logging.Level;
import java.util.logging.Logger;

public class PreviewTool extends javax.swing.JFrame {
    
    private JmeContext cx;
    private LWJGLCanvas glCanvas;
    private ModelCameraHandler handler;
    
    private SpecialStateRenderPass render;
    
    public PreviewTool() {
        initComponents();
        setVisible(true);
        
        new Thread(){
            @Override
            public void run(){
                try {
                    cx.waitFor();

                    handler = new ModelCameraHandler(cx.getRenderer().getCamera());
                    glCanvas.addMouseWheelListener(handler);
                    glCanvas.addMouseListener(handler);
                    glCanvas.addMouseMotionListener(handler);

                    render = new SpecialStateRenderPass();

                    Box box = new Box("mybox", Vector3f.ZERO, 10f, 10f, 10f);
                    
                    BlendState bs = cx.getRenderer().createBlendState();
                    bs.setBlendEnabled(true);
                    bs.setSourceFunction(SourceFunction.SourceAlpha);
                    bs.setDestinationFunction(DestinationFunction.OneMinusSourceAlpha);
                    box.setColorBuffer(null);
                    box.setDefaultColor(ColorRGBA.red);
                    
                    TextureState ts = cx.getRenderer().createTextureState();
                    ts.setEnabled(true);
                    URL url = new File("E:\\RADAKAN\\data\\images\\Monkey.jpg").toURI().toURL();
                    ts.setTexture(TextureManager.loadTexture(url));
                    box.setRenderState(ts);
                    
                    box.updateGeometricState(0.0f, true);
                    box.updateRenderState();
                    
                    render.add(box);
                    cx.getPassManager().add(render);
                } catch (MalformedURLException ex) {
                    Logger.getLogger(PreviewTool.class.getName()).log(Level.SEVERE, null, ex);
                } catch (InterruptedException ex) {
                    ErrorReporter.reportError("", ex);
                }
            }
        }.start();
    }
    
    public Canvas createCanvas(){
        try {
            cx = JmeContext.create(JmeContext.getDefaultImplementorClassName(), JmeContext.CONTEXT_CANVAS);
            cx.start();
            
            glCanvas = (LWJGLCanvas) cx.getCanvas();

            return glCanvas;
        } catch (Throwable ex) {
            ErrorReporter.reportError("Error while initializing canvas", ex);
        }
        
        return null;
    }
    
    /** This method is called from within the constructor to
     * initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is
     * always regenerated by the Form Editor.
     */
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        shadingMode = new javax.swing.ButtonGroup();
        pnlSplit = new javax.swing.JSplitPane();
        canvas = createCanvas();
        pnlButtons = new javax.swing.JPanel();
        btnWire = new javax.swing.JToggleButton();
        btnSolid = new javax.swing.JToggleButton();
        btnTextured = new javax.swing.JToggleButton();
        btnMaterial = new javax.swing.JToggleButton();
        btnAnimation = new javax.swing.JButton();

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);
        setTitle("PreviewTool");
        addWindowListener(new java.awt.event.WindowAdapter() {
            public void windowClosed(java.awt.event.WindowEvent evt) {
                formWindowClosed(evt);
            }
        });

        pnlSplit.setDividerLocation(500);
        pnlSplit.setOrientation(javax.swing.JSplitPane.VERTICAL_SPLIT);
        pnlSplit.setTopComponent(canvas);

        shadingMode.add(btnWire);
        btnWire.setIcon(new javax.swing.ImageIcon(getClass().getResource("/icons/wire.gif"))); // NOI18N
        btnWire.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btnWireActionPerformed(evt);
            }
        });

        shadingMode.add(btnSolid);
        btnSolid.setIcon(new javax.swing.ImageIcon(getClass().getResource("/icons/solid.gif"))); // NOI18N
        btnSolid.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btnSolidActionPerformed(evt);
            }
        });

        shadingMode.add(btnTextured);
        btnTextured.setIcon(new javax.swing.ImageIcon(getClass().getResource("/icons/textured.gif"))); // NOI18N
        btnTextured.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btnTexturedActionPerformed(evt);
            }
        });

        shadingMode.add(btnMaterial);
        btnMaterial.setIcon(new javax.swing.ImageIcon(getClass().getResource("/icons/material.gif"))); // NOI18N
        btnMaterial.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btnMaterialActionPerformed(evt);
            }
        });

        btnAnimation.setIcon(new javax.swing.ImageIcon(getClass().getResource("/icons/animation.gif"))); // NOI18N

        org.jdesktop.layout.GroupLayout pnlButtonsLayout = new org.jdesktop.layout.GroupLayout(pnlButtons);
        pnlButtons.setLayout(pnlButtonsLayout);
        pnlButtonsLayout.setHorizontalGroup(
            pnlButtonsLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(pnlButtonsLayout.createSequentialGroup()
                .addContainerGap()
                .add(btnWire, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, 56, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(btnSolid, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, 56, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(btnTextured, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, 56, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(btnMaterial, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, 56, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(btnAnimation, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, 55, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
                .addContainerGap(274, Short.MAX_VALUE))
        );
        pnlButtonsLayout.setVerticalGroup(
            pnlButtonsLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(org.jdesktop.layout.GroupLayout.TRAILING, pnlButtonsLayout.createSequentialGroup()
                .addContainerGap()
                .add(pnlButtonsLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.TRAILING)
                    .add(org.jdesktop.layout.GroupLayout.LEADING, btnAnimation, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .add(btnMaterial, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .add(btnTextured, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .add(org.jdesktop.layout.GroupLayout.LEADING, btnSolid, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .add(org.jdesktop.layout.GroupLayout.LEADING, btnWire, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                .addContainerGap())
        );

        pnlSplit.setRightComponent(pnlButtons);

        org.jdesktop.layout.GroupLayout layout = new org.jdesktop.layout.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(pnlSplit, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 589, Short.MAX_VALUE)
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(org.jdesktop.layout.GroupLayout.TRAILING, pnlSplit, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 585, Short.MAX_VALUE)
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents
    
    private void updateShadingMode(){
        if (btnWire.isSelected()){
            render.setState(SpecialState.WIREFRAME);
        }else if (btnSolid.isSelected()){
            render.setState(SpecialState.SOLID);
        }else if (btnTextured.isSelected()){
            render.setState(SpecialState.TEXTURED);
        }else if (btnMaterial.isSelected()){
            render.setState(SpecialState.MATERIAL);
        }
    }
    
    private void btnWireActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_btnWireActionPerformed
        updateShadingMode();
    }//GEN-LAST:event_btnWireActionPerformed

    private void btnSolidActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_btnSolidActionPerformed
        updateShadingMode();
    }//GEN-LAST:event_btnSolidActionPerformed

    private void btnTexturedActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_btnTexturedActionPerformed
        updateShadingMode();
    }//GEN-LAST:event_btnTexturedActionPerformed

    private void btnMaterialActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_btnMaterialActionPerformed
        updateShadingMode();
    }//GEN-LAST:event_btnMaterialActionPerformed

    private void formWindowClosed(java.awt.event.WindowEvent evt) {//GEN-FIRST:event_formWindowClosed
        cx.dispose();
    }//GEN-LAST:event_formWindowClosed
    
    /**
     * @param args the command line arguments
     */
    public static void main(String args[]) {
        java.awt.EventQueue.invokeLater(new Runnable() {
            public void run() {
                new PreviewTool();
            }
        });
    }
    
    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JButton btnAnimation;
    private javax.swing.JToggleButton btnMaterial;
    private javax.swing.JToggleButton btnSolid;
    private javax.swing.JToggleButton btnTextured;
    private javax.swing.JToggleButton btnWire;
    private java.awt.Canvas canvas;
    private javax.swing.JPanel pnlButtons;
    private javax.swing.JSplitPane pnlSplit;
    private javax.swing.ButtonGroup shadingMode;
    // End of variables declaration//GEN-END:variables
    
}
