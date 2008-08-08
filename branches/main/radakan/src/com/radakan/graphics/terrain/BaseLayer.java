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

package com.radakan.graphics.terrain;

import com.jme.image.Texture;
import com.jme.image.Texture.ApplyMode;
import com.jme.image.Texture.CombinerFunctionRGB;
import com.jme.image.Texture.CombinerOperandRGB;
import com.jme.image.Texture.CombinerSource;
import com.jme.image.Texture.WrapMode;
import com.jme.math.Vector3f;

/**
 * BaseLayer should be the first layer in all tile environments
 */
public class BaseLayer implements Layer {
    
    protected Texture base;
    protected float tileScale = -1;
    
    public BaseLayer(Texture baseTex){
        base = baseTex;
        base.setWrap(WrapMode.Repeat);
    }
    public BaseLayer() {
        this(null);
    }

    public void setScale(float scale){
        this.tileScale = scale;
    }
    
    public void genGLSLShaderCode(StringBuffer buf, int[] texIndices, boolean rDefined, boolean setFrag) {
        if (tileScale == -1)
            throw new UnsupportedOperationException();
                    
        if (setFrag){
            buf.append("   gl_FragColor = texture2D(tex"+texIndices[0]+",tca * "+tileScale+");");
        }else{
            buf.append("   vec4 r = texture2D(tex").append(texIndices[0]).append(",tca * "+tileScale+");\n");
        }
    }

    public String getGLSLShaderHeader() {
        return "";
    }
    
    public boolean getGLSLWriteDepth() {
        return false;
    }

    public void genARBShaderCode(StringBuffer buf, int[] texIndices, boolean rDefined, boolean setFrag) {
        if (tileScale == -1)
            throw new UnsupportedOperationException();
        
        if (tileScale == 1.0){
            if (setFrag){
                buf.append("TEX result.color,fragment.texcoord[0],texture["+texIndices[0]+"],2D;\n");
            }else{
                buf.append("TEX r,fragment.texcoord[0],texture[").append(texIndices[0])
                   .append("],2D;\n");
            }
        }else{
            buf.append("MUL c,fragment.texcoord[0], ").append(tileScale).append(";\n");
            if (setFrag){
                buf.append("TEX result.color,c,texture["+texIndices[0]+"],2D;\n");
            }else{
                buf.append("TEX r,c,texture[").append(texIndices[0])
                   .append("],2D;\n");
            }
        }
    }

    public String getARBShaderHeader() {
        return "";
    }

    public String getARBShaderOptions() {
        return "";
    }
    
    public Texture[] getTextures() {
        return new Texture[] { base };
    }
    
    public void execute(SplatEnv.CombinerState state) {
        if (state.getPassCount() != 1 || state.getTextureCount() > 0 || state.isPassFull())
            throw new UnsupportedOperationException("BaseLayer must be first!");
        
        state.pushTexture(base);
        
        base.setScale(new Vector3f(tileScale, tileScale, 1f));
        
        if (state.isLightingEnabled() && (state.isPassFull() || state.isLastLayer())){
            base.setApply(ApplyMode.Combine);
            base.setCombineFuncRGB(CombinerFunctionRGB.Modulate);
            base.setCombineSrc0RGB(CombinerSource.PrimaryColor);
            base.setCombineOp0RGB(CombinerOperandRGB.SourceColor);
            base.setCombineSrc1RGB(CombinerSource.CurrentTexture);
            base.setCombineOp1RGB(CombinerOperandRGB.SourceColor);
            base.setCombineScaleRGB(state.getLightModulateScale());
        }else
            base.setApply(ApplyMode.Replace);
    }
    
}
