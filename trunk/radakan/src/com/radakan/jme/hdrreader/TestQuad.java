/*
 * Copyright (c) 2003-2008 jMonkeyEngine
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 *
 * * Neither the name of 'jMonkeyEngine' nor the names of its contributors 
 *   may be used to endorse or promote products derived from this software 
 *   without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

package com.radakan.jme.hdrreader;

import com.jme.app.SimpleGame;
import com.jme.image.Image;
import com.jme.image.Texture;
import com.jme.image.Texture.MagnificationFilter;
import com.jme.image.Texture.MinificationFilter;
import com.jme.image.Texture.WrapMode;
import com.jme.image.Texture2D;
import com.jme.math.Vector3f;
import com.jme.renderer.ColorRGBA;
import com.jme.scene.Spatial.LightCombineMode;
import com.jme.scene.shape.Quad;
import com.jme.scene.state.BlendState;
import com.jme.scene.state.TextureState;
import com.jme.util.TextureManager;
import java.io.File;

import java.io.FileInputStream;
import java.io.InputStream;

public class TestQuad extends SimpleGame {

    private Quad quad;
    
    

    public static void main(String[] args) {
        TextureManager.registerHandler(".hdr", new HDRLoader());
        
        TestQuad app = new TestQuad();
        app.setConfigShowMode(ConfigShowMode.AlwaysShow);
        app.start(); 
    }

    /*
     * (non-Javadoc)
     * 
     * @see com.jme.app.SimpleGame#initGame()
     */
    protected void simpleInitGame() {
        try {
            display.setTitle("jME - HDR Loading");
            display.getRenderer().setBackgroundColor(ColorRGBA.lightGray);
            
            quad = new Quad("quad", 40, 40);
            
            File f = new File("C:\\test.hdr");
            
            HDRLoader loader = new HDRLoader();
            InputStream in = new FileInputStream(f);
            Image img = loader.load(in);
            in.close();
            
            img = ToneMapper.toneMap(img, 20f);
            
            System.out.println("Image");
            System.out.println("Resolution: "+img.getWidth()+"x"+img.getHeight());
            System.out.println("Format: "+img.getFormat());
            System.out.println("HasMips: "+img.hasMipmaps());
            
            float aspect = (float) img.getWidth() / img.getHeight();
            quad.setLocalScale(new Vector3f(aspect, 1f, 0));
            
            Texture t = new Texture2D();
            t.setImage(img);
            
            TextureState state = display.getRenderer().createTextureState();
            t.setMinificationFilter(MinificationFilter.Trilinear);
            t.setMagnificationFilter(MagnificationFilter.Bilinear);
            state.setTexture(t);
            t.setScale(new Vector3f(1f,-1f,1f));
            t.setWrap(WrapMode.Repeat);
            quad.setRenderState(state);
            
            BlendState bs = display.getRenderer().createBlendState();
            bs.setBlendEnabled(true);
            bs.setSourceFunction(BlendState.SourceFunction.SourceAlpha);
            bs.setDestinationFunction(BlendState.DestinationFunction.OneMinusSourceAlpha);
            quad.setRenderState(bs);
            
            quad.setLightCombineMode(LightCombineMode.Off);
            quad.setLocalTranslation(new Vector3f(0, 0, -40));
            quad.updateRenderState();
            rootNode.attachChild(quad);
            
        } catch (Throwable ex) {
            ex.printStackTrace();
        }
    }
}