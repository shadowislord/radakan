package com.radakan.util.preview;

import com.jme.image.Texture;
import com.jme.math.Vector3f;
import com.jme.renderer.ColorRGBA;
import com.jme.scene.Node;
import com.jme.scene.Spatial;
import com.jme.scene.Spatial.LightCombineMode;
import com.jme.scene.TexCoords;
import com.jme.scene.TriMesh;
import com.jme.scene.state.BlendState;
import com.jme.scene.state.MaterialState;
import com.jme.scene.state.RenderState;
import com.jme.scene.state.TextureState;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.net.URL;
import java.nio.FloatBuffer;
import java.nio.IntBuffer;

/**
 * Converts jME scenes to an OBJ model
 * @author MomokoFan
 */
public class JmeToObj {
    
    private int vertOffset = 1;
    
    public void convert(Spatial scene, File folder) throws IOException{
        File objf = new File(folder,scene.getName()+".obj");
        File mtlf = new File(folder,scene.getName()+".mtl");
        
        BufferedWriter objw = new BufferedWriter(new FileWriter(objf));
        BufferedWriter mtlw = new BufferedWriter(new FileWriter(mtlf));
        
        objw.append("# Generated by JmeToObj\r\n\r\n");
        objw.append("mtllib "+scene.getName()+".mtl\r\n");

        dumpSpatial(scene, objw, mtlw);
        
        objw.close();
        mtlw.close();
    }
    
    private static final String f2s(float f){
        return Float.toString(f);
    }
    
    public boolean dumpMaterial(Spatial source, BufferedWriter writer) throws IOException{
        boolean hasMat = false;
        
        MaterialState ms = (MaterialState) source.getRenderState(RenderState.RS_MATERIAL);
        
        ColorRGBA color;
        if (ms != null){
            writer.append("newmtl "+source.getName()).append("\r\n");
            hasMat = true;
            
            color = ms.getAmbient();
            writer.append("Ka ")
                  .append(f2s(color.r)).append(' ')
                  .append(f2s(color.g)).append(' ')
                  .append(f2s(color.b)).append("\r\n");
            
            
            color = ms.getDiffuse();
            writer.append("Kd ")
                  .append(f2s(color.r)).append(' ')
                  .append(f2s(color.g)).append(' ')
                  .append(f2s(color.b)).append("\r\n");
            
            color = ms.getSpecular();
            writer.append("Ks ")
                  .append(f2s(color.r)).append(' ')
                  .append(f2s(color.g)).append(' ')
                  .append(f2s(color.b)).append("\r\n");
            
            writer.append("Ns ")
                  .append(f2s(ms.getShininess())).append("\r\n");
        }
        
        BlendState as = (BlendState) source.getRenderState(RenderState.RS_BLEND);
        if (as != null){
            if (!hasMat){
                writer.append("newmtl "+source.getName()+"\r\n");
                hasMat = true;
            }
            
            writer.append("illum 9\r\n");
            if (ms != null)
                writer.append("d ").append(f2s(ms.getAmbient().a * ms.getDiffuse().a * ms.getEmissive().a));
        }else if (source.getLightCombineMode() != LightCombineMode.Off){
            if (hasMat) writer.append("illum 2\r\n");
        }else{
            if (!hasMat){
                writer.append("newmtl "+source.getName()+"\r\n");
                hasMat = true;
            }
            
            writer.append("illum 0\r\n");
        }
        
        TextureState ts = (TextureState) source.getRenderState(RenderState.RS_TEXTURE);
        if (ts != null && ts.getTexture(0)!=null && ts.getTexture(0).getImageLocation()!=null){
            if (!hasMat){
                writer.append("newmtl "+source.getName()+"\r\n");
                hasMat = true;
            }
            
            Texture t = ts.getTexture(0);
            URL url = new URL(t.getImageLocation());
            writer.append("map_Kd ");
            
            if (t.getTranslation()!=null){
                writer.append("-o "+t.getTranslation().x+" "+t.getTranslation().y+" ");
            }
            
            if (t.getScale()!=null){
                writer.append("-s "+t.getScale().x+" "+t.getScale().y+" ");
            }
            
            if (t.getWrap(Texture.WrapAxis.S) == Texture.WrapMode.BorderClamp
            ||  t.getWrap(Texture.WrapAxis.S) == Texture.WrapMode.Clamp
            ||  t.getWrap(Texture.WrapAxis.S) == Texture.WrapMode.EdgeClamp){
                writer.append("-clamp on");
            }
            
            // XXX: MAke this a valid path
            writer.append(url.getPath()).append("\r\n");
        }
        
        if (hasMat)
            writer.newLine();
        
        return hasMat;
    }
    
    public void dumpSpatial(Spatial spat, BufferedWriter objs, BufferedWriter mtls) throws IOException{
        if (spat instanceof TriMesh){
            boolean mat = dumpMaterial(spat, mtls);
            dumpMesh((TriMesh)spat, mat, objs);
        }else if (spat instanceof Node){
            Node n = (Node)spat;
            if (n.getChildren() == null)
                return;
            
            for (Spatial s: n.getChildren()){
                dumpSpatial(s, objs, mtls);
            }
        }
    }
    
    public void dumpMesh(TriMesh mesh, boolean hasMaterial, BufferedWriter writer) throws IOException{
        FloatBuffer vb = mesh.getVertexBuffer();
        FloatBuffer nb = mesh.getNormalBuffer();
        TexCoords tb = mesh.getTextureCoords(0);
        IntBuffer ib = mesh.getIndexBuffer();
        
        int len;
        Vector3f temp = new Vector3f();
        
        writer.append("o "+mesh.getName()).append("\r\n");
        
        Spatial s = mesh;
        writer.append("g ");
        while (s != null){
            writer.append(s.getName());
            s = s.getParent();
            if (s != null) writer.append(' ');
        }
        writer.append("\r\n");
        
        int vertsWritten = 0;
        // Vertex data
        if (vb!=null){
            len = vb.limit() / 3;
            vb.rewind();
            for (int i = 0; i < len; i++ ){
                temp.set(vb.get(), vb.get(), vb.get());
                mesh.localToWorld(temp, temp);
                writer.append("v ")
                  .append(Float.toString(temp.x))
                  .append(' ')
                  .append(Float.toString(temp.y))
                  .append(' ')
                  .append(Float.toString(temp.z))
                  .append("\r\n");
                vertsWritten++;
            }

            writer.append("\r\n");
        }
        
        
        // Texture coordinates
        if (tb!=null){
            FloatBuffer tc = tb.coords;
            int perVert = tb.perVert;
            
            len = tc.limit() / perVert;
            tc.rewind();
            for (int i = 0; i < len; i++){
                writer.append("vt ");
                
                for (int j = 0; j < perVert; j++){
                    float coord = tc.get();
                    writer.append(Float.toString(coord));
                    if (j != perVert-1)
                        writer.append(' ');
                }
                
                writer.append("\r\n");
            }
            writer.append("\r\n");
        }
        
        // Normals
        if (nb!=null){
            len = nb.limit() / 3;
            nb.rewind();
            for (int i = 0; i < len; i++ ){
                temp.set(nb.get(), nb.get(), nb.get());
                mesh.getWorldRotation().mult(temp, temp);
                writer.append("vn ")
                  .append(Float.toString(temp.x))
                  .append(' ')
                  .append(Float.toString(temp.y))
                  .append(' ')
                  .append(Float.toString(temp.z))
                  .append("\r\n");
            }
            writer.append("\r\n");
        }
        
        if (hasMaterial){
            writer.append("usemtl ").append(mesh.getName()).append("\r\n");
        }
        
        // Face Indexes
        if (ib!=null){
            len = ib.limit() / 3;
            ib.rewind();
            for (int i = 0; i < len; i++){
                String v1 = Integer.toString(vertOffset+ib.get());
                String v2 = Integer.toString(vertOffset+ib.get()); 
                String v3 = Integer.toString(vertOffset+ib.get());

                if (tb!=null && nb==null){
                    writer.append("f ")
                        .append(v1).append("/").append(v1)
                        .append(' ')
                        .append(v2).append("/").append(v2)
                        .append(' ')
                        .append(v3).append("/").append(v3)
                        .append("\r\n");
                }else if (tb==null && nb!=null){
                    writer.append("f ")
                        .append(v1).append("//").append(v1)
                        .append(' ')
                        .append(v2).append("//").append(v2)
                        .append(' ')
                        .append(v3).append("//").append(v3)
                        .append("\r\n");
                }else if (tb!=null && nb!=null){
                    writer.append("f ")
                        .append(v1).append("/").append(v1).append("/").append(v1)
                        .append(' ')
                        .append(v2).append("/").append(v2).append("/").append(v2)
                        .append(' ')
                        .append(v3).append("/").append(v3).append("/").append(v3)
                        .append("\r\n");
                }else{
                    writer.append("f ")
                        .append(v1)
                        .append(' ')
                        .append(v2)
                        .append(' ')
                        .append(v3)
                        .append("\r\n");
                }
                
            }
            writer.append("\r\n");
        }
        
        vertOffset += vertsWritten;
    }
    
}