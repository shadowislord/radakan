package com.gibbon.jme.context;


import com.gibbon.jme.context.lwjgl.LWJGLCanvas;
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
import java.awt.Dimension;
import java.awt.Toolkit;
import javax.swing.JFrame;

public class View3D extends javax.swing.JDialog {
    
    private int width, height;
    private JmeContext context;
    
    public View3D(JFrame parent, int width, int height) {
        super(parent, false);
        this.width=width;
        this.height=height;
        initComponents();
    }
    
    public JmeContext getContext(){
        return context;
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
    
    /** This method is called from within the constructor to
     * initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is
     * always regenerated by the Form Editor.
     */
    // <editor-fold defaultstate="collapsed" desc=" Generated Code ">                          
    private void initComponents() {
        gl = createCanvas();

        setDefaultCloseOperation(javax.swing.WindowConstants.DO_NOTHING_ON_CLOSE);
        setFocusable(false);
        setFocusableWindowState(false);
        setName("view3d");
        setResizable(false);
        setUndecorated(true);

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(gl, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.DEFAULT_SIZE, 400, Short.MAX_VALUE)
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(gl, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.DEFAULT_SIZE, 300, Short.MAX_VALUE)
        );
        pack();
    }// </editor-fold>                        
    
    protected Canvas canvas;

    protected Canvas createCanvas(){
        try {
            if (JmeContext.get()!=null){
                // share with the previous context
                context = JmeContext.create(JmeContext.getDefaultImplementorClassName(),
                                            JmeContext.CONTEXT_CANVAS,
                                            JmeContext.get());
            }else{
                context = JmeContext.create(JmeContext.getDefaultImplementorClassName(),
                                            JmeContext.CONTEXT_CANVAS);
            }
        } catch (InstantiationException ex) {
            ex.printStackTrace();
            return null;
        }
        
        context.start();
        canvas = context.getCanvas();
        return canvas;
    }
    
    public static void main(String[] args){
        Dimension d = Toolkit.getDefaultToolkit().getScreenSize();

        View3D view1 = new View3D(null,320,240);
        View3D view2 = new View3D(null,320,240);
        
        view1.setLocation((int)(d.getWidth() / 2 - view1.getWidth()) - 20, (int)(d.getHeight() / 2 - view1.getHeight() / 2));
        view1.setVisible(true);

        view2.setLocation((int)(d.getWidth() / 2) + 20, (int)(d.getHeight() / 2 - view2.getHeight() / 2));
        view2.setVisible(true);
        
        JmeContext context = JmeContext.get();
        try {
            // makes sure DisplaySystem.getDisplaySystem().getRenderer() is not null
            context.waitFor();
        } catch (InterruptedException ex) {
            ex.printStackTrace();
        }
        
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
        pl.setLocation(new Vector3f(-5,5,5));
        ls.attach(pl);
        root.setRenderState(ls);

        ZBufferState buf = context.getRenderer().createZBufferState();
        buf.setFunction(TestFunction.LessThanOrEqualTo);
        buf.setWritable(true);
        root.setRenderState(buf);

        TextureState ts = JmeContext.get().getRenderer().createTextureState();
        Texture t = TextureManager.loadTexture(
                        View3D.class.getClassLoader().getResource("jmetest/data/images/Monkey.jpg"));
        ts.setTexture(t);
        root.setRenderState(ts);

        root.updateRenderState();
        root.updateGeometricState(0,true);
        
        view1.getContext().getPassManager().add(new RotatingBox(root,ColorRGBA.red,true));
        view2.getContext().getPassManager().add(new RotatingBox(root,ColorRGBA.green,false));
    }
    
    // Variables declaration - do not modify                     
    private java.awt.Canvas gl;
    // End of variables declaration                   
    
}