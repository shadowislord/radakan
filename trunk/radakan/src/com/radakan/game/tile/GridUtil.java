package com.radakan.game.tile;

import com.jme.math.Vector2f;
import com.jme.math.Vector3f;
import com.jme.util.geom.BufferUtils;
import java.nio.FloatBuffer;
import java.nio.IntBuffer;

public final class GridUtil {

    public static final FloatBuffer writeVertexArray(int width, int height, Vector3f scale){
        FloatBuffer store = BufferUtils.createFloatBuffer(width*height*3);

        // going through Y/Z first, scanlines are horizontal in TLOD
        for (int z = 0; z < height; z++){
            for (int x = 0; x < width; x++){
                store.put( (float)x*scale.x );
                store.put( (float)0.0*scale.y );
                store.put( (float)z*scale.z );
            }
        }
        
        return store;
    }
    
    public static final FloatBuffer writeNormalArray(int width, int height) {
        FloatBuffer store = BufferUtils.createFloatBuffer(width*height*3);

        //Vector3f temp = new Vector3f();
        for (int z = 0; z < height; z++){
            for (int x = 0; x < width; x++){
//                getNormal(x,z,temp);
//                store.put(temp.x).put(temp.y).put(temp.z);
                store.put(0.0f).put(1.0f).put(0.0f);
            }
        }
        
        return store;
    }
    
    public static final IntBuffer writeIndexArray(int width, int height){
        int faceN = (width-1)*(height-1)*2;
        
        IntBuffer store = BufferUtils.createIntBuffer(faceN*3);

        int i = 0;
        for (int z = 0; z < height-1; z++){
            for (int x = 0; x < width-1; x++){
                store.put(i).put(i+width).put(i+width+1);
                store.put(i+width+1).put(i+1).put(i);
                i++;
                
                if (x==width-2) i++;
            }
        }
        store.flip();
        
        return store;
    }
    
    public static final FloatBuffer writeTexCoordArray(int width, int height, int tWidth, int tHeight, Vector2f offset, Vector2f scale){
        FloatBuffer store = BufferUtils.createFloatBuffer(width*height*2);
        
        if (offset == null)
            offset = new Vector2f(0f, 0f);
        
        if (scale == null)
            scale = new Vector2f(1f, 1f);
        
        Vector2f tcStore = new Vector2f();
        for (int y = 0; y < height; y++){
            for (int x = 0; x < width; x++){
                tcStore.set( (float)x / (float)tWidth,
                             (float)y / (float)tHeight );
                store.put( offset.x + tcStore.x * scale.x );
                store.put( offset.y + tcStore.y * scale.y );
                
//                store.put( ((float)x) / getWidth() );
//                store.put( ((float)y) / getHeight() );
            }
            
        }
        
        return store;
    }
    
}
