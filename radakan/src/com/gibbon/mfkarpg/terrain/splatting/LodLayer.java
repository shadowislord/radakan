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

/**
 * This is only useful for cards like GeForce 8000 which have dynamic branching.
 * 
 * @author Momoko_Fan
 */
public class LodLayer implements Layer {
    
    private Texture lod;
    private float startDist;
    private float endDist;
    
    public LodLayer(Texture lodtex, float startDist, float endDist) {
        lod = lodtex;
        this.startDist = startDist;
        this.endDist = endDist;
    }

    public void genGLSLShaderCode(StringBuffer buf, int[] texIndices, boolean rDefined, boolean setFrag) {
            buf.append("   if (vDepth > "+endDist+"){\n" +
                       "       gl_FragColor = texture2D(tex"+texIndices[0]+",tca);\n" +
                       "       return;\n"+
                       "   }\n");
    }

    public String getGLSLShaderHeader() {
        return "";
    }

    public void genARBShaderCode(StringBuffer buf, int[] texIndices, boolean rDefined, boolean setFrag) {
        
    }

    public String getARBShaderHeader() {
        return "";
    }

    public Texture[] getTextures() {
        return new Texture[]{ lod };
    }

    public void execute(SplatEnv.CombinerState state) {
        // level of detail layer only supported by shader cards
    }

    public boolean getGLSLWriteDepth() {
        return true;
    }

    public String getARBShaderOptions() {
        return "";
    }
    
}
