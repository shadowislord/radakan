package com.gibbon.tools;

import com.gibbon.jme.context.JmeContext;
import com.gibbon.jme.context.lwjgl.LWJGLCanvas;
import com.gibbon.radakan.error.ErrorReporter;
import com.gibbon.tools.SpecialStateRenderPass.SpecialState;
import com.jme.image.Texture.MagnificationFilter;
import com.jme.image.Texture.MinificationFilter;
import com.jme.math.FastMath;
import com.jme.math.Quaternion;
import com.jme.math.Vector3f;
import com.jme.renderer.ColorRGBA;
import com.jme.renderer.Renderer;
import com.jme.scene.Node;
import com.jme.scene.Spatial;
import com.jme.scene.TriMesh;
import com.jme.scene.shape.Sphere;
import com.jme.scene.state.BlendState;
import com.jme.scene.state.BlendState.DestinationFunction;
import com.jme.scene.state.BlendState.SourceFunction;
import com.jme.scene.state.CullState;
import com.jme.scene.state.CullState.Face;
import com.jme.scene.state.TextureState;
import com.jme.scene.state.ZBufferState;
import com.jme.scene.state.ZBufferState.TestFunction;
import com.jme.util.TextureManager;
import java.awt.Canvas;
import java.io.File;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.JFileChooser;
import javax.swing.JOptionPane;

public class PreviewTool extends javax.swing.JFrame {
    
    private JmeContext cx;
    private LWJGLCanvas glCanvas;
    private ModelCameraHandler handler;
    
    private SpecialStateRenderPass render;
    private Spatial model;
    private Node rootNode = new Node("rootNode");
    private CullState backFaces;
    
    private JFileChooser chooser;
    private FileNameExtensionFilter jme = new FileNameExtensionFilter("jMonkey Engine binary model (*.jme)","jme");
    private FileNameExtensionFilter md2 = new FileNameExtensionFilter("Quake II Model (*.md2)","md2");
    private FileNameExtensionFilter md3 = new FileNameExtensionFilter("Quake III Model (*.md3)","md3");
    private FileNameExtensionFilter md5 = new FileNameExtensionFilter("Doom III Model (*.md5mesh)","md5mesh");
    private FileNameExtensionFilter x3d = new FileNameExtensionFilter("Web3D (*.x3d)","x3d");
    private FileNameExtensionFilter obj = new FileNameExtensionFilter("LightWave 3D Object (*.obj)","obj");
    private FileNameExtensionFilter ms3d = new FileNameExtensionFilter("MilkShape 3D Model (*.ms3d)","ms3d");
    private FileNameExtensionFilter dae = new FileNameExtensionFilter("COLLADA 3D Digital Asset (*.dae)","dae");
    private FileNameExtensionFilter three_ds = new FileNameExtensionFilter("3D Studio 3D Scene (*.3ds)","3ds");
    private FileNameExtensionFilter ase = new FileNameExtensionFilter("3ds Max ASCII Export File (*.ase)","ase");
    private FileNameExtensionFilter meshxml = new FileNameExtensionFilter("Ogre3D Mesh XML (*.xml)", "xml");
    private FileNameExtensionFilter scene = new FileNameExtensionFilter("Ogre3D dotScene (*.scene)", "scene");
    private FileNameExtensionFilter import_combo = new FileNameExtensionFilter("All supported formats (*.md2, *.md3, *.obj, *.ms3d, *.dae, *.3ds, *.ase, *.md5mesh, *.x3d, *.xml)",
                                                                       "md2", "md3", "obj", "ms3d", "dae", "3ds", "ase", "md5mesh", "x3d", "xml", "scene");
    
    
    public PreviewTool() {
        //Logger.getLogger("").setLevel(Level.WARNING);
        
        initComponents();
        setLocationRelativeTo(null);
        setVisible(true);
        
        chooser = new JFileChooser();
        chooser.addChoosableFileFilter(jme);
        chooser.addChoosableFileFilter(md2);
        chooser.addChoosableFileFilter(md3);
        chooser.addChoosableFileFilter(md5);
        chooser.addChoosableFileFilter(x3d);
        chooser.addChoosableFileFilter(obj);
        chooser.addChoosableFileFilter(ms3d);
        chooser.addChoosableFileFilter(dae);
        chooser.addChoosableFileFilter(three_ds);
        chooser.addChoosableFileFilter(ase);
        chooser.addChoosableFileFilter(meshxml);
        chooser.addChoosableFileFilter(scene);
        chooser.addChoosableFileFilter(import_combo);
        chooser.setFileFilter(import_combo);
        
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
                    render.add(rootNode);
                    
                    model = new Sphere("sphere", 30, 20, 10);
                    model.getLocalRotation().fromAngles(-FastMath.HALF_PI, 0, 0);
                    model.setRenderQueueMode(Renderer.QUEUE_TRANSPARENT);
                    
                    BlendState bs = cx.getRenderer().createBlendState();
                    bs.setBlendEnabled(true);
                    bs.setSourceFunction(SourceFunction.SourceAlpha);
                    bs.setDestinationFunction(DestinationFunction.OneMinusSourceAlpha);
                    ((TriMesh)model).setColorBuffer(null);
                    ((TriMesh)model).setDefaultColor(new ColorRGBA(1.0f, 0.7f, 0.7f, 0.5f));
                    
                    TextureManager.COMPRESS_BY_DEFAULT = false;
                    TextureState ts = cx.getRenderer().createTextureState();
                    ts.setEnabled(true);
                    URL url = new File("data\\images\\Monkey.jpg").toURI().toURL();
                    ts.setTexture(TextureManager.loadTexture(url, MinificationFilter.Trilinear, 
                                                                  MagnificationFilter.Bilinear, 1.0f, true));
                    model.setRenderState(ts);
                    
                    rootNode.attachChild(model);
                    
                    
                    ZBufferState zbuf = cx.getRenderer().createZBufferState();
                    zbuf.setFunction(TestFunction.LessThan);
                    zbuf.setWritable(true);
                    rootNode.setRenderState(zbuf);
                    
                    backFaces = cx.getRenderer().createCullState();
                    backFaces.setCullFace(Face.Back);
                    rootNode.setRenderState(backFaces);
                    
                    rootNode.updateGeometricState(0.0f, true);
                    rootNode.updateRenderState();
                    
                    JmeContext.get().getRenderer().getCamera().setLocation(new Vector3f(0, 0, -50));
                    
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
        btnLoad = new javax.swing.JButton();
        chkBounds = new javax.swing.JCheckBox();
        chkNormals = new javax.swing.JCheckBox();
        chkBones = new javax.swing.JCheckBox();
        chkBackfaces = new javax.swing.JCheckBox();
        chkConvert = new javax.swing.JCheckBox();

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);
        setTitle("PreviewTool");
        addWindowListener(new java.awt.event.WindowAdapter() {
            public void windowClosed(java.awt.event.WindowEvent evt) {
                formWindowClosed(evt);
            }
        });

        pnlSplit.setDividerLocation(500);
        pnlSplit.setOrientation(javax.swing.JSplitPane.VERTICAL_SPLIT);
        pnlSplit.setResizeWeight(0.9);
        pnlSplit.setContinuousLayout(true);
        pnlSplit.setLeftComponent(canvas);

        pnlButtons.setMinimumSize(pnlButtons.getSize());

        shadingMode.add(btnWire);
        btnWire.setIcon(new javax.swing.ImageIcon(getClass().getResource("/icons/wire.gif"))); // NOI18N
        btnWire.setToolTipText("Render in wireframe");
        btnWire.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btnWireActionPerformed(evt);
            }
        });

        shadingMode.add(btnSolid);
        btnSolid.setIcon(new javax.swing.ImageIcon(getClass().getResource("/icons/solid.gif"))); // NOI18N
        btnSolid.setToolTipText("Render with lighting only");
        btnSolid.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btnSolidActionPerformed(evt);
            }
        });

        shadingMode.add(btnTextured);
        btnTextured.setIcon(new javax.swing.ImageIcon(getClass().getResource("/icons/textured.gif"))); // NOI18N
        btnTextured.setToolTipText("Render with texture only");
        btnTextured.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btnTexturedActionPerformed(evt);
            }
        });

        shadingMode.add(btnMaterial);
        btnMaterial.setIcon(new javax.swing.ImageIcon(getClass().getResource("/icons/material.gif"))); // NOI18N
        btnMaterial.setSelected(true);
        btnMaterial.setToolTipText("Render with all materials applied");
        btnMaterial.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btnMaterialActionPerformed(evt);
            }
        });

        btnAnimation.setIcon(new javax.swing.ImageIcon(getClass().getResource("/icons/animation.gif"))); // NOI18N
        btnAnimation.setToolTipText("Show animation window");

        btnLoad.setText("Load Model..");
        btnLoad.setToolTipText("Load a model from a file");
        btnLoad.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btnLoadActionPerformed(evt);
            }
        });

        chkBounds.setText("Display bounds");
        chkBounds.setToolTipText("Show bounding volumes");
        chkBounds.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                chkBoundsActionPerformed(evt);
            }
        });

        chkNormals.setText("Display normals");
        chkNormals.setToolTipText("Show normals");
        chkNormals.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                chkNormalsActionPerformed(evt);
            }
        });

        chkBones.setText("Display bones");
        chkBones.setToolTipText("Show bones for animated models");
        chkBones.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                chkBonesActionPerformed(evt);
            }
        });

        chkBackfaces.setText("Display backfaces");
        chkBackfaces.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                chkBackfacesActionPerformed(evt);
            }
        });

        chkConvert.setText("CoordSystem CAD to OGL");
        chkConvert.setToolTipText("Convert between Z-up to Y-up");
        chkConvert.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                chkConvertActionPerformed(evt);
            }
        });

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
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.UNRELATED)
                .add(pnlButtonsLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                    .add(pnlButtonsLayout.createSequentialGroup()
                        .add(pnlButtonsLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                            .add(chkBones)
                            .add(btnLoad))
                        .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                        .add(pnlButtonsLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                            .add(chkBounds)
                            .add(chkNormals)))
                    .add(pnlButtonsLayout.createSequentialGroup()
                        .add(chkBackfaces)
                        .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                        .add(chkConvert)))
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(btnAnimation, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, 55, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
                .addContainerGap(org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );
        pnlButtonsLayout.setVerticalGroup(
            pnlButtonsLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(pnlButtonsLayout.createSequentialGroup()
                .addContainerGap()
                .add(pnlButtonsLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                    .add(btnAnimation, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 76, Short.MAX_VALUE)
                    .add(pnlButtonsLayout.createSequentialGroup()
                        .add(pnlButtonsLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                            .add(chkBounds, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                            .add(btnLoad, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                        .add(2, 2, 2)
                        .add(pnlButtonsLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.TRAILING)
                            .add(chkNormals, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                            .add(chkBones, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                        .add(5, 5, 5)
                        .add(pnlButtonsLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.BASELINE)
                            .add(chkBackfaces, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                            .add(chkConvert)))
                    .add(org.jdesktop.layout.GroupLayout.TRAILING, btnMaterial, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 76, Short.MAX_VALUE)
                    .add(org.jdesktop.layout.GroupLayout.TRAILING, btnTextured, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 76, Short.MAX_VALUE)
                    .add(btnSolid, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 76, Short.MAX_VALUE)
                    .add(btnWire, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 76, Short.MAX_VALUE))
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
            .add(org.jdesktop.layout.GroupLayout.TRAILING, pnlSplit, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 604, Short.MAX_VALUE)
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

    private void btnLoadActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_btnLoadActionPerformed
        if (chooser.showOpenDialog(this) == JFileChooser.APPROVE_OPTION){
            File selected = chooser.getSelectedFile();
            if (selected.exists()){
                int i = selected.getName().lastIndexOf('.');
                if (i > 0){
                    String ext = selected.getName().substring(i+1);
                    try{
                        rootNode.detachAllChildren();
                        model = ModelLoader.loadModel(selected, ext);
                        
                        if (model == null)
                            return;
                        
                        model = ModelLoader.scaleAndCenter(model, 40.0f);
                        
                        rootNode.attachChild(model);
                        
                        rootNode.updateGeometricState(0, true);
                        rootNode.updateRenderState();
                    } catch (Throwable ex){
                        JOptionPane.showMessageDialog(this, 
                                                      "Failed to load model.\nReason: "+ex.toString(), 
                                                      "Error", 
                                                      JOptionPane.ERROR_MESSAGE);
                        ex.printStackTrace();
                    }
                }
            }
        }
    }//GEN-LAST:event_btnLoadActionPerformed

    private void chkBonesActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_chkBonesActionPerformed
        render.setBones(chkBones.isSelected());
    }//GEN-LAST:event_chkBonesActionPerformed

    private void chkBoundsActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_chkBoundsActionPerformed
        render.setBounds(chkBounds.isSelected());
    }//GEN-LAST:event_chkBoundsActionPerformed

    private void chkNormalsActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_chkNormalsActionPerformed
        render.setNormals(chkNormals.isSelected());
    }//GEN-LAST:event_chkNormalsActionPerformed

    private void chkBackfacesActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_chkBackfacesActionPerformed
        backFaces.setEnabled(!chkBackfaces.isSelected());
        rootNode.updateRenderState();
    }//GEN-LAST:event_chkBackfacesActionPerformed

    private void chkConvertActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_chkConvertActionPerformed
        if (chkConvert.isSelected()){
            Quaternion q = new Quaternion();
            q.fromAngles(-FastMath.HALF_PI, FastMath.PI, 0);
            rootNode.setLocalRotation(q);
        }else{
            rootNode.setLocalRotation(new Quaternion());
        }
        
        rootNode.updateGeometricState(0, true);
    }//GEN-LAST:event_chkConvertActionPerformed
    
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
    private javax.swing.JButton btnLoad;
    private javax.swing.JToggleButton btnMaterial;
    private javax.swing.JToggleButton btnSolid;
    private javax.swing.JToggleButton btnTextured;
    private javax.swing.JToggleButton btnWire;
    private java.awt.Canvas canvas;
    private javax.swing.JCheckBox chkBackfaces;
    private javax.swing.JCheckBox chkBones;
    private javax.swing.JCheckBox chkBounds;
    private javax.swing.JCheckBox chkConvert;
    private javax.swing.JCheckBox chkNormals;
    private javax.swing.JPanel pnlButtons;
    private javax.swing.JSplitPane pnlSplit;
    private javax.swing.ButtonGroup shadingMode;
    // End of variables declaration//GEN-END:variables
    
}
