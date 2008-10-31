package com.radakan.jme.hdrreader;

import com.jme.image.Image;
import com.jme.image.Image.Format;
import com.jme.math.FastMath;
import com.jme.math.Vector2f;
import com.jme.math.Vector3f;
import com.jme.util.geom.BufferUtils;
import java.nio.ByteBuffer;
import java.nio.FloatBuffer;
import org.lwjgl.opengl.GL11;
import org.lwjgl.util.glu.GLU;

public class ToneMapper {

    public static void readFloatPixel(Image img, int x, int y, Vector3f vec){
        FloatBuffer data = img.getData(0).asFloatBuffer();
        
        vec.x = data.get((y * img.getWidth() + x) * 3);
        vec.y = data.get(((y * img.getWidth() + x) * 3) + 1);
        vec.z = data.get(((y * img.getWidth() + x) * 3) + 2);
    }
    
    public static void writeFloatPixel(Image img, int x, int y, Vector3f vec){
        FloatBuffer data = img.getData(0).asFloatBuffer();
        
        data.put(((y * img.getWidth() + x) * 3), vec.x);
        data.put((((y * img.getWidth() + x) * 3) + 1), vec.y);
        data.put((((y * img.getWidth() + x) * 3) + 2), vec.z);
    }
    
    public static void writeRGBPixel(Image img, int x, int y, Vector3f vec){
        ByteBuffer data = img.getData(0);
        
        float R = FastMath.clamp(vec.x, 0f, 1f) * 255f;
        float G = FastMath.clamp(vec.y, 0f, 1f) * 255f;
        float B = FastMath.clamp(vec.z, 0f, 1f) * 255f;
        
        data.put(((y * img.getWidth() + x) * 3), (byte)(R));
        data.put((((y * img.getWidth() + x) * 3) + 1), (byte)(G));
        data.put((((y * img.getWidth() + x) * 3) + 2), (byte)(B));
    }
    
    public static int clamp(int i, int max){
        if (i < 0) return 0;
        else if (i > max) return max;
        return i;
    }
    
    public static float lum(Vector3f color){
        return color.dot(new Vector3f(.2125f, .7154f, .0721f));
    }
    
    private static final float[] PixelWeight =new float[]{ 0.2537f, 
                                                           0.2185f, 
                                                           0.0821f, 
                                                           0.0461f, 
                                                           0.0262f, 
                                                           0.0162f, 
                                                           0.0102f, 
                                                           0.0052f };
    
    public static Image logLum32F(Image img){
        ByteBuffer bb = BufferUtils.createByteBuffer(img.getData(0).capacity());
        Image img2 = new Image(Format.RGB32F, img.getWidth(), img.getHeight(), bb);
        
        Vector3f pixel = new Vector3f();
        
        for (int y = 0; y < img.getHeight(); y++){
            for (int x = 0; x < img.getWidth(); x++){
                readFloatPixel(img, x, y, pixel);
                //pixel.multLocal(PixelWeight[0]);
                //pixel.set(0, 0, 0);
                float lum = (float) Math.log(.0001f + lum(pixel));
                pixel.set(lum, lum, lum);
                
                writeFloatPixel(img2, x, y, pixel);
            }
        }
        
        return img2;
    }
    
    public static Image convert32FtoRGB8(Image img){
        ByteBuffer bb = BufferUtils.createByteBuffer(img.getData(0).capacity() / 4);
        Image img2 = new Image(Format.RGB8, img.getWidth(), img.getHeight(), bb);
        
        Vector3f pixel = new Vector3f();
        for (int y = 0; y < img.getHeight(); y++){
            for (int x = 0; x < img.getWidth(); x++){
                readFloatPixel(img, x, y, pixel);
                writeRGBPixel(img2, x, y, pixel);
            }
        }
        
        return img2;
    }
    
    /**
     * Finds average luminance of a RGB32F image.
     * 
     * First the logarithmic luminance is calculated for each pixel,
     * then the image is scaled to 64x64, then to 4x4, the pixels are averaged
     * to get average logarithmic luminance. 
     * 
     * @param img
     * @return
     */
    public static float logLumAvg32F(Image img){
        Image logLum = logLum32F(img);
        Image logLumRGB8 = convert32FtoRGB8(logLum);
        
        ByteBuffer bb = BufferUtils.createByteBuffer(64 * 64 * 3);
        Image logLumRGB8_64 = new Image(Format.RGB8, 64, 64, bb);
        
        int r = GLU.gluScaleImage(GL11.GL_RGB, 
                          logLum.getWidth(), 
                          logLum.getHeight(), 
                          GL11.GL_UNSIGNED_BYTE, 
                          logLumRGB8.getData(0), 
                          64, 
                          64, 
                          GL11.GL_UNSIGNED_BYTE, 
                          bb);
        
        if (r != 0) return -1f;
        
        ByteBuffer bb2 = BufferUtils.createByteBuffer(4 * 4 * 3);
        Image logLumRGB8_4 = new Image(Format.RGB8, 4, 4, bb2);
        
        r = GLU.gluScaleImage(GL11.GL_RGB, 
                          64, 
                          64, 
                          GL11.GL_UNSIGNED_BYTE, 
                          bb, 
                          4, 
                          4, 
                          GL11.GL_UNSIGNED_BYTE, 
                          bb2);
        
        if (r != 0) return -1f;
        
        float avg = 0f;
        byte[] color = new byte[3];
        for (int y = 0; y < 4; y++){
            for (int x = 0; x < 4; x++){
                
                color[0] = bb2.get( (((y * 4 + x) * 3) + 0) );
                //color[1] = bb2.get( (((y * 4 + x) * 3) + 1) );
                //color[2] = bb2.get( (((y * 4 + x) * 3) + 2) );
               
                avg += (color[0] & 0xFF) / 255f;
            }
        }
        
        return (float) Math.exp(avg / (4f * 4f));
    }
    
    public static Image blur32FImage(Image img){
        ByteBuffer bb = BufferUtils.createByteBuffer(img.getData(0).capacity());
        Image img2 = new Image(Format.RGB32F, img.getWidth(), img.getHeight(), bb);
        
        Vector3f pixel = new Vector3f();
        Vector3f temp = new Vector3f();
        
        for (int y = 0; y < img.getHeight(); y++){
            for (int x = 0; x < img.getWidth(); x++){
                readFloatPixel(img, x, y, pixel);
                pixel.multLocal(PixelWeight[0]);
                //pixel.set(0, 0, 0);
                for (int i = 1; i < 8; i++){
                    readFloatPixel(img, clamp(x + i, img.getWidth()-1), y, temp);
                    temp.multLocal(PixelWeight[i]);
                    pixel.addLocal(temp);
                    
                    readFloatPixel(img, clamp(x - i, img.getWidth()-1), y, temp);
                    temp.multLocal(PixelWeight[i]);
                    pixel.addLocal(temp);
                }
                writeFloatPixel(img2, x, y, pixel);
            }
        }
        
        bb = BufferUtils.createByteBuffer(img.getData(0).capacity());
        Image img3 = new Image(Format.RGB32F, img.getWidth(), img.getHeight(), bb);
        
        for (int y = 0; y < img2.getHeight(); y++){
            for (int x = 0; x < img2.getWidth(); x++){
                readFloatPixel(img2, x, y, pixel);
                pixel.multLocal(PixelWeight[0]);
                //pixel.set(0, 0, 0);
                for (int i = 1; i < 8; i++){
                    readFloatPixel(img2, x, clamp(y + i, img.getHeight()-1), temp);
                    temp.multLocal(PixelWeight[i]);
                    pixel.addLocal(temp);
                    
                    readFloatPixel(img2, x, clamp(y - i, img.getHeight()-1), temp);
                    temp.multLocal(PixelWeight[i]);
                    pixel.addLocal(temp);
                }
                writeFloatPixel(img3, x, y, pixel);
            }
        }
        
        return img3;
    }
    
    public static final Image toneMap(Image img, float exposure){
        Image blurred = blur32FImage(img);
        
        float avgLum = logLumAvg32F(img);
        
        ByteBuffer bb = BufferUtils.createByteBuffer(img.getData(0).capacity() / 4);
        Image img2 = new Image(Format.RGB8, img.getWidth(), img.getHeight(), bb);
        
        Vector3f pixel = new Vector3f();
        Vector3f blurPixel = new Vector3f();
        
        Vector3f temp = new Vector3f();
        Vector3f temp2 = new Vector3f();
        
        for (int y = 0; y < img2.getHeight(); y++){
            for (int x = 0; x < img2.getWidth(); x++){
                readFloatPixel(img, x, y, pixel);
                readFloatPixel(blurred, x, y, blurPixel);
                
                // op1
                temp.set(pixel);
                float mult = exposure / (avgLum + 0.0001f);
                temp.multLocal(mult);
                
                // op2
                temp2.set(pixel);
                temp2.divideLocal(8f);
                temp2.addLocal(1f,1f,1f);
                temp.multLocal(temp2);
                
                // op3
                temp2.set(pixel);
                temp2.addLocal(1f,1f,1f);
                temp.divideLocal(temp2);
                
                // op4
                temp2.set(blurPixel);
                temp2.multLocal(exposure / 12f);
                temp.addLocal(temp2);
                
                writeRGBPixel(img2, x, y, temp);
            }
        }
        
        return img2;
    }
    
}
