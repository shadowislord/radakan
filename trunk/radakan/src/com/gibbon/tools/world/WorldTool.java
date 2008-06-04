package com.gibbon.tools.world;

import com.gibbon.jme.context.JmeContext;
import com.gibbon.jme.context.RenderPass;
import com.gibbon.jme.context.lwjgl.LWJGLCanvas;
import com.gibbon.jme.context.lwjgl.LWJGLContext;
import com.gibbon.radakan.error.ErrorReporter;
import com.jme.bounding.BoundingBox;
import com.jme.light.DirectionalLight;
import com.jme.light.PointLight;
import com.jme.math.Vector3f;
import com.jme.renderer.AbstractCamera;
import com.jme.renderer.Camera;
import com.jme.renderer.ColorRGBA;
import com.jme.scene.Node;
import com.jme.scene.shape.Quad;
import com.jme.scene.state.CullState;
import com.jme.scene.state.CullState.Face;
import com.jme.scene.state.LightState;
import com.jme.scene.state.ZBufferState;
import com.jme.scene.state.ZBufferState.TestFunction;
import java.util.concurrent.Callable;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.JPopupMenu;

public class WorldTool extends javax.swing.JFrame {
    
    private static WorldTool instance;
    private EditorState state;
    private LWJGLCanvas glCanvas;
    private JmeContext context;
    
    public static Quad lilPlane = new Quad("quad", 4, 4);
    private PointLight camLight;
    private Node rootNode = new Node("rootNode");
    private WorldCameraHandler handler;
    private RenderPass render;
    
    private boolean doingMouseAction = false;
    private int mouseX = 0, mouseY = 0;
    
    public static void setMouseXY(int x, int y){
        instance.mouseX = x;
        instance.mouseY = y;
    }
    
    public static void setDoingMouseAction(int x, int y){
        instance.mouseX = x;
        instance.mouseY = y;
        instance.doingMouseAction = true;
    }
    
    public static void setNotDoingMouseAction(){
        instance.doingMouseAction = false;
    }
    
    public WorldTool() {
        instance = this;
        
        Logger.getLogger("").setLevel(Level.WARNING);
        
        state = EditorState.getState();
        initComponents();
        
        setLocationRelativeTo(null);
        
        new Thread(){
            @Override
            public void run(){
                try {
                    context.waitFor();

                    // create the model camera handler for the mouse
                    handler = new WorldCameraHandler(context.getRenderer());
                    glCanvas.addMouseWheelListener(handler);
                    glCanvas.addMouseListener(handler);
                    glCanvas.addMouseMotionListener(handler);

                    // create the render pass
                    render = new RenderPass(){
                        @Override
                        public void doUpdate(JmeContext cx){
                            Camera cam = cx.getRenderer().getCamera();
        
                            Vector3f temp = camLight.getLocation();
                            temp.set(cam.getLeft()).multLocal(5.0f);
                            temp.addLocal(cam.getLocation());
                            camLight.setLocation(temp);
                            
                            if (doingMouseAction){
                                TerrainBrush.doMouseAction(mouseX, mouseY);
                            }
                        }
                    };
                    render.add(rootNode);
                    context.getPassManager().add(render);
                    
                    // set some default states for the root node
                    ZBufferState zbuf = context.getRenderer().createZBufferState();
                    zbuf.setFunction(TestFunction.LessThan);
                    zbuf.setWritable(true);
                    rootNode.setRenderState(zbuf);
                    
                    CullState backFaces = context.getRenderer().createCullState();
                    backFaces.setCullFace(Face.Back);
                    rootNode.setRenderState(backFaces);
                    
                    LightState ls = context.getRenderer().createLightState();
                    ls.setSeparateSpecular(true);
                    
                    DirectionalLight dl = new DirectionalLight();
                    dl.setEnabled(true);
                    dl.setDirection(new Vector3f(0,0,-1).normalizeLocal());
                    dl.setDiffuse(new ColorRGBA(0.6f, 0.6f, 0.3f, 1.0f));
                    ls.attach(dl);

                    DirectionalLight dl2 = new DirectionalLight();
                    dl2.setEnabled(true);
                    dl.setDirection(new Vector3f(1,0.1f,0).normalizeLocal());
                    dl.setDiffuse(new ColorRGBA(0.4f, 0.4f, 0.8f, 1.0f));
                    ls.attach(dl2);
                    
                    camLight = new PointLight();
                    camLight.setEnabled(true);
                    camLight.setDiffuse(ColorRGBA.lightGray);
                    camLight.setSpecular(ColorRGBA.white);
                    ls.attach(camLight);
                    rootNode.setRenderState(ls);
                    
                    lilPlane.setDefaultColor(ColorRGBA.red);
                    rootNode.attachChild(lilPlane);
                    
                    rootNode.updateGeometricState(0.0f, true);
                    rootNode.updateRenderState();
                    
                    // put the camera above the terrain and face the center
                    final AbstractCamera cam = (AbstractCamera) JmeContext.get().getRenderer().getCamera();
                    cam.setLocation(new Vector3f(0, 50, -50));
                    cam.lookAt(Vector3f.ZERO, Vector3f.UNIT_Y);
                    
                    context.executeLater(new Callable<Object>(){
                        public Object call(){
                            cam.getProjectionMatrix();
                            cam.getModelViewMatrix();
                            cam.update();
                            return null;
                        }
                    });
                    
                } catch (Throwable ex) {
                    ErrorReporter.reportError("Error while initializing model view", ex);
                }
            }
        }.start();
    }
    
    private LWJGLCanvas createCanvas() {
        try {
            context = JmeContext.create(LWJGLContext.class, JmeContext.CONTEXT_CANVAS);
            context.start();
            glCanvas = (LWJGLCanvas) context.getCanvas();
            return glCanvas;
        } catch (Throwable ex) {
            ErrorReporter.reportError("Error occured while initializing 3D canvas", ex);
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

        brushTypeGroup = new javax.swing.ButtonGroup();
        split = new javax.swing.JSplitPane();
        canvas = createCanvas();
        tab = new javax.swing.JTabbedPane();
        pnlTerrain = new javax.swing.JPanel();
        pnlTerrainBrush = new javax.swing.JPanel();
        radRaise = new javax.swing.JRadioButton();
        radLower = new javax.swing.JRadioButton();
        sldBrushSize = new javax.swing.JSlider();
        lblTerrainBSize = new javax.swing.JLabel();
        pnlTexture = new javax.swing.JPanel();
        pnlTexBrush = new javax.swing.JPanel();
        jScrollPane1 = new javax.swing.JScrollPane();
        lstTex = new javax.swing.JList();
        btnTexAdd = new javax.swing.JButton();
        btnTexRemove = new javax.swing.JButton();
        menu = new javax.swing.JMenuBar();
        menuFile = new javax.swing.JMenu();
        menuFileNew = new javax.swing.JMenuItem();
        menuFileSep1 = new javax.swing.JSeparator();
        menuFileExit = new javax.swing.JMenuItem();

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);
        setTitle("WorldTool");

        split.setDividerLocation(400);
        split.setContinuousLayout(true);

        canvas.setBackground(new java.awt.Color(0, 0, 0));
        split.setLeftComponent(canvas);

        tab.addChangeListener(new javax.swing.event.ChangeListener() {
            public void stateChanged(javax.swing.event.ChangeEvent evt) {
                tabStateChanged(evt);
            }
        });

        pnlTerrainBrush.setBorder(javax.swing.BorderFactory.createTitledBorder("Brush"));

        brushTypeGroup.add(radRaise);
        radRaise.setSelected(true);
        radRaise.setText("Raise");
        radRaise.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                radRaiseActionPerformed(evt);
            }
        });

        brushTypeGroup.add(radLower);
        radLower.setText("Lower");
        radLower.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                radLowerActionPerformed(evt);
            }
        });

        org.jdesktop.layout.GroupLayout pnlTerrainBrushLayout = new org.jdesktop.layout.GroupLayout(pnlTerrainBrush);
        pnlTerrainBrush.setLayout(pnlTerrainBrushLayout);
        pnlTerrainBrushLayout.setHorizontalGroup(
            pnlTerrainBrushLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(pnlTerrainBrushLayout.createSequentialGroup()
                .addContainerGap()
                .add(pnlTerrainBrushLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                    .add(radRaise)
                    .add(radLower))
                .addContainerGap(106, Short.MAX_VALUE))
        );
        pnlTerrainBrushLayout.setVerticalGroup(
            pnlTerrainBrushLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(pnlTerrainBrushLayout.createSequentialGroup()
                .add(radRaise)
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(radLower)
                .addContainerGap(61, Short.MAX_VALUE))
        );

        sldBrushSize.setMaximum(30);
        sldBrushSize.setMinimum(1);
        sldBrushSize.setValue(5);
        sldBrushSize.addChangeListener(new javax.swing.event.ChangeListener() {
            public void stateChanged(javax.swing.event.ChangeEvent evt) {
                sldBrushSizeStateChanged(evt);
            }
        });

        lblTerrainBSize.setText("Brush size: ");

        org.jdesktop.layout.GroupLayout pnlTerrainLayout = new org.jdesktop.layout.GroupLayout(pnlTerrain);
        pnlTerrain.setLayout(pnlTerrainLayout);
        pnlTerrainLayout.setHorizontalGroup(
            pnlTerrainLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(pnlTerrainLayout.createSequentialGroup()
                .addContainerGap()
                .add(pnlTerrainLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                    .add(pnlTerrainBrush, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .add(pnlTerrainLayout.createSequentialGroup()
                        .add(lblTerrainBSize, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 65, Short.MAX_VALUE)
                        .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                        .add(sldBrushSize, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, 110, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)))
                .addContainerGap())
        );
        pnlTerrainLayout.setVerticalGroup(
            pnlTerrainLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(pnlTerrainLayout.createSequentialGroup()
                .addContainerGap()
                .add(pnlTerrainBrush, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
                .add(15, 15, 15)
                .add(pnlTerrainLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                    .add(lblTerrainBSize)
                    .add(sldBrushSize, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE))
                .addContainerGap(221, Short.MAX_VALUE))
        );

        tab.addTab("Terrain", pnlTerrain);

        pnlTexBrush.setBorder(javax.swing.BorderFactory.createTitledBorder("Brush"));

        lstTex.setModel(new javax.swing.AbstractListModel() {
            String[] strings = { "Item 1", "Item 2", "Item 3", "Item 4", "Item 5" };
            public int getSize() { return strings.length; }
            public Object getElementAt(int i) { return strings[i]; }
        });
        jScrollPane1.setViewportView(lstTex);

        btnTexAdd.setText("Add");

        btnTexRemove.setText("Remove");

        org.jdesktop.layout.GroupLayout pnlTexBrushLayout = new org.jdesktop.layout.GroupLayout(pnlTexBrush);
        pnlTexBrush.setLayout(pnlTexBrushLayout);
        pnlTexBrushLayout.setHorizontalGroup(
            pnlTexBrushLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(pnlTexBrushLayout.createSequentialGroup()
                .addContainerGap()
                .add(pnlTexBrushLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                    .add(pnlTexBrushLayout.createSequentialGroup()
                        .add(btnTexAdd)
                        .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                        .add(btnTexRemove))
                    .add(jScrollPane1, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 147, Short.MAX_VALUE))
                .addContainerGap())
        );
        pnlTexBrushLayout.setVerticalGroup(
            pnlTexBrushLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(pnlTexBrushLayout.createSequentialGroup()
                .add(jScrollPane1, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 130, Short.MAX_VALUE)
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(pnlTexBrushLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.BASELINE)
                    .add(btnTexAdd)
                    .add(btnTexRemove))
                .addContainerGap())
        );

        org.jdesktop.layout.GroupLayout pnlTextureLayout = new org.jdesktop.layout.GroupLayout(pnlTexture);
        pnlTexture.setLayout(pnlTextureLayout);
        pnlTextureLayout.setHorizontalGroup(
            pnlTextureLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(pnlTextureLayout.createSequentialGroup()
                .addContainerGap()
                .add(pnlTexBrush, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addContainerGap())
        );
        pnlTextureLayout.setVerticalGroup(
            pnlTextureLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(pnlTextureLayout.createSequentialGroup()
                .addContainerGap()
                .add(pnlTexBrush, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
                .addContainerGap(196, Short.MAX_VALUE))
        );

        tab.addTab("Texture", pnlTexture);

        split.setRightComponent(tab);

        menuFile.setText("File");

        menuFileNew.setText("New");
        menuFileNew.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                menuFileNewActionPerformed(evt);
            }
        });
        menuFile.add(menuFileNew);
        menuFile.add(menuFileSep1);

        menuFileExit.setText("Exit");
        menuFileExit.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                menuFileExitActionPerformed(evt);
            }
        });
        menuFile.add(menuFileExit);

        menu.add(menuFile);

        setJMenuBar(menu);

        org.jdesktop.layout.GroupLayout layout = new org.jdesktop.layout.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(split, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 610, Short.MAX_VALUE)
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(org.jdesktop.layout.GroupLayout.TRAILING, split, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 434, Short.MAX_VALUE)
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

    private void updateEditorState(){
        switch (tab.getSelectedIndex()){
            case 0: 
                state.editType = EditType.TERRAIN;
                break;
            default:
                throw new RuntimeException("Unsupported operation");
        }
        
        if (state.editType == EditType.TERRAIN){
            state.brushSize = sldBrushSize.getValue();
            if (radRaise.isSelected()){
                state.brushType = BrushType.RAISE;
            }else if (radLower.isSelected()){
                state.brushType = BrushType.LOWER;
            }
        }
    }
    
    private void tabStateChanged(javax.swing.event.ChangeEvent evt) {//GEN-FIRST:event_tabStateChanged
        updateEditorState();
}//GEN-LAST:event_tabStateChanged

    private void radRaiseActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_radRaiseActionPerformed
        updateEditorState();
    }//GEN-LAST:event_radRaiseActionPerformed

    private void radLowerActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_radLowerActionPerformed
        updateEditorState();
    }//GEN-LAST:event_radLowerActionPerformed

    private void sldBrushSizeStateChanged(javax.swing.event.ChangeEvent evt) {//GEN-FIRST:event_sldBrushSizeStateChanged
        updateEditorState();
    }//GEN-LAST:event_sldBrushSizeStateChanged

    private void menuFileExitActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_menuFileExitActionPerformed
        dispose();
        context.dispose();
    }//GEN-LAST:event_menuFileExitActionPerformed

    private void menuFileNewActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_menuFileNewActionPerformed
        Vector3f scale = new Vector3f(2f, 2f, 2f);
        
        state.terrain = WorldUtil.createGrid(64, 64, scale, null);
        rootNode.attachChild(state.terrain);
        rootNode.setModelBound(new BoundingBox());
        rootNode.updateModelBound();
        
        rootNode.updateGeometricState(0.0f, true);
        rootNode.updateRenderState();
}//GEN-LAST:event_menuFileNewActionPerformed
    
    /**
     * @param args the command line arguments
     */
    public static void main(String args[]) {
        JPopupMenu.setDefaultLightWeightPopupEnabled(false);
        
        java.awt.EventQueue.invokeLater(new Runnable() {
            public void run() {
                new WorldTool().setVisible(true);
            }
        });
    }
    
    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.ButtonGroup brushTypeGroup;
    private javax.swing.JButton btnTexAdd;
    private javax.swing.JButton btnTexRemove;
    private java.awt.Canvas canvas;
    private javax.swing.JScrollPane jScrollPane1;
    private javax.swing.JLabel lblTerrainBSize;
    private javax.swing.JList lstTex;
    private javax.swing.JMenuBar menu;
    private javax.swing.JMenu menuFile;
    private javax.swing.JMenuItem menuFileExit;
    private javax.swing.JMenuItem menuFileNew;
    private javax.swing.JSeparator menuFileSep1;
    private javax.swing.JPanel pnlTerrain;
    private javax.swing.JPanel pnlTerrainBrush;
    private javax.swing.JPanel pnlTexBrush;
    private javax.swing.JPanel pnlTexture;
    private javax.swing.JRadioButton radLower;
    private javax.swing.JRadioButton radRaise;
    private javax.swing.JSlider sldBrushSize;
    private javax.swing.JSplitPane split;
    private javax.swing.JTabbedPane tab;
    // End of variables declaration//GEN-END:variables
    
}
