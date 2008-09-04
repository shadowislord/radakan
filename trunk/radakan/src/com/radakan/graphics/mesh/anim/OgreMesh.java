/*
 * Radakan is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Radakan is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Radakan.  If not, see <http://www.gnu.org/licenses/>.
 */

package com.radakan.graphics.mesh.anim;

import com.jme.scene.TriMesh;
import com.jme.util.export.InputCapsule;
import com.jme.util.export.JMEExporter;
import com.jme.util.export.JMEImporter;
import com.jme.util.export.OutputCapsule;
import com.jme.util.geom.BufferUtils;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.FloatBuffer;
import java.nio.IntBuffer;

/**
 * Holds the bind pose and a weightbuffer that defines vertex->bone/weight associations.
 */
public class OgreMesh extends TriMesh {

    private static final long serialVersionUID = 8831653270716808462L;
    
    private transient FloatBuffer vertexBufferOriginal;
    private transient FloatBuffer normalBufferOriginal;
    private WeightBuffer weightBuffer;
    
    public OgreMesh(String name){
        super(name);
    }
    
    public void cloneFromMesh(OgreMesh source){
        vertexBufferOriginal = source.vertexBufferOriginal;
        normalBufferOriginal = source.normalBufferOriginal;
        
        setVertexBuffer(BufferUtils.createFloatBuffer(source.getVertexBuffer().capacity()));
        setNormalBuffer(BufferUtils.createFloatBuffer(source.getNormalBuffer().capacity()));
        setWeightBuffer(source.weightBuffer);
        
        if (hasBindPose()){
            // restore original vertex data from bind pose
            restoreBindPose();
        }else{
            // restore original vertex data from source
            source.getVertexBuffer().rewind();
            vertBuf.rewind();
            vertBuf.put(source.getVertexBuffer());
            
            source.getNormalBuffer().rewind();
            normBuf.rewind();
            normBuf.put(source.getNormalBuffer());
        }
    }
    
    public void setWeightBuffer(WeightBuffer weightBuf){
        if (weightBuf.indexes.limit() / 4 != this.getVertexCount())
            throw new IllegalArgumentException();
        
        weightBuffer = weightBuf;
    }
    
    public WeightBuffer getWeightBuffer(){
        return weightBuffer;
    }
    
    public FloatBuffer getVertexBufferOriginal(){
        return vertexBufferOriginal;
    }
    
    public FloatBuffer getNormalBufferOriginal(){
        return normalBufferOriginal;
    }
    
    public void clearBindPose(){
        vertexBufferOriginal = null;
        normalBufferOriginal = null;
    }
    
    public void saveCurrentToBindPose(){
        if (vertexBufferOriginal == null){
            vertexBufferOriginal = BufferUtils.createFloatBuffer(vertBuf.capacity());
        }
        if (normalBufferOriginal == null){
            normalBufferOriginal = BufferUtils.createFloatBuffer(normBuf.capacity());
        }
              
        vertBuf.rewind();
        vertexBufferOriginal.rewind();
        vertexBufferOriginal.put(vertBuf);
        
        normBuf.rewind();
        normalBufferOriginal.rewind();
        normalBufferOriginal.put(normBuf);
    }
    
    public void restoreBindPose(){
        vertBuf.rewind();
        vertexBufferOriginal.rewind();
        vertBuf.put(vertexBufferOriginal);
        
        normBuf.rewind();
        normalBufferOriginal.rewind();
        normBuf.put(normalBufferOriginal);
    }
    
    public boolean hasBindPose(){
        return vertexBufferOriginal != null &&
               (normBuf == null || normalBufferOriginal != null);
    }
    
    @Override
    public void write(JMEExporter e) throws IOException{
        // dont want to write a vertex buffer in an animation here.. 
        // make sure to restore bind pose
        if (hasBindPose())
            restoreBindPose();
        
        super.write(e);
        
        OutputCapsule out = e.getCapsule(this);
        out.write(hasBindPose(), "HadBindPose", false);
        if (weightBuffer != null){
            out.write(weightBuffer.indexes, "BoneIndexes", null);
            out.write(weightBuffer.weights, "BoneWeights", null);
        }
    }
    
    @Override
    public void read(JMEImporter i) throws IOException{
        super.read(i);
        
        InputCapsule in = i.getCapsule(this);
        if (in.readBoolean("HadBindPose", false)){
            saveCurrentToBindPose();
        }
        
        ByteBuffer indexes = in.readByteBuffer("BoneIndexes", null);
        if (indexes != null){
            FloatBuffer weights = in.readFloatBuffer("BoneWeights", null);
            weightBuffer = new WeightBuffer(indexes, weights);
        }
    }
    
}
