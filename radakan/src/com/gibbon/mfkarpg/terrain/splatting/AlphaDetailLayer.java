/*
 * Copyright (c) 2007, MFKARPG
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     - Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     - Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     - Neither the name of the Gibbon Entertainment nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY 'Gibbon Entertainment' "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL 'Gibbon Entertainment' BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

package com.gibbon.mfkarpg.terrain.splatting;

import com.jme.image.Texture;
import com.jme.image.Texture.ApplyMode;
import com.jme.image.Texture.CombinerFunctionAlpha;
import com.jme.image.Texture.CombinerFunctionRGB;
import com.jme.image.Texture.CombinerOperandAlpha;
import com.jme.image.Texture.CombinerOperandRGB;
import com.jme.image.Texture.CombinerSource;
import com.jme.image.Texture.WrapMode;
import com.jme.math.Vector3f;
import com.jme.scene.state.BlendState.DestinationFunction;
import com.jme.scene.state.BlendState.SourceFunction;

/**
 * A detail layer blended with an alpha texture.
 * For use in texture splatting.
 * 
 * @author Momoko_Fan
 */
public class AlphaDetailLayer implements Layer {
    
    protected SplatEnv env;
    protected Texture[] tex = new Texture[2];
    protected float tileScale = -1.0f;
    
    public AlphaDetailLayer(Texture detailmap, Texture alphamap) {
        setDetailmap(detailmap);
        setAlphamap(alphamap);
    }
    public AlphaDetailLayer(Texture detailmap){
        setDetailmap(detailmap);
    }
    public AlphaDetailLayer(){
    }
    
    public void setDetailmap(Texture detailmap){
        tex[0] = detailmap;
        tex[0].setWrap(WrapMode.Repeat);
        if (env!=null) env.refresh();
    }
    
    public void setAlphamap(Texture alphamap){
        if (alphamap==null)
            throw new NullPointerException();
        tex[1] = alphamap;
        if (env!=null) env.refresh();
    }
    
    public Texture getAlphamap(){
        return tex[1];
    }
    
    public void setScale(float scale){
        if (scale < 1.0)
            throw new IllegalArgumentException();
                    
        tileScale = scale;
    }

    /**
     * vec4 r = the accum variable
     * vec2 tcc = texture coordinates for detailmaps (tiled/scaled)
     * vec2 tca = texture coordinates for alphamaps
     */
    public void genGLSLShaderCode(StringBuffer buf, int[] texIndices, boolean rDefined, boolean setFrag) {
        if (rDefined){
            if (setFrag){
                buf.append("   gl_FragColor = ");
            }else{
                buf.append("   r = ");
            }

            buf.append("mix(r,texture2D(tex").append(texIndices[0]);
                    
            if (tileScale == -1)  
                buf.append(",tcc),texture2D(tex").append(texIndices[1]);
            else
                buf.append(",tca * "+tileScale+"),texture2D(tex").append(texIndices[1]);
            
            buf.append(",tca).a);\n");
        }else{
            if (setFrag){
                buf.append("   gl_FragColor = ");
            }else{
                buf.append("   vec4 r = ");
            }
            
            if (tileScale == -1)
                buf.append("texture2D(tex"+texIndices[0]+",tcc);\n");
            else
                buf.append("texture2D(tex"+texIndices[0]+",tca * "+tileScale+");\n");
        }
    }
    
    public String getGLSLShaderHeader() {
        return "";
    }
    
    public boolean getGLSLWriteDepth() {
        return false;
    }

    /**
     * c = temporary variable (used for detailmap color storage)
     * a = temporary variable (used for alphamap color storage)
     * r = the accum variable
     *
     * fragment.texcoord[1] = scaled coordinates
     * fragment.texcoord[0] = normal coordinates
     */
    public void genARBShaderCode(StringBuffer buf, int[] texIndices, boolean rDefined, boolean setFrag) {
        if (!rDefined)
            throw new UnsupportedOperationException("Use BaseLayer as the first layer in a SplatEnv");
        
        if (tileScale == -1.0f)
            buf.append("TEX c,fragment.texcoord[1],texture["+texIndices[0]+"],2D;\n");
        else{
            buf.append("MUL c,fragment.texcoord[0], "+tileScale+";\n");
            buf.append("TEX c,c,texture["+texIndices[0]+"],2D;\n");
        }
        
        buf.append("TEX a,fragment.texcoord[0],texture["+texIndices[1]+"],2D;\n");
        buf.append("LRP ");
        
        if (setFrag){
            buf.append("result.color");
        }else{
            buf.append("r");
        }
        buf.append(",a.a,c,r;\n");
    }
    
    public String getARBShaderHeader() {
        return "";
    }
    
    public String getARBShaderOptions() {
        return "";
    }

    public Texture[] getTextures() {
        return tex;
    }
    
    public void execute(SplatEnv.CombinerState state){
        if (state.getUnitCount() == 1){
            throw new UnsupportedOperationException("ARB_multitexture extension is required");
        }
        
        // not enough texture slots to do texture splatting
        // flush this pass
        if (state.getRemainingSlots() < 2){
            state.addPass(SourceFunction.SourceAlpha, DestinationFunction.OneMinusSourceAlpha);
        }
        
        // use default scale if no overriding scale specified
        if (tileScale == -1)
            tex[0].setScale(new Vector3f(state.getTileScale(), state.getTileScale(), 1));
        else
            tex[0].setScale(new Vector3f(tileScale, tileScale, 1));
        
        boolean starting = state.getTextureCount() == 0;
        
        state.pushTexture(tex[0]);
        int alphaTexIndex = state.pushTexture(tex[1]);

        if (state.getUnitCount() == 2){
            // compatability mode
            // works on all cards with ARB_multitexture extension
            if (state.isLightingEnabled())
                tex[0].setApply(ApplyMode.Modulate);
            else
                tex[0].setApply(ApplyMode.Replace);
            
            tex[1].setApply(ApplyMode.Modulate);
            
            return;
        }
        
        // tex[0] blends it's color with the previous color using the alphamap
        // unless it's the first layer in a pass, then it simply replaces the color
        if (starting){
            // unit0_color = this_color
            // unit0_alpha = next_alpha
            
            tex[0].setApply(ApplyMode.Combine);
            
            tex[0].setCombineFuncRGB(CombinerFunctionRGB.Replace);
            
            tex[0].setCombineSrc0RGB(CombinerSource.CurrentTexture);
            tex[0].setCombineOp0RGB(CombinerOperandRGB.SourceColor);
            
            //tex[0].setCombineSrc1RGB(Texture.ACS_TEXTURE0+alphaTexIndex);
            //tex[0].setCombineOp1RGB(Texture.ACO_SRC_ALPHA);
            
            // -------------------------------------------------------
            if (state.getPassCount() == 0){
                tex[0].setCombineFuncAlpha(CombinerFunctionAlpha.Replace);
                tex[0].setCombineSrc0Alpha(CombinerSource.Previous);
                tex[0].setCombineOp0Alpha(CombinerOperandAlpha.SourceAlpha);
            }else{
                tex[0].setCombineFuncAlpha(CombinerFunctionAlpha.Replace);
                tex[0].setCombineSrc0Alpha(CombinerSource.valueOf("TextureUnit"+alphaTexIndex));
                tex[0].setCombineOp0Alpha(CombinerOperandAlpha.SourceAlpha);
            }
       }else{
            tex[0].setApply(ApplyMode.Combine);
            
            // unit0_color = mix(prev_color,this_color,next_alpha)
            // unit0_alpha = prev_alpha + next_alpha
            
            tex[0].setCombineFuncRGB(CombinerFunctionRGB.Interpolate);
           
            tex[0].setCombineSrc0RGB(CombinerSource.CurrentTexture);
            tex[0].setCombineOp0RGB(CombinerOperandRGB.SourceColor);
            
            
            tex[0].setCombineSrc1RGB(CombinerSource.Previous);
            tex[0].setCombineOp1RGB(CombinerOperandRGB.SourceColor);
            
            tex[0].setCombineSrc2RGB(CombinerSource.valueOf("TextureUnit"+alphaTexIndex));
            tex[0].setCombineOp2RGB(CombinerOperandRGB.SourceAlpha);
            
            // -------------------------------------------------------
            if (state.getPassCount() == 0){
                tex[0].setCombineFuncAlpha(CombinerFunctionAlpha.Replace);
                
                tex[0].setCombineSrc0Alpha(CombinerSource.Previous);
                tex[0].setCombineOp0Alpha(CombinerOperandAlpha.SourceAlpha);
                
                tex[0].setCombineSrc0Alpha(CombinerSource.Previous);
                tex[0].setCombineOp0Alpha(CombinerOperandAlpha.SourceAlpha);
            }else{
                tex[0].setCombineFuncAlpha(CombinerFunctionAlpha.Add);

                tex[0].setCombineSrc0Alpha(CombinerSource.valueOf("TextureUnit"+alphaTexIndex));
                tex[0].setCombineOp0Alpha(CombinerOperandAlpha.SourceAlpha);

                tex[0].setCombineSrc1Alpha(CombinerSource.Previous);
                tex[0].setCombineOp1Alpha(CombinerOperandAlpha.SourceAlpha);
            }
        }
        
        // tex[1] is on passthrough, unless lighting must be added to the result color
        tex[1].setApply(ApplyMode.Combine);
        
        tex[1].setCombineFuncAlpha(CombinerFunctionAlpha.Replace);
            
        tex[1].setCombineSrc0Alpha(CombinerSource.Previous);
        tex[1].setCombineOp0Alpha(CombinerOperandAlpha.SourceAlpha);
        
        if (state.isLightingEnabled() && (state.getRemainingSlots()==0 || state.isLastLayer())){
            // unit1_color = prev_color * vertex_color
            tex[1].setCombineFuncRGB(CombinerFunctionRGB.Modulate);
            
            tex[1].setCombineSrc0RGB(CombinerSource.Previous);
            tex[1].setCombineOp0RGB(CombinerOperandRGB.SourceColor);
            
            tex[1].setCombineSrc1RGB(CombinerSource.PrimaryColor);
            tex[1].setCombineOp1RGB(CombinerOperandRGB.SourceColor);
            
            tex[1].setCombineScaleRGB(state.getLightModulateScale());
        }else{
            // unit1_color = prev_color
            tex[1].setCombineFuncRGB(CombinerFunctionRGB.Replace);
            
            tex[1].setCombineSrc0RGB(CombinerSource.Previous);
            tex[1].setCombineOp0RGB(CombinerOperandRGB.SourceColor);
        }
    }

}
