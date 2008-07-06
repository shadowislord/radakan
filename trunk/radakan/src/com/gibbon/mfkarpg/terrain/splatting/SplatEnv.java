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
import com.jme.image.Texture.CombinerScale;
import com.jme.image.Texture.WrapMode;
import com.jme.math.FastMath;
import com.jme.renderer.Renderer;
import com.jme.scene.state.BlendState;
import com.jme.scene.state.BlendState.DestinationFunction;
import com.jme.scene.state.BlendState.SourceFunction;
import com.jme.scene.state.BlendState.TestFunction;
import com.jme.scene.state.FragmentProgramState;
import com.jme.scene.state.GLSLShaderObjectsState;
import com.jme.scene.state.TextureState;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

public class SplatEnv {
    
    protected List<Layer> layers = new ArrayList<Layer>();
    protected boolean needUpdate = false;

    protected List<Texture> textures = new ArrayList<Texture>();
    protected List<TextureState> states = new ArrayList<TextureState>();
    protected List<int[]> ranges = new ArrayList<int[]>();
    
    protected CombinerState state = new CombinerState();
    
    public void copy(SplatEnv target){
        target.state.setLightingEnabled(state.isLightingEnabled());
        target.setTileScale(getTileScale());
        for (Layer t: layers){
            target.addLayer(t);
        }
    }
    
    public void addLayer(Layer t){
        layers.add(t);
//        if (t instanceof AlphaDetailLayer){
//            ((AlphaDetailLayer)t).setScale(state.tile_scale);
//        }else if (t instanceof BaseLayer){
//            ((BaseLayer)t).setScale(state.tile_scale);
//        }
        refresh();
    }
    
    public void removeLayer(Layer t){
        layers.remove(t);
        refresh();
    }
    
    public List<Layer> getLayers(){
        return Collections.unmodifiableList(layers);
    }  
    
    public Layer getLayer(int index){
        return layers.get(index);
    }
    
    public int getLayerCount() {
        return layers.size();
    }
    
    public void refresh(){
        needUpdate = true;
    }

    public void setLightingEnabled(boolean enable) {
        state.setLightingEnabled(enable);
    }
    
    public class CombinerState {
        
        protected Renderer r;
        
        protected int tile_scale = 1;
        
        protected int texture_index;
        protected int texture_units = -1;
        protected int total_textures;
        
        protected int pass_index;
        protected int total_passes;

        protected List<TextureState> texStates = new ArrayList<TextureState>();
        protected List<BlendState> alphaStates = new ArrayList<BlendState>();
        
        protected boolean lastTex = false;
        protected boolean light = false;
        protected CombinerScale lightScale = CombinerScale.One;
        
        public void setRenderer(Renderer renderer){
            r = renderer;
        }
        public void reset(){
            texture_index = 0;
            total_textures = 0;
            pass_index = 0;
            total_passes = 0;
            texStates.clear();
            alphaStates.clear();
            light = false;
            lastTex = false;
            lightScale = CombinerScale.One;
            addPass();
        }
        public int pushTexture(Texture tex){
            if (isPassFull())
                throw new IllegalStateException("Pass is full");
            
            texStates.get(pass_index).setTexture(tex, texture_index);
            total_textures++;
            return texture_index++;
        }
        public void bindPass(int index){
            if (index > total_passes)
                throw new IndexOutOfBoundsException();
            
            pass_index = index;
            texture_index = texStates.get(pass_index).getNumberOfSetTextures();
        }
        public void addPass(BlendState as){
            alphaStates.add(as);
            texStates.add(r.createTextureState());
            total_passes++;
            pass_index = total_passes - 1;
            texture_index = 0;
        }
        public void addPass(SourceFunction blendSrc, DestinationFunction blendDst){
            BlendState as = r.createBlendState();
            as.setBlendEnabled(true);
            as.setSourceFunction(blendSrc);
            as.setDestinationFunction(blendDst);
            if (blendSrc == SourceFunction.SourceAlpha){
                as.setTestEnabled(true);
                as.setReference(0.01f);
                as.setTestFunction(TestFunction.GreaterThan);
            }
            addPass(as);
        }
        public void addPass(){
            addPass(null);
        }
        public int getPassCount(){
            return total_passes;
        }
        public int getTextureCount(){
            return texture_index;
        }
        public int getUnitCount(){
            return texture_units;
        }
        public int getRemainingSlots(){
            return texture_units - texture_index;
        }
        public Texture peekTexture(){
            return texStates.get(pass_index).getTexture(texture_index);
        }
        public boolean isPassFull(){
            return texture_index == texture_units;
        }
        public int getTileScale(){
            return tile_scale;
        }
        public boolean isLightingEnabled(){
            return state.light;
        }
        public void setLightingEnabled(boolean enable){
            state.light = enable;
        }
        public void setLightModulateScale(CombinerScale scale){
            state.lightScale = scale;
        }
        public CombinerScale getLightModulateScale(){
            return state.lightScale;
        }
        public boolean isLastLayer(){
            return lastTex;
        }
        
    }
    
    public void setTileScale(int tileScale){
        state.tile_scale = tileScale;
    }
    
    public int getTileScale() {
        return state.tile_scale;
    }
    
    protected void compileTextureList(){
        textures.clear();
        ranges.clear();
        for (Layer t: layers){
            int start = textures.size();
            textures.addAll(Arrays.asList(t.getTextures()));
            ranges.add(new int[]{start,textures.size()-1});
        }
        needUpdate = false;
    }
    
    public TextureState[] createFixedFuncPasses(Renderer r){
        state.texture_units = TextureState.getNumberOfFixedUnits();
        state.setRenderer(r);
        state.reset();
        
        for (int i = 0; i < layers.size(); i++){
            Layer layer = layers.get(i);
            
            if (i == layers.size()-1)
                state.lastTex = true;
            
            layer.execute(state);
        }

        return state.texStates.toArray(new TextureState[0]);
    }
    
    public BlendState[] getFixedFuncAlphaStates(){
        return state.alphaStates.toArray(new BlendState[0]);
    }
    
    public TextureState[] createShaderPasses(Renderer r){
        final int unitN = TextureState.getNumberOfFragmentUnits();
        
        if (needUpdate)
            compileTextureList();
        
        if (textures.size()==0)
            throw new IllegalStateException("No textures to process");
        
        TextureState[] tsa = new TextureState[(int)FastMath.ceil((float)textures.size() / (float)unitN)];
        for (int j = 0; j < tsa.length; j++)
            tsa[j] = r.createTextureState();
        
        int i = 0;
        int p = 0;
        for (Texture t: textures){
            t.setWrap(WrapMode.Repeat);
            tsa[p].setTexture(t,i);
            //System.out.println("("+p+","+i+"): "+t.getImageLocation());
            i++;
            if (i == unitN){
                i = 0;
                p++;
            }
        }
        
        return tsa;
    }
    
//    protected static final String getDefaultVertexShader(){
//        return  "varying vec2 tcc, tca;\n" +
//                "\n" +
//                "void main(){" +
//                "   tca = gl_MultiTexCoord0.st;\n" +
//                "   tcc = gl_MultiTexCoord1.st;\n" +
//                "   gl_Position = ftransform();\n" +
//                "}\n";
//    }
    
    protected static final String getDefaultVertexShaderDepth(){
        return  "varying float vDepth;\n" +
                "\n" +
                "void main(){" +
                "   gl_TexCoord[0].xy = gl_MultiTexCoord0.st;\n" +
                "   gl_Position = ftransform();\n" +
                "   vDepth = gl_Position.z;\n" +
                "}\n";
    }
    
    public GLSLShaderObjectsState createGLSLShader(Renderer r){
        GLSLShaderObjectsState shader = r.createGLSLShaderObjectsState();
        boolean writeDepth = false;
        for (Layer l: layers){
            if (l.getGLSLWriteDepth()){
                writeDepth = true;
                break;
            }
        }
        shader.load(
                writeDepth ? getDefaultVertexShaderDepth() : null,
                genGLSLShader(writeDepth));
        
        for (int i = 0; i < textures.size(); i++)
            shader.setUniform("tex"+i,i);
        
        return shader;
    }
    
    public FragmentProgramState createARBShader(Renderer r){
        FragmentProgramState fp = r.createFragmentProgramState();
        fp.load(genARBShader());
        return fp;
    }
    
    private String genARBShader(){
        if (needUpdate)
            compileTextureList();
        
        StringBuffer sb = new StringBuffer();
        
        sb.append("!!ARBfp1.0\n");
        sb.append("OPTION ARB_precision_hint_fastest;\n");
        
        Set<Class> uniqueTypes = new HashSet<Class>();
        for (int i = 0; i < layers.size(); i++){
            Layer t = layers.get(i);
            if (!uniqueTypes.contains(t.getClass())){
                sb.append(t.getARBShaderOptions());
                uniqueTypes.add(t.getClass());
            }
        }
        sb.append("\n");
        sb.append("TEMP a,c,r;\n");
        
        uniqueTypes.clear();
        for (int i = 0; i < layers.size(); i++){
            Layer t = layers.get(i);
            if (!uniqueTypes.contains(t.getClass())){
                sb.append(t.getARBShaderHeader());
                uniqueTypes.add(t.getClass());
            }
        }
        
        for (int i = 0; i < layers.size(); i++){
            layers.get(i).genARBShaderCode(sb,ranges.get(i),i!=0,i==layers.size()-1);
        }
        
        sb.append("END\n");
        
        System.out.println(sb.toString());
        
        return sb.toString();
    }
    
    private String genGLSLShader(boolean depth){
        if (needUpdate)
            compileTextureList();
        
        StringBuffer sb = new StringBuffer();
        
        //sb.append("void main(){\n    gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);\n}\n");
        
        //sb.append("varying vec2 tcc, tca;\n");
        sb.append("uniform sampler2D ");
        for (int i = 0; i < textures.size(); i++){
            sb.append("tex").append(i);
            if (i!=textures.size()-1) sb.append(",");
        }          
        sb.append(";\n");
        if (depth)
            sb.append("varying float vDepth;\n");
        
        sb.append("void main(){\n");
        
        sb.append("    vec2 tca = gl_TexCoord[0].xy;\n");
        
        Set<Class> uniqueTypes = new HashSet<Class>();
        for (int i = 0; i < layers.size(); i++){
            Layer t = layers.get(i);
            if (!uniqueTypes.contains(t.getClass())){
                sb.append(t.getGLSLShaderHeader());
                uniqueTypes.add(t.getClass());
            }
        }
        
        sb.append("\n");
        
        for (int i = 0; i < layers.size(); i++){
            layers.get(i).genGLSLShaderCode(sb,ranges.get(i),i!=0,i==layers.size()-1);
        }
        
        sb.append("}\n");
        
        System.out.println("Shader: \n"+sb.toString());
        
        return sb.toString();
    }

}
