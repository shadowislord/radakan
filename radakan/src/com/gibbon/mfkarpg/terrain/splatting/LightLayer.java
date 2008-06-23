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

import com.gibbon.mfkarpg.terrain.*;
import com.jme.image.Texture;
import com.jme.image.Texture.ApplyMode;
import com.jme.image.Texture.CombinerFunctionAlpha;
import com.jme.image.Texture.CombinerFunctionRGB;
import com.jme.image.Texture.CombinerOperandAlpha;
import com.jme.image.Texture.CombinerOperandRGB;
import com.jme.image.Texture.CombinerScale;
import com.jme.image.Texture.CombinerSource;
import com.jme.renderer.ColorRGBA;
import com.jme.scene.state.BlendState.DestinationFunction;
import com.jme.scene.state.BlendState.SourceFunction;

public class LightLayer implements Layer {
    
    protected Texture lightmap = null;
    protected CombinerScale modulateScale = CombinerScale.One;
    
    public LightLayer() {
    }
    
    public LightLayer(Texture lightmap){
        setLightmap(lightmap);
    }
    
    public LightLayer(Texture lightmap, CombinerScale modScale){
        setLightmap(lightmap);
        setModulateScale(modScale);
    }
    
    public LightLayer(CombinerScale modScale){
        setModulateScale(modScale);
    }
    
    public void setModulateScale(CombinerScale scale){
        modulateScale = scale;
    }
    
    public void setLightmap(Texture lightmap){
        this.lightmap = lightmap;
    }

    public void genGLSLShaderCode(StringBuffer buf, int[] texIndices, boolean rDefined, boolean setFrag) {
        if (!rDefined)
            throw new UnsupportedOperationException("LightLayer must be the last layer in the SplatEnv");

        if (setFrag)
            buf.append("   gl_FragColor = r * ");
        else
            buf.append("   r *= ");
        
        if (lightmap == null){
            buf.append("gl_Color");
        }else{
            buf.append("texture2D(tex"+texIndices[0]+",tca)");
        }
        
        if (modulateScale == CombinerScale.One){
            buf.append(";\n");
        }else{
            buf.append(" * "+modulateScale+";\n");
        }
    }

    public String getGLSLShaderHeader() {
        return "";
    }

    public boolean getGLSLWriteDepth() {
        return false;
    }

    public String getARBShaderOptions() {
        return "";
    }
    
    public void genARBShaderCode(StringBuffer buf, int[] texIndices, boolean rDefined, boolean setFrag) {
        if (!rDefined)
            throw new UnsupportedOperationException("LightLayer must be the last layer in the SplatEnv");
        
        if (lightmap == null){
            if (modulateScale == CombinerScale.One){
                if (setFrag)
                    buf.append("MUL result.color,r,fragment.color;\n");
                else
                    buf.append("MUL r,r,fragment.color;\n");
            }else{
                buf.append("MUL r,r,fragment.color;\n");
                if (setFrag)
                    buf.append("MUL result.color,r,"+modulateScale+";\n");
                else
                    buf.append("MUL r,r,"+modulateScale+";\n");
            }
        }else{
            buf.append("TEX c,fragment.texcoord[0],texture[").append(texIndices[0]).append("],2D;\n");
            if (modulateScale != CombinerScale.One){
                buf.append("MUL c,c,"+modulateScale+";\n");
            }
            if (setFrag)
                buf.append("MUL result.color,r,c;\n");
            else
                buf.append("MUL r,r,c;\n");
        }
    }

    public String getARBShaderHeader() {
        return "";
    }

    public Texture[] getTextures() {
        if (lightmap==null)
            return new Texture[0];
        else
            return new Texture[] { lightmap };
    }
    
    public boolean isValidCombineScale(float scale){
        return scale == 1.0f || scale == 2.0f || scale == 4.0f;
    }
    
    public void execute(SplatEnv.CombinerState state){
        if (lightmap==null){
            state.setLightingEnabled(true);
            state.setLightModulateScale(modulateScale);
            return;
        }
        
        boolean spotForMe = true;
        
        for (int i = 0; i < state.getPassCount(); i++){
            state.bindPass(i);
            if (state.isPassFull()){
                spotForMe = false;
                break;
            }
        }
        
        if (spotForMe){
            // use multitexturing to apply lightmap
            if (modulateScale != CombinerScale.One){
                lightmap.setApply(ApplyMode.Combine);
                lightmap.setCombineFuncRGB(CombinerFunctionRGB.Modulate);
                lightmap.setCombineSrc0RGB(CombinerSource.CurrentTexture);
                lightmap.setCombineOp0RGB(CombinerOperandRGB.SourceColor);
                lightmap.setCombineSrc1RGB(CombinerSource.Previous);
                lightmap.setCombineOp1RGB(CombinerOperandRGB.SourceColor);
                lightmap.setCombineScaleRGB(modulateScale);
                
                lightmap.setCombineFuncAlpha(CombinerFunctionAlpha.Replace);
                lightmap.setCombineSrc0Alpha(CombinerSource.Previous);
                lightmap.setCombineOp0Alpha(CombinerOperandAlpha.SourceAlpha);
            }else{
                // modulate scale is 1, modulate
                lightmap.setApply(ApplyMode.Modulate);
            }
            
            for (int i = 0; i < state.getPassCount(); i++){
                state.bindPass(i);
                state.pushTexture(lightmap);
            }
        }else{
            // use alpha blending to apply lightmap
            // the result color is sd + ds which evaluates to 2sd
            // so the default modulate scale is 2
            if (modulateScale != CombinerScale.Two){
                CombinerScale m = modulateScale == CombinerScale.Four ? CombinerScale.Two : CombinerScale.One;
                lightmap.setApply(ApplyMode.Combine);
                
                lightmap.setCombineFuncRGB(CombinerFunctionRGB.Replace);
                lightmap.setCombineSrc0RGB(CombinerSource.CurrentTexture);
                lightmap.setCombineOp0RGB(CombinerOperandRGB.SourceColor);
                lightmap.setCombineScaleRGB(m);
                
                lightmap.setCombineFuncAlpha(CombinerFunctionAlpha.Replace);
                lightmap.setCombineSrc0Alpha(CombinerSource.Constant);
                lightmap.setCombineOp0Alpha(CombinerOperandAlpha.SourceAlpha);
            }else{
                lightmap.setApply(ApplyMode.Replace);
            }
            
            
            state.addPass(SourceFunction.DestinationColor, DestinationFunction.SourceColor);
            state.pushTexture(lightmap);
        }
    }

}
