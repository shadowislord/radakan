package com.gibbon.jme.pass;

import com.gibbon.jme.context.JmeContext;
import com.jme.math.FastMath;
import com.jme.math.Vector3f;
import com.jme.renderer.Camera;
import com.jme.renderer.RenderContext;
import org.lwjgl.opengl.GL11;

/**
 * Stereo pass with assymetric frustum projection
 * and 3 stereo presentation modes (anaglyph, side by side, opengl stereo buffers)
 * 
 * To use stereo buffer mode, the display must be initialized with a 
 * stereo setting (requires dedicated hardware) and before passing GameSettings to the JmeContext, 
 * "GameStereo" must be set to "true".
 * 
 * @author Momoko_Fan
 */
public class StereoRenderPass extends RenderPass {

    protected float focalLength = 15f;
    
    /**
     * intraocular distance
     */
    protected float IOD = 1f;
    
    protected StereoMode mode = StereoMode.SIDE_BY_SIDE;
    protected ProjectionMode pMode = ProjectionMode.ASYMMETRIC_FRUSTUM;
    
    protected Vector3f savedCamLoc = new Vector3f(),
                       savedNodeLoc = new Vector3f();
    
    protected Vector3f temp = new Vector3f(),
                       temp2 = new Vector3f();
    
    protected enum CameraSide {
        LEFT,
        RIGHT
    }
    
    public enum StereoMode {
        /**
         * Stereo information is trasferred using seperate color channels
         * Left eye is sent with the red color channel, while the right eye is sent
         * as teal (green & blue) color channel.
         */
        ANAGLYPH,
        
        /**
         * Stereo information is transferred using OpenGL's dedicated stereo buffers.
         * Requires special hardware that supports stereo mode and driver, such as
         * nVidia consumer stereo driver.
         * In addition, the OpenGL context must be initialized in stereo mode,
         * set the "GameStereo" parameter on the GameSettings passed to JmeContext to "true".
         */
        STEREO_BUFFER,
        
        /**
         * Stereo information is rendered seperately to each part of the screen.
         * The left half of the display contains the left eye, while the right half contains the right eye.
         */
        SIDE_BY_SIDE
    }
    
    /**
     * The type of projection to use to create a stereo rendering.
     */
    public enum ProjectionMode{
        /**
         * Simple offset moves the camera a bit to the left and a bit to the right to create
         * the stereo effect. It's equivelant to putting two cameras parallel to each over side by side.
         */
        SIMPLE_OFFSET,
        
        /**
         * Assymetric frustum is a more advanced stereo projection mode that takes 
         * focus into account. It is a more accurate representation of how two eyes
         * would percieve the environment with a focus plane. Unfortunately it can also
         * cause more strain to the eye since it must adjust to the effects of simulating focus.
         */
        ASYMMETRIC_FRUSTUM
    }
    
    public StereoRenderPass(){
        super("StereoRender");
    }
    
    /**
     * Sets how the render pass transffers stereo information to the framebuffer.
     */
    public void setMode(StereoMode mode){
        this.mode = mode;
    }
    
    /**
     * Sets the method that the render pass uses to simulate stereo with two cameras.
     */
    public void setProjection(ProjectionMode mode){
        pMode = mode;
    }
    
    /**
     * Set the distance to where the two eyes focus
     */
    public void setFocusDistance(float dist){
        focalLength = dist;
    }
    
    public float getFocusDistance(){
        return focalLength;
    }
    
    /**
     * Set the distance between the stereo cameras
     */
    public void setEyeDistance(float dist){
        IOD = dist;
    }
    
    public float getEyeDistance(){
        return IOD;
    }
    
    /**
     * Sets up the camera frustum for a left or right eye.
     */
    protected void setFrustum(JmeContext cx, CameraSide side){
        Camera cam = cx.getRenderer().getCamera();
        
        float aspectratio = 0; 
        float aperture;
        
        float near = 0;
        float ndfl = 0;
        
        float widthdiv2 = 0;
        
        cam.getDirection().cross(cam.getUp(), temp);
        
        if (pMode == ProjectionMode.ASYMMETRIC_FRUSTUM){
            // Divide by 2 for side-by-side stereo
            aspectratio = cx.getRenderer().getWidth() / cx.getRenderer().getHeight();
            aperture = 45.0f;

            near = cam.getFrustumNear();
            ndfl = near / focalLength;

            // aperture in radians
            widthdiv2 = near * FastMath.tan((FastMath.DEG_TO_RAD * aperture) / 2.0f);
            
            temp.multLocal(IOD / 2.0f);
        }else{
            temp.multLocal(IOD * 4.0f);
        }
        
        if (side == CameraSide.RIGHT){
            if (pMode == ProjectionMode.ASYMMETRIC_FRUSTUM){
                float top = widthdiv2;
                float bottom = -widthdiv2;
                float left = (-aspectratio * widthdiv2) - (0.5f * IOD * ndfl);
                float right = (aspectratio * widthdiv2) - (0.5f * IOD * ndfl);

                cam.setFrustum(near, cam.getFrustumFar(), left, right, top, bottom);
            }
            
            cam.getLocation().addLocal(temp);
        }else{
            if (pMode == ProjectionMode.ASYMMETRIC_FRUSTUM){
                float top = widthdiv2;
                float bottom = -widthdiv2;
                float left = (-aspectratio * widthdiv2) + (0.5f * IOD * ndfl);
                float right = (aspectratio * widthdiv2) + (0.5f * IOD * ndfl);

                cam.setFrustum(near, cam.getFrustumFar(), left, right, top, bottom);
            }
            
            cam.getLocation().subtractLocal(temp);
        }
    }
    
    @Override
    public void runPass(JmeContext cx){
        doUpdate(cx);
        RenderContext<?> rc = cx.getRenderContext();
        applyPassStates(rc);
        cx.getRenderer().setPolygonOffset(zFactor, zOffset);
        
        Camera cam = cx.getRenderer().getCamera();

        // save the original camera location
        savedCamLoc.set(cam.getLocation());
        
        
        // LEFT EYE
        setFrustum(cx, CameraSide.LEFT);
        if (mode == StereoMode.SIDE_BY_SIDE)
            cam.setViewPort(0f, 0.5f, 0f, 1f);
        else if (mode == StereoMode.ANAGLYPH)
            GL11.glColorMask(true, false, false, true);
        else if (mode == StereoMode.STEREO_BUFFER)
            GL11.glDrawBuffer(GL11.GL_BACK_LEFT);
            
        cam.update();
        doRender(cx);
        // ****
        cx.getRenderer().clearZBuffer();
        
        // RIGHT EYE
        setFrustum(cx, CameraSide.RIGHT);
        if (mode == StereoMode.SIDE_BY_SIDE)
            cam.setViewPort(0.5f, 1.0f, 0f, 1f);
        else if (mode == StereoMode.ANAGLYPH)
            GL11.glColorMask(false, true, true, true);
        else if (mode == StereoMode.STEREO_BUFFER)
            GL11.glDrawBuffer(GL11.GL_BACK_RIGHT);
            
        cam.update();
        doRender(cx);
        
        cam.getLocation().set(savedCamLoc);
        
        if (mode == StereoMode.ANAGLYPH)
            GL11.glColorMask(true, true, true, true);
        else if (mode == StereoMode.SIDE_BY_SIDE)
            cam.setViewPort(0f, 1f, 0f, 1f);
        else if (mode == StereoMode.STEREO_BUFFER)
            GL11.glDrawBuffer(GL11.GL_BACK_LEFT);
    }
    
}
