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
import com.jme.image.Texture.WrapMode;

public class PackedAlphaDetailLayer implements Layer {
    
    protected Texture[] tex = new Texture[5];
    
    public PackedAlphaDetailLayer(Texture packedalpha, Texture ... detailmaps){
        setPackedAlpha(packedalpha);
        setDetailmaps(detailmaps);
    }
    public PackedAlphaDetailLayer() {
    }
    
    public void setPackedAlpha(Texture packedalpha){
        tex[0] = packedalpha; 
    }
    public void setDetailmap(int index, Texture detailmap){
        detailmap.setWrap(WrapMode.Repeat);
        tex[1+index] = detailmap;
    }
    
    public void setDetailmaps(Texture[] detailmaps){
        if (detailmaps.length<1 || detailmaps.length>4)
            throw new IllegalArgumentException("detailmap count must be 1 <= N <= 4");
        
        for (int i = 0; i < detailmaps.length; i++){
            if (detailmaps[i]==null) 
                throw new NullPointerException();
            
            setDetailmap(i,detailmaps[i]);
        }
    }
    
    public void genGLSLShaderCode(StringBuffer buf, int[] texIndices, boolean rDefined, boolean setFrag) {
        buf.append("   a = texture2D(tex"+texIndices[0]+",tca);\n");
        final char[] channels = { 'a', 'r', 'g', 'b' };
        int j = 0;
        int len = texIndices[1] - texIndices[0];
        for (int i = 0; i < len; i++){
            j = i + texIndices[0]+1;
            if (i==0 && !rDefined){
                buf.append("   vec4 r = texture2D(tex"+j+",tcc)*a.r;\n");
                continue;
            }
            buf.append("   r = mix(r,texture2D(tex"+j+",tcc),a."+channels[i]+");\n");
        }
        if (setFrag){
            buf.append("   gl_FragColor = r;\n");
        }
    }
    
    public String getGLSLShaderHeader() {
        return "   vec4 a;\n";
    }
    
    public boolean getGLSLWriteDepth() {
        return false;
    }

    public void genARBShaderCode(StringBuffer buf, int[] texIndices, boolean rDefined, boolean setFrag) {
        buf.append("TEX a,fragment.texcoord[0],texture["+texIndices[0]+"],2D;\n");
        
        final char[] channels = new char [] { 'a', 'r', 'g', 'b' };
        int j = 0;
        int len = texIndices[1] - texIndices[0];
        for (int i = 0; i < len; i++){
            j = i + texIndices[0]+1;
            buf.append("TEX c,fragment.texcoord[1],texture["+j+"],2D;\n");
            if (i==0 && !rDefined){
                buf.append("MUL r,c,a.r;\n");
                continue;
            }
            buf.append("LRP ");
            if (i == len-1 && setFrag)
                buf.append("result.color,a."+channels[i]+",c,r;\n");
            else
                buf.append("r,a."+channels[i]+",c,r;\n");
        }
    }
    
    public String getARBShaderHeader() {
        return "";
    }

    public String getARBShaderOptions() {
        return "";
    }
    
    public Texture[] getTextures() {
        if (tex[4]!=null)
            return tex;
        
        int last = -1;
        for (int i = 1; i < tex.length; i++){
            if (tex[i]==null) last = i-1;
        }
        if (last<=0)
            return new Texture[0];
        
        return null;
        //return Arrays.copyOfRange(tex,1,last);
    }

    public void execute(SplatEnv.CombinerState state) {
        // not supported by non-shader cards
    }

}
