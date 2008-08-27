import com.jme.app.SimpleGame;

import com.jme.image.Image;
import com.jme.image.Image.Format;
import com.jme.image.Texture;
import com.jme.image.Texture2D;
import com.jme.renderer.ColorRGBA;
import com.jme.scene.Spatial.LightCombineMode;
import com.jme.scene.shape.Quad;
import com.jme.scene.state.TextureState;
import com.jme.util.geom.BufferUtils;
import java.nio.ByteBuffer;
import java.nio.FloatBuffer;

/**
 * A class used to identify a particular jME bug.
 * 
 * @author Kirill Vainer
 */
public class Bugs extends SimpleGame {

    public static void main(String[] args){
        Bugs bugs = new Bugs();
        bugs.setConfigShowMode(ConfigShowMode.AlwaysShow);
        bugs.start();
    }
    
    @Override
    protected void simpleInitGame() {
        display.getRenderer().setBackgroundColor(ColorRGBA.darkGray);
        
        Quad doesntWork = new Quad("DoesntWork", 10, 10);
        doesntWork.setLightCombineMode(LightCombineMode.Off);
        
        ByteBuffer data = BufferUtils.createByteBuffer(4 * 4 * 4 * 3);
        FloatBuffer fb = data.asFloatBuffer();
        Image floatImage = new Image(Format.Luminance32F, 4, 4, data);
        
        // a uniform white texture
        fb.rewind();
        for (int y = 0; y < 4; y++){
            for (int x = 0; x < 4; x++){
                fb.put(1f);
            }
        }
        fb.rewind();
        
        Texture tex = new Texture2D();
        tex.setImage(floatImage);
        
        TextureState problems = display.getRenderer().createTextureState();
        problems.setTexture(tex);
        doesntWork.setRenderState(problems);
        
        rootNode.attachChild(doesntWork);
        rootNode.updateRenderState();
    }

}
