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

/**
 * Implements linear fog for shader texturing.
 * Does nothing in fixed-function mode.
 * @author Daniel
 */
public class FogLayer implements Layer {
    
    public FogLayer() {
    }

    public void genGLSLShaderCode(StringBuffer buf, int[] texIndices, boolean rDefined, boolean setFrag) {
        if (!setFrag)
            throw new IllegalStateException("FogLayer must be last in a SplatEnv");
        
        buf.append("   if (vDepth > gl_Fog.start)\n"+
                   "       gl_FragColor = mix(r,gl_Fog.color,1.0-(gl_Fog.end-vDepth)*gl_Fog.scale);\n"+
                   "   else\n"+
                   "       gl_FragColor = r;\n");
    }

    public String getGLSLShaderHeader() {
        return "";
    }
    
    public boolean getGLSLWriteDepth() {
        return true;
    }

    public void genARBShaderCode(StringBuffer buf, int[] texIndices, boolean rDefined, boolean setFrag) {
        buf.append("MOV result.color, r;\n");
    }

    public String getARBShaderHeader() {
        return "";
    }
    
    public String getARBShaderOptions() {
        return "OPTION ARB_fog_linear;\n";
    }

    public Texture[] getTextures() {
        return new Texture[0];
    }
    
    public void execute(SplatEnv.CombinerState state){
    }

}
