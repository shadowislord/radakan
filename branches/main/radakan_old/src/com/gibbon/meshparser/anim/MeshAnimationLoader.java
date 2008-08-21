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

package com.gibbon.meshparser.anim;

import com.gibbon.jme.context.JmeContext;
import com.gibbon.meshparser.*;
import com.gibbon.meshparser.anim.PoseTrack.PoseFrame;
import com.jme.math.Vector3f;
import com.jme.scene.TriMesh;
import com.jme.scene.state.GLSLShaderObjectsState;
import com.jmex.terrain.TerrainPage;
import java.util.ArrayList;
import java.util.List;
import org.w3c.dom.Node;

/**
 * Utility class used by OgreLoader to load poses and mesh animations.
 */
public class MeshAnimationLoader {
    
    public static String applySkinningShader(String shader){
        shader = shader.replace("hw_skin_vars", "attribute vec4 weights;\n" +
                                       "attribute vec4 matrixIndices;\n" +
                                       "uniform mat4 BoneMatrices[256];\n");
        shader = shader.replace("hw_skin_compute", "    vec4 index = matrixIndices;\n" +
                                          "    vec4 weight = weights;\n" +
                                          "\n" +
                                          "    vec4 vPos = vec4(0.0);\n" +
                                          "    vec4 vNormal = vec4(0.0);\n" +
                                          "    vec4 normal = vec4(gl_Normal.xyz,0.0);\n" +
                                          "\n" +
                                          "    for (float i = 0.0; i < 4; i += 1.0){\n" +
                                          "        mat4 skinMat = boneMatrices[int(index.x)];\n" +
                                          "        vPos    += weight.x * skinMat * gl_Vertex;\n" +
                                          "        vNormal += weight.x * skinMat * normal;\n" +
                                          "        index = index.yzwx;\n" +
                                          "        weight = weight.yzwx;\n" +
                                          "    }\n" +
                                          "\n");
        shader = shader.replace("hw_skin_vpos", "(gl_ModelViewProjectionMatrix * vPos)");
        shader = shader.replace("hw_skin_vnorm", "(normalize(inverseModelView * tempNormal).xyz)");
        
        return shader;
    }
    
    public static GLSLShaderObjectsState createSkinningShader(){
        GLSLShaderObjectsState shader = JmeContext.get().getRenderer().createGLSLShaderObjectsState();
        String str = "hw_skin_vars\n" +
                        "\n" +
                        "void main(){" +
                        "   hw_skin_compute" +
                        "   gl_Position = hw_skin_vpos;" +
                        "}";
        str = applySkinningShader(str);
        shader.load(str, null);
        return shader;
    }
    
    public static MeshAnimation loadMeshAnimation(Node animationNode, List<Pose> poseList, TriMesh sharedgeom, List<TriMesh> submeshes){
        String name =  XMLUtil.getAttribute(animationNode, "name");
        float length = XMLUtil.getFloatAttribute(animationNode, "length");
        
        MeshAnimation anim = new MeshAnimation(name, length);
        List<Track> tracks = new ArrayList<Track>();
        
        Node tracksNode = XMLUtil.getChildNode(animationNode, "tracks");
        if (tracksNode != null){
            Node trackNode = tracksNode.getFirstChild();
            while (trackNode != null){
                if (trackNode.getNodeName().equals("track")){
                    TriMesh target;
                    if (XMLUtil.getAttribute(trackNode, "target").equals("mesh")){
                        target = sharedgeom;
                    }else{
                        if (XMLUtil.getAttribute(trackNode, "index") == null)
                            target = submeshes.get(0);
                        else
                            target = submeshes.get(XMLUtil.getIntAttribute(trackNode, "index"));
                    }
                    
                    if (XMLUtil.getAttribute(trackNode, "type").equals("pose")){
                        PoseTrack pt = loadPoseTrack(trackNode, target, poseList);
                        tracks.add(pt);
                    }else{
                        throw new UnsupportedOperationException("Morph animations not supported!");
                    }
                }
                
                trackNode = trackNode.getNextSibling();
            }
        }
        
        anim.setTracks(tracks.toArray(new Track[0]));
        
        return anim;
    }
    
    public static Pose loadPose(Node poseNode, TriMesh target){
        String name = XMLUtil.getAttribute(poseNode, "name");
        Pose pose = new Pose(name, target);
        
        List<Vector3f> offsets = new ArrayList<Vector3f>();
        List<Integer>  indices = new ArrayList<Integer>();
        
        Node poseoffsetNode = poseNode.getFirstChild();
        while (poseoffsetNode != null){
            if (poseoffsetNode.getNodeName().equals("poseoffset")){
                int vertIndex = XMLUtil.getIntAttribute(poseoffsetNode, "index");
                Vector3f offset = new Vector3f();
                offset.x = XMLUtil.getFloatAttribute(poseoffsetNode, "x");
                offset.y = XMLUtil.getFloatAttribute(poseoffsetNode, "y");
                offset.z = XMLUtil.getFloatAttribute(poseoffsetNode, "z");
                
                offsets.add(offset);
                indices.add(vertIndex);
            }
            
            poseoffsetNode = poseoffsetNode.getNextSibling();
        }
        
        int[] indicesArray = new int[indices.size()];
        for (int i = 0; i < indicesArray.length; i++){
            indicesArray[i] = indices.get(i);
        }
        
        pose.setData(offsets.toArray(new Vector3f[0]),
                     indicesArray);
        
        return pose;
    }
    
    public static PoseTrack loadPoseTrack(Node trackNode, TriMesh target, List<Pose> posesList){
        PoseTrack track = new PoseTrack(target);
        
        List<Float> times = new ArrayList<Float>();
        List<PoseFrame> frames = new ArrayList<PoseFrame>();
        
        Node keyframesNode = XMLUtil.getChildNode(trackNode, "keyframes");
        Node keyframeNode = keyframesNode.getFirstChild();
        while (keyframeNode != null){
            if (keyframeNode.getNodeName().equals("keyframe")){
                float time = XMLUtil.getFloatAttribute(keyframeNode, "time");
                List<Pose> poses = new ArrayList<Pose>();
                List<Float> weights = new ArrayList<Float>();
                
                Node poserefNode = keyframeNode.getFirstChild();
                while (poserefNode != null){
                    if (poserefNode.getNodeName().equals("poseref")){
                        int poseindex = XMLUtil.getIntAttribute(poserefNode, "poseindex");
                        poses.add(posesList.get(poseindex));
                        float weight = XMLUtil.getFloatAttribute(poserefNode, "influence");
                        weights.add(weight);
                    }
                    
                    poserefNode = poserefNode.getNextSibling();
                }
                
                // convert poses and weights to arrays and create a PoseFrame
                PoseFrame frame = new PoseFrame();
                frame.poses = poses.toArray(new Pose[0]);
                frame.weights = new float[weights.size()];
                for (int i = 0; i < frame.weights.length; i++){
                    frame.weights[i] = weights.get(i);
                }
                
                times.add(time);
                frames.add(frame);
            }
            
            keyframeNode = keyframeNode.getNextSibling();
        }
        
        // convert times and frames to arrays and write to the track
        float[] timesArray = new float[times.size()];
        for (int i = 0; i < timesArray.length; i++){
            timesArray[i] = times.get(i);
        }
        
        track.setData(timesArray, frames.toArray(new PoseFrame[0]));
        
        return track;
    }
    
}