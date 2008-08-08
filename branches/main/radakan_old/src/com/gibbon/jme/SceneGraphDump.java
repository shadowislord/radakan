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

package com.gibbon.jme;

import com.jme.bounding.BoundingBox;
import com.jme.bounding.BoundingCapsule;
import com.jme.bounding.BoundingSphere;
import com.jme.bounding.BoundingVolume;
import com.jme.bounding.OrientedBoundingBox;
import com.jme.image.Image;
import com.jme.image.Image.Format;
import com.jme.image.Texture;
import com.jme.image.Texture.CombinerOperandRGB;
import com.jme.image.Texture.CombinerSource;
import com.jme.math.FastMath;
import com.jme.math.Quaternion;
import com.jme.math.Vector3f;
import com.jme.renderer.ColorRGBA;
import com.jme.renderer.Renderer;
import com.jme.scene.Geometry;
import com.jme.scene.Node;
import com.jme.scene.Spatial;
import com.jme.scene.state.BlendState;
import com.jme.scene.state.CullState;
import com.jme.scene.state.GLSLShaderObjectsState;
import com.jme.scene.state.LightState;
import com.jme.scene.state.RenderState;
import com.jme.scene.state.TextureState;
import java.util.Arrays;

/**
 * This class is mostly commented out as the code doesn't work with jME2.0
 * 
 * @author Momoko_Fan
 */
public final class SceneGraphDump {

    private SceneGraphDump(){
    }
    
    private static final String SPACES;
    static {
        char c[] = new char [256];
        Arrays.fill(c,' ');
        SPACES = new String(c);
    }
    
    private static float[] angles = new float [3];
    
    private static String stringRotation (Quaternion q){
       synchronized (angles){
        q.toAngles(angles);
        return "["+
                (FastMath.RAD_TO_DEG*angles[0])+", "+
                (FastMath.RAD_TO_DEG*angles[1])+", "+
                (FastMath.RAD_TO_DEG*angles[2])+"]";
       }
    }
    private static String stringTranslation (Vector3f v){
        return "("+v.x+", "+v.y+", "+v.z+")";
    }
    private static String stringScale (Vector3f v){
        return "{"+((v.x+v.y+v.z)/3)+"}";
    }

    private static String stringRenderStates (Spatial s){
        StringBuffer sb = new StringBuffer();
        if (s.getRenderState(RenderState.RS_BLEND)!=null) sb.append("A");
        if (s.getRenderState(RenderState.RS_CULL)!=null) sb.append("C");
        if (s.getRenderState(RenderState.RS_FOG)!=null) sb.append("F");
        if (s.getRenderState(RenderState.RS_COLORMASK_STATE)!=null) sb.append("Cm");
        if (s.getRenderState(RenderState.RS_SHADE)!=null) sb.append("Sh");
        if (s.getRenderState(RenderState.RS_STENCIL)!=null) sb.append("St");
        if (s.getRenderState(RenderState.RS_FRAGMENT_PROGRAM)!=null) sb.append("Fp");
        if (s.getRenderState(RenderState.RS_GLSL_SHADER_OBJECTS)!=null) sb.append("Glsl");
        if (s.getRenderState(RenderState.RS_LIGHT)!=null) sb.append("L");
        if (s.getRenderState(RenderState.RS_MATERIAL)!=null) sb.append("M");
        if (s.getRenderState(RenderState.RS_TEXTURE)!=null) sb.append("T");
        if (s.getRenderState(RenderState.RS_VERTEX_PROGRAM)!=null) sb.append("Vp");
        if (s.getRenderState(RenderState.RS_WIREFRAME)!=null) sb.append("W");
        if (s.getRenderState(RenderState.RS_ZBUFFER)!=null) sb.append("Z");
        
        if (sb.length()==0)
            return null;
        
        return sb.toString();
    }

    private static String stringLocks (int locks){
        StringBuffer sb = new StringBuffer();
        if ((locks & Spatial.LOCKED_BOUNDS) == Spatial.LOCKED_BOUNDS) sb.append("Vol");
        if ((locks & Spatial.LOCKED_BRANCH) == Spatial.LOCKED_BRANCH) sb.append("Brn");
        if ((locks & Spatial.LOCKED_MESH_DATA) == Spatial.LOCKED_MESH_DATA) sb.append("Msh");
        if ((locks & Spatial.LOCKED_SHADOWS) == Spatial.LOCKED_SHADOWS) sb.append("Shd");
        if ((locks & Spatial.LOCKED_TRANSFORMS) == Spatial.LOCKED_TRANSFORMS) sb.append("Trs");
        return sb.toString();
    }
    private static String stringRenderQueue (int queueMode){
        switch (queueMode){
            case Renderer.QUEUE_INHERIT: return "inherit";
            case Renderer.QUEUE_OPAQUE: return "opaque";
            case Renderer.QUEUE_ORTHO: return "ortho";
            case Renderer.QUEUE_SKIP: return "skip";
            case Renderer.QUEUE_TRANSPARENT: return "transparent";
            default: return "?";
        }
    }

    private static String stringClass(Class<?> clazz){
        return clazz.isAnonymousClass() ? clazz.getName() : clazz.getSimpleName();
    }
    
    private static String stringBlendColor(ColorRGBA blend, CombinerOperandRGB operation){
        if (operation == CombinerOperandRGB.SourceColor)
            return "vec3("+blend.r+", "+blend.g+", "+blend.b+")";
        else if (operation == CombinerOperandRGB.SourceAlpha)
            return ""+blend.a;
        else
            return "?";
    }
    
    private static String stringTexture(int texN, int unit, boolean alpha){
        return "texture2D(tex"+texN+", gl_TexCoord["+texN+"].st)" + (alpha ? ".a" : ".rgb");
    }
    
//    public static String convertCombineArgument(CombinerSource source, CombinerOperandRGB operation, int unit, ColorRGBA blend){
//        String arg;
//        switch (source){
//            case Constant:
//                return stringBlendColor(blend,operation);
//            case Previous:
//                arg = "r";
//                break;
//            case PrimaryColor:
//                arg = "gl_Color";
//                break;
//            case CurrentTexture:
//                arg = "texture2D(tex"+unit+", gl_TexCoord["+unit+"].st)";
//                break;
//            default:
//                if (source >= Texture.ACS_TEXTURE0 && source <= Texture.ACS_TEXTURE31){
//                    arg = "texture2D(tex"+(source - Texture.ACS_TEXTURE0)+", gl_TexCoord["+unit+"])";
//                }else{
//                    arg = "???";
//                }
//        }
//        switch (operation){
//            case Texture.ACO_SRC_COLOR:
//                return arg+".rgb";
//            case Texture.ACO_ONE_MINUS_SRC_COLOR:
//                return "1.0 - "+arg+".rgb";
//            case Texture.ACO_SRC_ALPHA:
//                return arg+".a";
//            case Texture.ACO_ONE_MINUS_SRC_ALPHA:
//                return "1.0 - "+arg+".a";
//            default:
//                return arg+".?";
//        }
//    }
//    
//    public static String convertCombineFunc(int func, String[] args){
//        switch (func){
//            case Texture.ACF_ADD:
//                return args[0]+" + "+args[1];
//            case Texture.ACF_ADD_SIGNED:
//                return args[0]+" + "+args[1]+" - 0.5";
//            case Texture.ACF_DOT3_RGB:
//            case Texture.ACF_DOT3_RGBA:
//                return "dot("+args[0]+", "+args[1]+")";
//            case Texture.ACF_INTERPOLATE:
//                return "mix("+args[1]+", "+args[0]+", "+args[2]+")";
//            case Texture.ACF_MODULATE:
//                return args[0]+" * "+args[1];
//            case Texture.ACF_REPLACE:
//                return args[0];
//            case Texture.ACF_SUBTRACT:
//                return args[0]+" - "+args[1];
//            default:
//                return "???";
//        }
//    }
//    
//    public static String convertCombineModes(Texture t, int unit){
//        if (t.getApply()!=Texture.AM_COMBINE){
//            switch (t.getApply()){
//                case Texture.AM_ADD: return "r.rgb = r.rgb + "+stringTexture(unit,unit,false)+";\n" +
//                                            "r.a = r.a * "+stringTexture(unit,unit,true)+";\n";
//                case Texture.AM_BLEND: return "r.rgb = mix(r.rgb, "+stringBlendColor(t.getBlendColor(), Texture.ACO_SRC_COLOR)+", "+stringTexture(unit,unit,false)+");\n" +
//                                              "r.a = r.a * "+stringTexture(unit,unit,true)+";\n";
//                case Texture.AM_DECAL: return "r.rgb = mix(r.rgb, "+stringTexture(unit,unit,false)+", "+stringTexture(unit,unit,true)+");\n" +
//                                              "r.a = r.a;\n";
//                case Texture.AM_MODULATE: return "r.rgb = r.rgb * "+stringTexture(unit,unit,false)+";\n" +
//                                                 "r.a = r.a * "+stringTexture(unit,unit,true)+";\n";
//                case Texture.AM_REPLACE: return "r.rgb = "+stringTexture(unit,unit,false)+";\n" +
//                                                "r.a = "+stringTexture(unit,unit,true)+";\n";
//                default:
//                    return "// ???\n";
//            }
//        }else{
//            ColorRGBA blend = t.getBlendColor();
//            
//            String[] args = new String[3];
//            args[0] = convertCombineArgument(t.getCombineSrc0RGB(), t.getCombineOp0RGB(), unit, blend);
//            args[1] = convertCombineArgument(t.getCombineSrc1RGB(), t.getCombineOp1RGB(), unit, blend);
//            args[2] = convertCombineArgument(t.getCombineSrc2RGB(), t.getCombineOp2RGB(), unit, blend);
//            
//            String out = "r.rgb = "+convertCombineFunc(t.getCombineFuncRGB(), args);
//            if (t.getCombineScaleRGB() != 1.0f){
//                out += " * "+t.getCombineScaleRGB();
//            }
//            out += ";\n";
//            
//            args[0] = convertCombineArgument(t.getCombineSrc0Alpha(), t.getCombineOp0Alpha(), unit, blend);
//            args[1] = convertCombineArgument(t.getCombineSrc1Alpha(), t.getCombineOp1Alpha(), unit, blend);
//            args[2] = convertCombineArgument(t.getCombineSrc2Alpha(), t.getCombineOp2Alpha(), unit, blend);
//            
//            out = out +"r.a = "+convertCombineFunc(t.getCombineFuncAlpha(), args);
//            if (t.getCombineScaleAlpha() != 1.0f){
//                out += " * "+t.getCombineScaleAlpha();
//            }
//            out += ";\n";
//            
//            return out;
//        }
//    }
//    
//    private static void dumpSceneElement (SceneElement s){
//        System.out.println(stringClass(s.getClass())+": "+s.getName());
//        
//        if (s.getCullMode()!=SceneElement.CULL_DYNAMIC)
//            System.out.println("CullMode: "+stringCullMode(s.getCullMode()));
//        
//        if (s.getTextureCombineMode()!=TextureState.COMBINE_CLOSEST)
//            System.out.println("TexMode: "+stringTCombine(s.getTextureCombineMode()));
//        
//        if (s.getLightCombineMode()!=LightState.COMBINE_FIRST)
//            System.out.println("LightMode: "+stringLCombine(s.getLightCombineMode()));
//        
//        if (s.getNormalsMode()!=SceneElement.NM_GL_NORMALIZE_IF_SCALED)
//            System.out.println("NormalMode: "+stringNormalMode(s.getNormalsMode()));
//        
//        String states = stringRenderStates(s);
//        if (states != null) System.out.println("RenderStates: "+states);
//        
//        for (int i = 0; i < RenderState.RS_MAX_STATE-1; i++){
//            RenderState rs = s.getRenderState(i);
//            if (rs==null) continue;
//            dumpRenderState(rs);
//        }
//        
//        if (s.getLocks()!=0)
//            System.out.println("Locks: "+stringLocks(s.getLocks()));
//    }
//    private static void dumpSpatial (Spatial s){
//        dumpSceneElement(s);
//        if (!s.getWorldTranslation().equals(Vector3f.ZERO)
//         || !s.getWorldScale().equals(new Vector3f(1f,1f,1f))
//         || !s.getWorldRotation().equals(new Quaternion())){
//            System.out.println("Transform: "+stringTranslation(s.getWorldTranslation())+
//                                             stringRotation(s.getWorldRotation())+
//                                             stringScale(s.getWorldScale()));
//        }
//        if (s.getRenderQueueMode()!=Renderer.QUEUE_SKIP){
//            System.out.println("RenderQueue: "+stringRenderQueue(s.getRenderQueueMode()));
//        }
//        if (s.getControllerCount()!=0)
//            System.out.println("Controllers: "+s.getControllers());
//        if (s.getZOrder()!=0)
//            System.out.println("ZOrder: "+s.getZOrder());
//        
//        if (s.getWorldBound() != null){
//            BoundingVolume bv = s.getWorldBound();
//            if (bv instanceof BoundingBox){
//                System.out.println("Bound: AABB");
//            }else if (bv instanceof BoundingSphere){
//                System.out.println("Bound: Sphere");
//            }else if (bv instanceof BoundingCapsule){
//                System.out.println("Bound: Capsule");
//            }else if (bv instanceof OrientedBoundingBox){
//                System.out.println("Bound: OBB");
//            }
//        }
//    }
//    private static void dumpNode (Node n){
//        dumpSpatial(n);
//        if (n.getChildren() != null && n.getChildren().size() != 0){
//            StringBuffer sb = new StringBuffer();
//            sb.append("Children: ");
//            for (Spatial s: n.getChildren()){
//                sb.append(stringClass(s.getClass())+"/"+s.getName()+"  ");
//            }
//            System.out.println(sb.toString());
//        }
//    }
    
//    private static void dumpTriangleBatch (TriangleBatch tb){
//        System.out.println("Triangles: "+tb.getTriangleCount());
//        if (tb.getMode()==TriangleBatch.TRIANGLE_FAN)
//            System.out.println("Triangle Mode: Fan");
//        else if (tb.getMode()==TriangleBatch.TRIANGLE_STRIP)
//            System.out.println("Triangle Mode: Strip");
//    }
//    private static void dumpBatch (GeomBatch gb){
//        System.out.println(".......................");
//        dumpSceneElement(gb);
//        
//        StringBuffer sb = new StringBuffer();
//        sb.append("Batches set: ");
//        
//        if (gb.getVertexBuffer()!=null) sb.append("V");
//        if (gb instanceof TriangleBatch){
//            TriangleBatch tb = (TriangleBatch) gb;
//            if (tb.getIndexBuffer()!=null) sb.append("I");
//        }
//        if (gb.getBinormalBuffer()!=null) sb.append("B");
//        if (gb.getNormalBuffer()!=null) sb.append("N");
//        if (gb.getTangentBuffer()!=null) sb.append("T");
//        if (gb.getColorBuffer()!=null) sb.append("C");
//        
//        for (int i = 0; i < gb.getTextureBuffers().size(); i++){
//            sb.append("T").append(i);
//        }
//        
//        if (gb.getVBOInfo()!=null) sb.append(" (VBO)");
//        if (gb.getDisplayListID()!=-1) sb.append(" (DisplayList)");
//        
//        System.out.println(sb.toString());
//
//        System.out.println("Vertex: "+gb.getVertexCount());
//        if (gb instanceof TriangleBatch)
//            dumpTriangleBatch((TriangleBatch)gb);
//    }
//    
//    private static void dumpGeometry (Geometry g){
//        dumpSpatial(g);
//        for (int i = 0; i < g.getBatchCount(); i++){
//            dumpBatch(g.getBatch(i));
//        }
//    }
    
//    public static String convertImage(Image img){
//        if (img == null)
//            return "NULL";
//        
//        StringBuffer sb  = new StringBuffer();
//        sb.append(stringImageType(img.getType())+"x"+img.getWidth()+"x"+img.getHeight());
//        return sb.toString();
//    }
    
//    public static void dumpTextureState(TextureState ts){
//        for (int i = 0; i < ts.getNumberOfSetTextures(); i++){
//            Texture t = ts.getTexture(i);
//            String loc = null;
//
//                if (t.getImageLocation()!=null && !t.getImageLocation().equals(""))
//                    loc = t.getImageLocation();
//
//            System.out.println(i+": "+loc+" - "+convertImage(t.getImage()));
//            System.out.println(convertCombineModes(t, i));
//        }
//    }
//    public static String convertBlendSrc(int blendSrc){
//        switch (blendSrc){
//            case AlphaState.SB_ONE:
//                return "1";
//            case AlphaState.SB_ZERO:
//                return "0";
//            case AlphaState.SB_SRC_ALPHA:
//                return "src.a";
//            case AlphaState.SB_DST_ALPHA:
//                return "dst.a";
//            case AlphaState.SB_DST_COLOR:
//                return "dst.rgb";
//            case AlphaState.SB_ONE_MINUS_SRC_ALPHA:
//                return "1 - src.a";
//            default:
//                return "?";
//        }
//    }
//    public static String convertBlendDst(int blendDst){
//        switch (blendDst){
//            case AlphaState.DB_ONE:
//                return "1";
//            case AlphaState.DB_ZERO:
//                return "0";
//            case AlphaState.DB_SRC_ALPHA:
//                return "src.a";
//            case AlphaState.DB_DST_ALPHA:
//                return "dst.a";
//            case AlphaState.DB_SRC_COLOR:
//                return "src.rgb";
//            case AlphaState.DB_ONE_MINUS_SRC_ALPHA:
//                return "1 - src.a";
//            default:
//                return "?";
//        }
//    }
//    
//    public static void dumpAlphaState(AlphaState as){
//        if (as == null)
//            return;
//        
//        if (as.isBlendEnabled()){
//            System.out.println("Blend("+convertBlendSrc(as.getSrcFunction())+", "+convertBlendDst(as.getDstFunction())+")");
//        }
//        if (as.isTestEnabled()){
//            
//        }
//    }
    
    private static void dumpGLSLState(GLSLShaderObjectsState glsl){
        System.out.println(glsl);
    }
    
//    private static void dumpRenderState(RenderState rs){
//        if (rs instanceof GLSLShaderObjectsState){
//            dumpGLSLState((GLSLShaderObjectsState)rs);
//        }else if (rs instanceof TextureState){
//            dumpTextureState((TextureState)rs);
//        }
//    }
//    
    public static void showLetterKeys(){
        System.out.println(
                "RenderState keys:\n" +
                "A 	- AlphaState\n" +
                "C 	- CullState\n" +
                "L 	- LightState\n" +
                "M 	- MaterialState\n" +
                "T 	- TextureState\n" +
                "Z 	- ZBufferState\n" +
                "F 	- FogState\n" +
                "St 	- StencilState\n" +
                "W 	- Wireframestate\n" +
                "Cm 	- ColorMaskState\n" +
                "D 	- DitherState\n" +
                "Sh 	- ShadeState\n" +
                "Fp 	- FragmentProgramState\n" +
                "Vp 	- VertexProgramState\n" +
                "Glsl 	- GLSLShaderObjectsState\n");
        System.out.println(
                "Geometry Buffer keys:\n"+
                "V	- Vertex\n"+
                "N	- Normal\n"+
                "C	- Color\n"+
                "T	- Tangent\n"+
                "B	- Binormal\n"+
                "I	- Index\n"+
                "T#	- Texture Coordinate\n");
                
    }
    
//    public static void dump (SceneElement se){
//        if (se instanceof Geometry){
//            dumpGeometry((Geometry)se);
//            System.out.println("----------------------------------------");
//        }
//        else if (se instanceof Node){
//            dumpNode((Node)se);
//            System.out.println("----------------------------------------");
//            Node n = (Node)se;
//            if (n.getChildren()==null) return;
//            for (Spatial spat: n.getChildren()){
//                dump(spat);
//            }
//        }
//        else{
//            dumpSceneElement(se);
//            System.out.println("----------------------------------------");
//        }
//    }
    
}
