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
import com.jme.image.Texture.WrapMode;

/**
 * A layer which mixes packed detail and alpha textures
 * to create grayscale splatting.
 * 
 * @author Momoko_Fan
 */
@Deprecated
public class GrayscaleDetailLayer implements Layer {
    
    protected Texture[] tex = new Texture[2];
    
    public GrayscaleDetailLayer() {
    }

    public GrayscaleDetailLayer(Texture detail, Texture mixmap){
        setDetail(detail);
        setMixmap(mixmap);
    }
    
    public void setDetail(Texture detail){
        tex[1] = detail;
        tex[1].setWrap(WrapMode.Repeat);
    }
    public void setMixmap(Texture mixmap){
        tex[0] = mixmap;
    }
    
    public boolean getGLSLWriteDepth() {
        return false;
    }
    
    public void genGLSLShaderCode(StringBuffer buf, int[] texIndices, boolean rDefined, boolean setFrag) {
        // dot product
        buf.append("   m = texture2D(tex"+texIndices[0]+",tca);\n");
        buf.append("   c = dot( (texture2D(tex"+texIndices[1]+",tcc) / 2.0) / 0.5,m);\n");
        
        // write to color
        if (rDefined){
            if (setFrag)
                buf.append("   gl_FragColor = ");
            else
                buf.append("   r = ");

            buf.append("mix(r,c,length(m));\n");           
        }else{
            if (setFrag)
                buf.append("   gl_FragColor = c;\n");
            else
                buf.append("   r = c;\n");
        }
    }

    public String getGLSLShaderHeader() {
        return "   float c;\n" +
               "   vec4 m;\n";
    }

    public String getARBShaderOptions() {
        return "";
    }
    
    public void genARBShaderCode(StringBuffer buf, int[] texIndices, boolean rDefined, boolean setFrag) {
        buf.append("TEX c,fragment.texcoord[1],texture[").append(texIndices[1]).append("],2D;\n");
        buf.append("TEX a,fragment.texcoord[0],texture[").append(texIndices[0]).append("],2D;\n");
        if (rDefined){
            buf.append("DP4 c,c,a;\n");
            buf.append("LRP ");
            
            if (setFrag)
                buf.append("r");
            else
                buf.append("result.color");
           
            buf.append(",0.5,c,r;\n");
        }else{
            if (setFrag){
                buf.append("DP4 result.color,c,a;\n");
            }else{
                buf.append("DP4 r,c,a;\n");
            }
        }
        
    }

    public String getARBShaderHeader() {
        return "";
        //return "TEMP d,m;";
    }
  
    public Texture[] getTextures() {
        return tex;
    }

    public void execute(SplatEnv.CombinerState state){
        if (state.getRemainingSlots() < 2)
            throw new UnsupportedOperationException("GrayscaleDetail requires at least 2 texture slots");
        
        state.pushTexture(tex[0]);
        int alphaTexIndex = state.pushTexture(tex[1]);
        
//        tex[0].setApply(Texture.AM_COMBINE);
//        
//        tex[0].setCombineFuncRGB(Texture.ACF_DOT3_RGB);
//        
//        tex[0].setCombineSrc0RGB(Texture.ACS_TEXTURE);
//        tex[0].setCombineOp0RGB(Texture.ACO_SRC_COLOR);
//        
//        tex[0].setCombineSrc1RGB(Texture.ACS_TEXTURE0+alphaTexIndex);
//        tex[0].setCombineOp1RGB(Texture.ACO_SRC_COLOR);
//        
//        // --------------------------------------------------------
//        tex[0].setCombineFuncAlpha(Texture.ACF_MODULATE);
//        
//        tex[0].setCombineSrc0Alpha(Texture.ACS_TEXTURE);
//        tex[0].setCombineOp0Alpha(Texture.ACO_SRC_ALPHA);
//        
//        tex[0].setCombineSrc1Alpha(Texture.ACS_TEXTURE0+alphaTexIndex);
//        tex[0].setCombineOp1Alpha(Texture.ACO_SRC_ALPHA);
//        
//        tex[1].setApply(Texture.AM_COMBINE);
//        
//        if (state.isLightingEnabled() && state.getRemainingSlots()==0){
//            tex[1].setCombineFuncRGB(Texture.ACF_MODULATE);
//            
//            tex[1].setCombineSrc0RGB(Texture.ACS_PREVIOUS);
//            tex[1].setCombineOp0RGB(Texture.ACO_SRC_COLOR);
//            tex[1].setCombineSrc1RGB(Texture.ACS_PRIMARY_COLOR);
//            tex[1].setCombineOp1RGB(Texture.ACO_SRC_COLOR);
//            tex[1].setCombineScaleRGB(state.getLightModulateScale());
//        }else{
//            tex[1].setCombineFuncRGB(Texture.ACF_REPLACE);
//            
//            tex[1].setCombineSrc0RGB(Texture.ACS_PREVIOUS);
//            tex[1].setCombineOp0RGB(Texture.ACO_SRC_COLOR);
//        }
//        
//        tex[1].setCombineFuncAlpha(Texture.ACF_REPLACE);
//            
//        tex[1].setCombineSrc0Alpha(Texture.ACS_PREVIOUS);
//        tex[1].setCombineOp1Alpha(Texture.ACO_SRC_ALPHA);
        
        // unit1_color = (tex0_color DOT tex1_color) * vertex_color
        // unit1_alpha = tex0_alpha * tex1_alpha
    }

}
