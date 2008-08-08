package com.gibbon.jme;

import com.jme.math.FastMath;
import com.jme.math.Quaternion;
import com.jme.math.Vector3f;
import com.jme.scene.Spatial;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.StreamTokenizer;

/**
 * Loads MD5Camera files and creates a CameraAnimationController for it.
 * 
 * @author Momoko_Fan
 */
public class DoomCameraLoader {

    private CameraAnimationController camera;
    private CatmullRomCurve posCurve;
    private CameraFrame[] frames;
    private int frameRate;
    
    private transient int numCuts;
    private static final Quaternion conversion;
    
    static {
        conversion = new Quaternion();
        conversion.fromAngles(-FastMath.HALF_PI, FastMath.PI, 0);
    }
    
    /**
     * Represents a single frame of camera motion
     */
    private static class CameraFrame {
        Vector3f position = new Vector3f();
        Quaternion rotation = new Quaternion();
        float FoV;
        boolean isCut = false;
    }
    
    private void constructController(Spatial mover){
        Vector3f[] positions = new Vector3f[frames.length];
        Vector3f[] lefts = new Vector3f[frames.length];
        Vector3f[] ups = new Vector3f[frames.length];
        Vector3f[] directions = new Vector3f[frames.length];
        
        Vector3f[] axes = new Vector3f[3];
        
        for (int i = 0; i < frames.length; i++){
            positions[i] = new Vector3f(frames[i].position);
            //conversion.multLocal(positions[i]);

            //frames[i].rotation.fromAxes(lefts[i], ups[i], directions[i]);
            //frames[i].rotation.fromAxes(directions[i], ups[i], lefts[i]);
            frames[i].rotation.toAxes(axes);
            
            lefts[i] = new Vector3f(axes[0]);
            ups[i] = new Vector3f(axes[1]);
            directions[i] = new Vector3f(axes[2]);
        }
        
        posCurve = new CatmullRomCurve("MD5CameraPositions", positions);
        CatmullRomCurve leftCurve = new CatmullRomCurve("MD5CameraRotationLefts", lefts); 
        CatmullRomCurve upsCurve = new CatmullRomCurve("MD5CameraRotationUps", ups); 
        CatmullRomCurve dirCurve = new CatmullRomCurve("MD5CameraRotationDirections", directions); 
        
        if (mover != null){
            float maxTime = (1f/10f) * frames.length;
            camera = new CameraAnimationController(posCurve, 
                                                   upsCurve, 
                                                   leftCurve, 
                                                   dirCurve, 
                                                   mover, 
                                                   0f,
                                                   maxTime);
            mover.addController(camera);
        }
    }
    
    /**
     * Setup the parser to read the MD5 camera format
     * @param reader
     */
    private void setupReader(StreamTokenizer reader){
        reader.quoteChar('"');
        reader.ordinaryChar('{');
        reader.ordinaryChar('}');
        reader.ordinaryChar('(');
        reader.ordinaryChar(')');
        reader.parseNumbers();
        reader.slashSlashComments(true);
        reader.eolIsSignificant(true);
    }
    
    /**
     * Calculates the W value for a quaternion with only X, Y, and Z values.
     * As used in MD5 formats.
     * @param raw
     */
    private void processOrientation(Quaternion raw) {
        float t = 1.0f - (raw.x * raw.x) - (raw.y * raw.y) - (raw.z * raw.z);
        if (t < 0.0f) {
            raw.w = 0.0f;
        } else {
            raw.w = -(FastMath.sqrt(t));
        }
    }
    
    /**
     * Reads all the camera cuts
     * @param reader
     * @throws java.io.IOException
     */
    private void processCuts(StreamTokenizer reader) throws IOException{
        int cutIndex = 0;
        int type = -4;
        
        while(reader.nextToken() != '}' && cutIndex < numCuts) {
            type = reader.ttype;
	    switch (type) {
                case StreamTokenizer.TT_NUMBER:
                    frames[cutIndex].isCut = true;
                    break;
                case StreamTokenizer.TT_EOL:
                    cutIndex++;
                    break;
                default:
                    break;
            }
        }
    }
    
    /**
     * Writes a transform value to a frame
     * 
     * @param frame The frame index
     * @param index The value index
     * @param value The value
     */
    private void processTransform(int frame, int index, float value){
        switch (index) {
            case 0:
                frames[frame].position.x = value;
                break;
            case 1:
                frames[frame].position.y = value;
                break;
            case 2:
                frames[frame].position.z = value;
                break;
            case 3:
                frames[frame].rotation.x = value;
                break;
            case 4:
                frames[frame].rotation.y = value;
                break;
            case 5:
                frames[frame].rotation.z = value;
                processOrientation(frames[frame].rotation);
                break;
            default:
                break;
        }
    }
    
    private void processFrames(StreamTokenizer reader) throws IOException{
        int pointer = -1;
        int cameraIndex = -1;
        while (reader.nextToken() != '}') {
            switch (reader.ttype) {
                case '(':
                    while (reader.nextToken() != ')') {
                        processTransform(cameraIndex, pointer, (float) reader.nval);
                        pointer++;
                    }
                    break;
                case StreamTokenizer.TT_NUMBER:
                    if (pointer == 6){
                        frames[cameraIndex].FoV = (float) reader.nval;
                    }
                    break;
                case StreamTokenizer.TT_EOL:
                    pointer = 0;
                    cameraIndex++;
                    break;
                default:
                    break;
                }
        }
    }
    
    /**
     * Loads camera motion data from a streamtokenizer
     * @param reader
     * @throws java.io.IOException
     */
    private void processCamera(StreamTokenizer reader) throws IOException{
        String sval = null;
        while(reader.nextToken() != StreamTokenizer.TT_EOF) {
            sval = reader.sval;
            if (sval != null) {
                if (sval.equals("MD5Version")) {
                    reader.nextToken();
                    if (reader.nval != 10) {
                        throw new IOException("MD5Version "+(int)reader.nval+" not supported!");
                    }
                } else if (sval.equals("numFrames")) {
                    reader.nextToken();
                    frames = new CameraFrame[(int)reader.nval];
                    for (int i = 0; i < frames.length; i++)
                        frames[i] = new CameraFrame();
                } else if (sval.equals("numCuts")) {
                    reader.nextToken();
                    numCuts = (int)reader.nval;
                } else if (sval.equals("frameRate")){
                    reader.nextToken();
                    frameRate = (int)reader.nval;
                } else if (sval.equals("cuts")) {
                    reader.nextToken();
                    processCuts(reader);
                } else if (sval.equals("camera")) {
                    reader.nextToken();
                    processFrames(reader);
                }
            }
        }
    }
    
    /**
     * Load camera motion data from an inputstream
     * @param in
     */
    public void load(InputStream in, Spatial mover) throws IOException{
        StreamTokenizer reader = new StreamTokenizer(new InputStreamReader(in));
        setupReader(reader);
        processCamera(reader);
        constructController(mover);
    }
    
    public CameraAnimationController getCameraController(){
        return camera;
    }
    
    public CatmullRomCurve getPositionCurve(){
       return posCurve;
    }
    
}
