/*
 * Radakan RPG is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Radakan RPG is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Radakan RPG.  If not, see <http://www.gnu.org/licenses/>.
 */

package com.gibbon.meshparser;

import com.gibbon.radakan.error.ErrorReporter;
import com.jme.image.Texture;
import com.jme.renderer.ColorRGBA;
import com.jme.scene.state.MaterialState;
import com.jme.scene.state.RenderState;
import com.jme.scene.state.TextureState;
import com.jme.scene.state.ZBufferState;
import com.jme.util.TextureManager;
import com.jme.util.resource.ResourceLocatorTool;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.StreamTokenizer;
import java.net.URL;
import java.util.HashMap;
import java.util.Map;

/**
 * Reads OGRE3D material files<br/>
 * 
 * MaterialLoader.getMaterials() to get a map of materials.
 * call Material.apply(Spatial) to apply material to a model.
 */
public class MaterialLoader {
    
    private StreamTokenizer reader;
    private Map<String, Material> materialMap;
    private static final boolean DEBUG = true;
    
    public MaterialLoader() {
    }
    
    public void println(String str){
        if (DEBUG)
            System.out.println(str);
    }
    
    public Map<String, Material> getMaterials(){
        return materialMap;
    }
    
    public String tokenName(){
        switch (reader.ttype){
            case StreamTokenizer.TT_EOF:
                return "EOF";
            case StreamTokenizer.TT_EOL:
                return "EOL";
            case StreamTokenizer.TT_NUMBER:
                return reader.nval+"";
            case StreamTokenizer.TT_WORD:
                return reader.sval;
            default:
                return Character.toString((char)reader.ttype);
        }
    }
    
    public String nextStatement() throws IOException{
        while (reader.ttype != StreamTokenizer.TT_WORD){
            if (reader.ttype == StreamTokenizer.TT_EOF){
                return null;
            }
            reader.nextToken();
        }
        return reader.sval;
    }
    
    public double nextNumber() throws IOException {
        while (reader.ttype != StreamTokenizer.TT_NUMBER){
            if (reader.ttype == StreamTokenizer.TT_EOF){
                return -1;
            }
            reader.nextToken();
        }
        return reader.nval;
    }
    
    public ColorRGBA readColor() throws IOException{
        ColorRGBA color = new ColorRGBA();
        color.r = (float) nextNumber();
        reader.nextToken();
        color.g = (float) nextNumber();
        reader.nextToken();
        color.b = (float) nextNumber();
        reader.nextToken();
        color.a = (float) nextNumber();
        return color;
    }
    
    public float[] readFloatArray(){
        return null;
    }
    
    public void readMaterialStatement(Material material) throws IOException{
        String stat_name = nextStatement();
        if (stat_name.equals("receive_shadows")){
            reader.nextToken();
            material.recieveShadows = reader.sval.equalsIgnoreCase("true");
        }
        while (reader.ttype != StreamTokenizer.TT_EOL)
            reader.nextToken();
    }
    
    public void readTechniqueStatement(Material material) throws IOException{
        String stat_name = nextStatement();
        while (reader.ttype != StreamTokenizer.TT_EOL)
            reader.nextToken();
    }
    
    public void readUnitStatement(TextureState tex, int unit) throws IOException{
        String stat_name = nextStatement();
        
        if (stat_name.equals("texture")){
            reader.nextToken();
            String texName = nextStatement();
            
            URL texURL = ResourceLocatorTool.locateResource(ResourceLocatorTool.TYPE_TEXTURE, texName);
            
            if (texURL != null){
                Texture t = TextureManager.loadTexture(texURL, Texture.MM_LINEAR_LINEAR, Texture.FM_LINEAR, 0.0f, false);
                tex.setTexture(t, unit);
            }
            
            println("TEXTURE: "+texURL);
        }else if (stat_name.equals("tex_address_mode")){
            reader.nextToken();
            String mode = nextStatement();
            Texture t = tex.getTexture(unit);
            if (mode.equals("wrap")){
                t.setWrap(Texture.WM_WRAP_S_WRAP_T);
            }else{
                t.setWrap(Texture.WM_CLAMP_S_CLAMP_T);
            }
            println("ADDRESS MODE: "+mode);
        }else if (stat_name.equals("filtering")){
            reader.nextToken();
            String mode = nextStatement();
            Texture t = tex.getTexture(unit);
            if (mode.equals("trilinear")){
                t.setFilter(Texture.FM_LINEAR);
                t.setMipmapState(Texture.MM_LINEAR_LINEAR);
            }else{
                // ??
            }
            println("FILTERING: "+mode);
        }else{
            println("UNIT STAT: "+stat_name);
        }
        
        while (reader.ttype != StreamTokenizer.TT_EOL)
            reader.nextToken();
    }
    
    public void readTextureUnit(Material material) throws IOException{
        // skip 'texture_unit'
        nextStatement();
        
        TextureState ts = (TextureState) material.getState(RenderState.RS_TEXTURE);
        int unit = ts.getNumberOfSetTextures();
        
        println("TEXTURE UNIT START: "+unit);
        
        while (reader.ttype != '{')
            reader.nextToken();
        
        while (reader.ttype != '}'){
            readUnitStatement(ts, unit);
            while (reader.ttype == StreamTokenizer.TT_EOL)
                reader.nextToken();
        }
        
        println("TEXTURE UNIT END");
    }
    
    public void readPassStatement(Material material) throws IOException{
        String stat_name = nextStatement();
        
        if (stat_name.equals("ambient")){
            MaterialState ms = (MaterialState) material.getState(RenderState.RS_MATERIAL);
            ms.setAmbient(readColor());
            println("AMBIENT: "+ms.getAmbient());
        }else if (stat_name.equals("diffuse")){
            MaterialState ms = (MaterialState) material.getState(RenderState.RS_MATERIAL);
            ms.setDiffuse(readColor());
            println("DIFFUSE: "+ms.getDiffuse());
        }else if (stat_name.equals("specular")){
            MaterialState ms = (MaterialState) material.getState(RenderState.RS_MATERIAL);
            ms.setSpecular(readColor());
            reader.nextToken();
            ms.setShininess((float)nextNumber());
            println("SPECULAR: "+ms.getSpecular());
            println("SHININESS: "+ms.getShininess());
        }else if (stat_name.equals("emissive")){
            MaterialState ms = (MaterialState) material.getState(RenderState.RS_MATERIAL);
            ms.setEmissive(readColor());
            println("EMISSIVE: "+ms.getEmissive());
        }else if (stat_name.equals("scene_blend")){
            reader.nextToken();
            String mode = nextStatement();
            if (mode.equals("alpha_blend")){
                material.transparent = true;
//                AlphaState as = (AlphaState) material.getState(RenderState.RS_ALPHA);
//                as.setBlendEnabled(true);
//                as.setSrcFunction(AlphaState.SB_ZERO);
//                as.setDstFunction(AlphaState.DB_ZERO);
//                
//                CullState cs = (CullState) material.getState(RenderState.RS_CULL);
//                cs.setCullMode(CullState.CS_FRONT_AND_BACK);
            }else{
                throw new IOException("Unknown scene_blend mode: "+mode);
            }
        }else if (stat_name.equals("depth_write")){
            reader.nextToken();
            String enabled = nextStatement();
            ZBufferState zbuf = (ZBufferState) material.getState(RenderState.RS_ZBUFFER);
            if (enabled.equals("on")){
                zbuf.setWritable(true);
            }else{
                zbuf.setWritable(false);
            }
        }else if (stat_name.equals("texture_unit")){
            readTextureUnit(material);
        }else{
            println("PASS STAT: "+stat_name);
        }
        
        while (reader.ttype != StreamTokenizer.TT_EOL)
            reader.nextToken();
    }
    
    public void readPass(Material material) throws IOException{
        // skip "pass"
        reader.nextToken();
        
        println("PASS START");
        
        while (reader.ttype != '{')
            reader.nextToken();
        
        while (reader.ttype != '}'){
            readPassStatement(material);
            while (reader.ttype == StreamTokenizer.TT_EOL)
                reader.nextToken();
        }
        
        println("PASS END");
    }
    
    public void readTechnique(Material material) throws IOException{
        // skip "technique"
        String stat_name = nextStatement();
        
        if (!stat_name.equals("technique"))
            throw new IOException();
        
        println("TECHNIQUE START");
        
        while (reader.ttype != '{')
            reader.nextToken();
        
        while (reader.ttype != '}'){
            stat_name = nextStatement();
            
            if (stat_name.equals("pass")){
                reader.pushBack();
                readPass(material);
            }else{
                reader.pushBack();
                readTechniqueStatement(material);
            }
        }
        
        println("TECHNIQUE END");
    }
    
    public Material readMaterial() throws IOException{
        String stat_name = nextStatement();
        if (stat_name == null)
            // finished parsing
            return null;
        
        if (!stat_name.equals("material"))
            throw new IOException();
        
        reader.resetSyntax();
        reader.ordinaryChar('{');
        reader.ordinaryChar('}');
        reader.wordChars('_', '_');
        reader.wordChars('-', '-');
        reader.wordChars(' ', ' ');
        reader.wordChars('\t', '\t');
        reader.wordChars('0', '9');
        reader.wordChars('A', 'Z');
        reader.wordChars('a', 'z');
        reader.eolIsSignificant(true);
        
        reader.nextToken();
        String matName = reader.sval.trim();
        
        Material mat = new Material(matName);
        
        println("MATERIAL START: "+matName);
        materialMap.put(matName, mat);
        
        reader.resetSyntax();
        setupReader(reader);
        
        while (reader.ttype != '{'){
            reader.nextToken();
            
            //if (reader.ttype != '{' && reader.ttype != StreamTokenizer.TT_EOL)
            //    matName = matName + " " + reader.toString();
        }

        while (reader.ttype != '}'){
            if (reader.ttype == StreamTokenizer.TT_WORD){
                stat_name = reader.sval;
                
                if (stat_name.equals("technique")){
                    reader.pushBack();
                    readTechnique(mat);
                }else{
                    reader.pushBack();
                    readMaterialStatement(mat);
                }
            }
            
            reader.nextToken();
        }
        
        println("MATERIAL END");
        
        return mat;
    }
    
    public void setupReader(StreamTokenizer reader){
        reader.slashSlashComments(true);
        reader.parseNumbers();
        reader.ordinaryChar('{');
        reader.ordinaryChar('}');
        reader.wordChars('_', '_');
        reader.wordChars('A', 'Z');
        reader.wordChars('a', 'z');
        reader.wordChars('+', '/');
        reader.whitespaceChars('\t', '\t');
        reader.whitespaceChars(' ', ' ');
        //reader.wordChars('-', '-');
        reader.eolIsSignificant(true);
    }
    
    public void load(InputStream in){
        reader = new StreamTokenizer(new InputStreamReader(in));
        setupReader(reader);
        
        materialMap = new HashMap<String, Material>();
        
        try{
            while (true){
                Material mat = readMaterial();
                if (mat == null)
                    return;
            }
        } catch (IOException ex){
            ErrorReporter.reportError("Failed to read material file", ex);
        }
    }
    
    
    
}
