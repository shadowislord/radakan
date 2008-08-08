package com.gibbon.tools.world;

import com.jme.image.Image;
import com.jme.image.Image.Format;
import com.jme.image.Texture;
import com.jme.image.Texture2D;
import com.jme.math.Vector2f;
import com.jme.math.Vector3f;
import com.jme.renderer.ColorRGBA;
import com.jme.scene.TriMesh;
import com.jme.util.lwjgl.LWJGLTextureUpdater;
import java.nio.ByteBuffer;

public class TextureBrush {

    public static float byte2float(byte b){
        return ((float)(b & 0xFF)) / 255f;
    }
    
    public static byte float2byte(float f){
        return (byte) (f * 255f);
    }
    
    public static float short2float(short s){
        return ((float)((int)s & 0xFFFF)) / 65535f;
    }
    
    public static short float2short(float f){
        return (short) (f * 65535f);
    }
    
    public static void manipulatePixel(Image image, int x, int y, ColorRGBA color, boolean write){
        ByteBuffer buf = image.getData(0);
        int width = image.getWidth();
        
        if (write){
            switch (image.getFormat()){
                case Intensity8:
                case Alpha8:
                case Luminance8:
                    buf.position( (y * width + x) );
                    buf.put(float2byte((color.r + color.g + color.b) * color.a / 3.0f ));
                    return;
                case RGB8:
                    buf.position( (y * width + x) * 3 );
                    buf.put(float2byte(color.r * color.a))
                       .put(float2byte(color.g * color.a))
                       .put(float2byte(color.b * color.a));
                    return;
                case RGB16:
                    buf.position( (y * width + x) * 6 );
                    buf.putShort(float2short(color.r * color.a))
                       .putShort(float2short(color.g * color.a))
                       .putShort(float2short(color.b * color.a));
                    return;
                case RGBA8:
                    buf.position( (y * width + x) * 4 );
                    buf.put(float2byte(color.r))
                       .put(float2byte(color.g))
                       .put(float2byte(color.b))
                       .put(float2byte(color.a));
                    return;
                case RGB32F:
                    buf.position( (y * width + x) * 12 );
                    buf.putFloat(color.r * color.a)
                       .putFloat(color.g * color.a)
                       .putFloat(color.b * color.a);
                    return;
                default:
                    throw new UnsupportedOperationException();
            }
        }else{
            switch (image.getFormat()){
                case Intensity8:
                case Alpha8:
                case Luminance8:
                    buf.position( (y * width + x) );
                    float f = byte2float(buf.get());
                    color.set(f, f, f, 1.0f);
                    return;
                case RGB8:
                    buf.position( (y * width + x) * 3 );
                    color.set(byte2float(buf.get()), byte2float(buf.get()), byte2float(buf.get()), 1.0f);
                    return;
                case RGB16:
                    buf.position( (y * width + x) * 6 );
                    color.set(short2float(buf.getShort()), short2float(buf.getShort()), short2float(buf.getShort()), 1.0f);
                    return;
                case RGBA8:
                    buf.position( (y * width + x) * 4 );
                    color.set(byte2float(buf.get()), byte2float(buf.get()), byte2float(buf.get()), byte2float(buf.get()));
                    return;
                case RGB32F:
                    buf.position( (y * width + x) * 12 );
                    color.set(buf.getFloat(), buf.getFloat(), buf.getFloat(), 1.0f);
                    return;
                default:
                    throw new UnsupportedOperationException();
            }
        }
    }
    
    public static byte addByteClamp(byte a, byte b){
        if ( ((int)a + (int)b) > 127 )
            return 127;
        else
            return (byte) (a + b);
    }
    
    public static void doPaintAction(Image image, Vector2f uv, float radius, float intensity){
        Vector2f texuv = new Vector2f();
        ColorRGBA color = new ColorRGBA();
        
        int width = image.getWidth();
        int height = image.getHeight();
        
        for (int y = 0; y < height; y++){
            for (int x = 0; x < width; x++){
                manipulatePixel(image, x, y, color, false);
                
                texuv.set((float)x / width, (float)y / height);

                float dist = texuv.distanceSquared(uv);
                if (dist < radius * radius) {
                    dist = 1.5f - (dist / radius);
                    float amount = intensity * dist;
                    if ( (amount > 0.0 && intensity > 0.0) ||
                         (amount < 0.0 && intensity < 0.0)) {
                        color.r += amount;
                        color.g += amount;
                        color.b += amount;
                    }

                    color.clamp();
                    
//                    if (intensity > 0.0){
//                        color.set(1.0f, 1.0f, 1.0f, 1.0f);
//                    }else{
//                        color.set(0.0f, 0.0f, 0.0f, 0.0f);
//                    }
                }
                
                manipulatePixel(image, x, y, color, true);
            }
        }
         
        image.getData(0).rewind();
    }
    
    public static void doMouseAction(int x, int y, boolean drag, boolean finish) {
        EditorState state = EditorState.getState();
        
        Vector3f worldCoords = new Vector3f();
        TriMesh collided = PickUtils.findClickedObject(x, y, true, worldCoords, null);
        
        if (collided == null || !collided.getName().startsWith("TERRAIN"))
            return;
            
        TileGroup group = (TileGroup) collided.getParent().getParent();
        final float groupWidth = World.getWorld().getGroupSize() * World.getWorld().getTileSize();
        
        float brushSize = (float)state.brushSize / groupWidth;
        
        // aquire UV coords for texture
        Vector2f UV = new Vector2f();
        UV.set(-worldCoords.x, -worldCoords.z);
        UV.subtractLocal(group.getX() * groupWidth, group.getY() * groupWidth);
        UV.divideLocal(groupWidth);
        
        int activeTex = state.textureIndex;
        
        if (activeTex == -1)
            return;
        
        if (!group.isTextureSet()){
            group.setTextureSet(World.getWorld().getTextureSet(), true);
        }
        
        final float startIntensity = 0.05f * state.brushStrength;
        
        if (activeTex == 0){
            int count = group.getAlphamapCount();
            float intensity = startIntensity / count;
            
            for (int i = 0; i < count; i++){
                Texture2D alphamap = group.getPaintableAlphamap(i);
                
                doPaintAction(alphamap.getImage(), UV, brushSize, -intensity);
                LWJGLTextureUpdater.updateTexture(alphamap,                         // OGL texture
                                                  alphamap.getImage().getData(0),   // image data
                                                  alphamap.getImage().getWidth(),   // width
                                                  alphamap.getImage().getHeight(),  // height
                                                  alphamap.getImage().getFormat()); // format
            }
        }else{
            int count = group.getAlphamapCount();
            float intensity = startIntensity;

            for (int i = 0; i < count; i++){
                if (activeTex-1 == i){
                    intensity = startIntensity;
                }else{
                    intensity = -startIntensity / (count-1);
                }
                
                Texture2D alphamap = group.getPaintableAlphamap(i);
                
                doPaintAction(alphamap.getImage(), UV, brushSize, intensity);
                LWJGLTextureUpdater.updateTexture(alphamap,                         // OGL texture
                                                  alphamap.getImage().getData(0),   // image data
                                                  alphamap.getImage().getWidth(),   // width
                                                  alphamap.getImage().getHeight(),  // height
                                                  alphamap.getImage().getFormat()); // format
            }
        }
        
    }

    private TextureBrush() {
    }

}
