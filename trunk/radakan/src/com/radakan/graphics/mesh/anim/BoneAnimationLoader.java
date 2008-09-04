/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package com.radakan.graphics.mesh.anim;

import com.jme.math.Quaternion;
import com.jme.math.TransformMatrix;
import com.jme.math.Vector3f;
import com.jme.scene.state.GLSLShaderObjectsState;
import com.jme.system.DisplaySystem;
import java.nio.ByteBuffer;
import java.nio.FloatBuffer;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;
import org.w3c.dom.Node;

import static com.radakan.util.XMLUtil.*;

/**
 *
 * @author Kirill
 */
public class BoneAnimationLoader {

    public static String applySkinningShader(String shader, int numBones, int maxWeightsPerVert){
        shader = shader.replace("hw_skin_vars", "attribute vec4 weights;\n" +
                                       "attribute vec4 indexes;\n" +
                                       "uniform mat4 boneMatrices["+numBones+"];\n");
        shader = shader.replace("hw_skin_compute", 
                                          "    vec4 index = indexes;\n" +
                                          "    vec4 weight = weights;\n" +
                                          "\n" +
                                          "    vec4 vPos = vec4(0.0);\n" +
                                          "    vec4 vNormal = vec4(0.0);\n" +
                                          "    vec4 normal = vec4(gl_Normal.xyz,0.0);\n" +
                                          "\n" +
                                          "    for (float i = 0.0; i < "+((float)maxWeightsPerVert)+"; i += 1.0){\n" +
                                          "        mat4 skinMat = boneMatrices[int(index.x)];\n" +
                                          "        vPos    += weight.x * (skinMat * gl_Vertex);\n" +
                                          "        vNormal += weight.x * (skinMat * normal);\n" +
                                          "        index = index.yzwx;\n" +
                                          "        weight = weight.yzwx;\n" +
                                          "    }\n" +
                                          "\n");
//        shader = shader.replace("hw_skin_compute", 
//                                          "    vec4 vPos = boneMatrices[int(indexes.x)] * gl_Vertex;\n" +
//                                          "\n");
        shader = shader.replace("hw_skin_vpos", "(gl_ModelViewProjectionMatrix * vPos)");
        shader = shader.replace("hw_skin_vnorm", "(normalize(inverseModelView * tempNormal).xyz)");
        
        return shader;
    }
    
    public static GLSLShaderObjectsState createSkinningShader(int numBones, int maxWeightsPerVert){
        GLSLShaderObjectsState shader = DisplaySystem.getDisplaySystem().getRenderer().createGLSLShaderObjectsState();
        String str =    "hw_skin_vars\n" +
                        "\n" +
                        "void main(){\n" +
                        "   hw_skin_compute;\n" +
                        "\n" +
                        "   gl_TexCoord[0] = gl_MultiTexCoord0;\n" +
                        "   gl_FrontColor = gl_LightSource[0].ambient;\n" +
                        "   //vPos = gl_Vertex;\n" +
                        "   gl_Position = hw_skin_vpos;\n" +
                        "}\n";
        //hw_skin_compute
        //hw_skin_vars
        str = applySkinningShader(str, numBones, maxWeightsPerVert);
        //System.out.println(str);
        shader.load(str, null);
        return shader;
    }
    
    private static class VertexBoneAssignments {
        
        byte[] indexes = new byte[4];
        float[] weights = new float[4];
        int written = 0;
        
        VertexBoneAssignments(byte index, float weight){
            push(index, weight);
        }
        
        void push(byte index, float weight){
            assert written < 4;
            indexes[written] = index;
            weights[written] = weight;
            written++;
        }
    }
    
    public static BoneAnimation loadAnimation(Node animationNode, Skeleton skeleton){
        // read the name and length (in seconds) of the animation
        String name = getAttribute(animationNode, "name");
        float length = getFloatAttribute(animationNode, "length");

        // list to store keyframes from ALL tracks
        // they will be sorted and later added onto the BoneAnimation
        List<BoneTrack> tracks = new ArrayList<BoneTrack>();
        
        // the bones that have animation tracks associated with them
        // animation processing will only effect those bones
        Set<Bone> bonesWithTracks = new HashSet<Bone>();
        
        // skeleton -> animations -> animation -> tracks
        Node tracksNode = getChildNode(animationNode, "tracks");
        Node trackNode = tracksNode.getFirstChild();
        while (trackNode != null){
            if (!trackNode.getNodeName().equals("track")){
                trackNode = trackNode.getNextSibling();
                continue;
            }
            
            ArrayList<Float> times = new ArrayList<Float>();
            ArrayList<Vector3f> translations = new ArrayList<Vector3f>();
            ArrayList<Quaternion> rotations = new ArrayList<Quaternion>();
            
            Bone bone = skeleton.getBone(getAttribute(trackNode, "bone"));
            bonesWithTracks.add(bone);
            
            // tracks -> keyframes -> keyframe
            Node keyframe = getChildNode(trackNode, "keyframes").getFirstChild();
            while (keyframe != null){
                if (!keyframe.getNodeName().equals("keyframe")){
                    keyframe = keyframe.getNextSibling();
                    continue;
                }

                Node translate = getChildNode(keyframe, "translate");
                Node rotate    = getChildNode(keyframe, "rotate");
                Node scale     = getChildNode(keyframe, "scale");
                
                float time = getFloatAttribute(keyframe, "time");
                Vector3f pos;
                if (translate != null){
                    // FIXME: The Z and Y axis are flipped here!!
                    pos = new Vector3f(getFloatAttribute(translate,"x"),
                                       getFloatAttribute(translate,"y"),
                                       getFloatAttribute(translate,"z"));
                }else{
                    pos = new Vector3f(0,0,0);
                }
                
                Quaternion rot;
                if (rotate != null){
                    rot = new Quaternion();
                    Node raxis = getChildNode(rotate, "axis");
                    // FIXME: The Z and Y axis are flipped here!!
                    Vector3f axis = new Vector3f(getFloatAttribute(raxis, "x"),
                                                 getFloatAttribute(raxis, "y"),
                                                 getFloatAttribute(raxis, "z"));
                    float angle = getFloatAttribute(rotate, "angle");
                    rot.fromAngleAxis(angle, axis);
                }else{
                    rot = new Quaternion();
                }
                
                // Scale doesn't do anything !?
                Vector3f scal;
                if (scale != null){
                    if (getAttribute(scale, "factor") != null){
                        float factor = Float.parseFloat(getAttribute(scale, "factor"));
                        scal = new Vector3f(factor, factor, factor);
                    }else{
                        scal = new Vector3f(getFloatAttribute(scale,"x"),
                                             getFloatAttribute(scale,"y"),
                                             getFloatAttribute(scale,"z"));
                    }
                }else{
                    scal = new Vector3f(1,1,1);
                }
                
                times.add(time);
                translations.add(pos);
                rotations.add(rot);
                
                keyframe = keyframe.getNextSibling();
            }
            
            float[] timesArray = new float[times.size()];
            for (int i = 0; i < timesArray.length; i++)
                timesArray[i] = times.get(i);
            
            int targetBoneIndex = skeleton.getBoneIndex(bone);
            BoneTrack bTrack = new BoneTrack(targetBoneIndex,
                                             timesArray,
                                             translations.toArray(new Vector3f[0]),
                                             rotations.toArray(new Quaternion[0]));
            
            tracks.add(bTrack);
            
            trackNode = trackNode.getNextSibling();
        }

        BoneAnimation anim = new BoneAnimation(name, length, tracks.toArray(new BoneTrack[0]));
        System.out.println("ANIM("+name+", len="+length+")");
        
        return anim;
    }
    
    public static void loadAnimations(Node animationsNode, Skeleton skeleton, Map<String, Animation> store){
        Node animationNode = animationsNode.getFirstChild();
        while (animationNode != null){
            if (!animationNode.getNodeName().equals("animation")){
                animationNode = animationNode.getNextSibling();
                continue;
            }

            BoneAnimation bAnim = loadAnimation(animationNode, skeleton);
            Animation anim = store.get(bAnim.getName());
            if (anim == null){
                anim = new Animation(bAnim, null);
                store.put(bAnim.getName(), anim);
            }else{
                anim.setBoneAnimation(bAnim);
            }

            animationNode = animationNode.getNextSibling();
        }
    }
    
    public static WeightBuffer loadWeightBuffer(Node assignmentsNode, int vertexCount){
        WeightBuffer weightBuffer = new WeightBuffer(vertexCount);
        
        ByteBuffer ib = weightBuffer.indexes;
        FloatBuffer wb = weightBuffer.weights;
        
        Map<Integer, VertexBoneAssignments> assignments = new HashMap<Integer, VertexBoneAssignments>();
        
        Node vbassignNode = assignmentsNode.getFirstChild();
        while (vbassignNode != null){
            if (vbassignNode.getNodeName().equals("vertexboneassignment")){
                // fetch data from XML node
                int vertIndex  = getIntAttribute(vbassignNode, "vertexindex");
                byte boneIndex = (byte) getIntAttribute(vbassignNode, "boneindex");
                float weight   = getFloatAttribute(vbassignNode, "weight");

                // assign data
                VertexBoneAssignments assign = assignments.get(vertIndex);
                if (assign == null){
                    assignments.put(vertIndex, new VertexBoneAssignments(boneIndex, weight));
                }else{
                    assign.push(boneIndex, weight);
                }
            }

            vbassignNode = vbassignNode.getNextSibling();
        }
        
        for (Entry<Integer, VertexBoneAssignments> assign : assignments.entrySet()){
            VertexBoneAssignments val = assign.getValue();
            
            ib.put(assign.getKey() * 4,     val.indexes[0]);
            ib.put(assign.getKey() * 4 + 1, val.indexes[1]);
            ib.put(assign.getKey() * 4 + 2, val.indexes[2]);
            ib.put(assign.getKey() * 4 + 3, val.indexes[3]);
            
            wb.put(assign.getKey() * 4,     val.weights[0]);
            wb.put(assign.getKey() * 4 + 1, val.weights[1]);
            wb.put(assign.getKey() * 4 + 2, val.weights[2]);
            wb.put(assign.getKey() * 4 + 3, val.weights[3]);
        }
        
        ib.rewind();
        wb.rewind();
        
        return weightBuffer;
    }
    
}
