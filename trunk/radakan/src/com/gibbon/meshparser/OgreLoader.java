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

package com.gibbon.meshparser;

import com.jme.animation.AnimationController;
import com.jme.animation.Bone;
import com.jme.animation.BoneAnimation;
import com.jme.animation.BoneTransform;
import com.jme.animation.SkinNode;
import com.jme.math.FastMath;
import com.jme.math.Matrix4f;
import com.jme.math.Quaternion;
import com.jme.math.Vector3f;
import com.jme.scene.Controller;
import com.jme.scene.Spatial;
import com.jme.scene.TexCoords;
import com.jme.scene.TriMesh;
import com.jme.scene.TriMesh.Mode;
import com.jme.util.geom.BufferUtils;

import com.jme.util.resource.ResourceLocatorTool;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;

import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import java.io.IOException;
import java.net.URL;
import java.nio.FloatBuffer;
import java.nio.IntBuffer;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import org.w3c.dom.NamedNodeMap;
import org.xml.sax.SAXException;

/**
 * Loads Ogre MESH.XML and SKELETON.XML files<br/>
 * 
 * You must call OgreLoader.setMaterials otherwise OgreLoader won't
 * find the materials.
 * 
 * @author Momoko_Fan
 */
public class OgreLoader {

    private static final Logger logger = Logger.getLogger(OgreLoader.class.getName());
    
    private TriMesh sharedgeom = new TriMesh("shared");
    private List<TriMesh> submeshes = new ArrayList<TriMesh>();
    private Bone rootBone;
    private Bone[] boneList;
    private SkinNode skinnode;
    private com.jme.scene.Node rootnode;
    private Map<String, Material> materialMap;
    private boolean Z_up_to_Y_up = true;
    
    private static final Matrix4f convertMatrix;
    private static final boolean DEBUG = true;
    
    static {
        convertMatrix = new Matrix4f();
        convertMatrix.fromAngleAxis(FastMath.HALF_PI, Vector3f.UNIT_X);
        Matrix4f rot2 = new Matrix4f();
        rot2.fromAngleAxis(FastMath.PI, Vector3f.UNIT_Z);
        convertMatrix.multLocal(rot2);
    }
    
    public void println(String str){
        if (DEBUG)
            System.out.println(str);
    }
    
    private Node getChildNode(Node node, String name) {
        Node child = node.getFirstChild();
        while (child != null && !child.getNodeName().equals(name) ){
            child = child.getNextSibling();
        }
        return child;
    }
    
    private String getAttribute(Node node, String name){
        Node att = node.getAttributes().getNamedItem(name);
        return att == null ? null : att.getNodeValue();
    }
    
    private void printAttributes(Node node){
        NamedNodeMap map = node.getAttributes();
        for (int i = 0; i < map.getLength(); i++){
            println("ATTRIB: "+map.item(i).getNodeName());
        }
    }
    
    private float getFloatAttribute(Node node, String name){
        return Float.parseFloat(getAttribute(node,name));
    }
    
    private void applyMaterial(String name, Spatial target){
        if (name.contains(".")){
            int index = name.lastIndexOf(".");
            name = name.substring(0, index);
        }
        
        if (materialMap == null)
            return;
        
        Material mat = materialMap.get(name);
        if (mat != null){
            mat.apply(target);
        }else{
            throw new IllegalStateException("Cannot find material: "+name);
        }
    }
    
    public void setMaterials(Map<String, Material> materials){
        materialMap = materials;
    }
    
    public Spatial loadModel(URL url, boolean Z_up){
        try {
            Z_up_to_Y_up = Z_up;
            
            DocumentBuilder builder = DocumentBuilderFactory.newInstance().newDocumentBuilder();
            Document doc = builder.parse(url.openStream());

            println("MESH("+url.getFile()+")");
            
            return loadMesh(doc);
        } catch (ParserConfigurationException ex) {
            Logger.getLogger(OgreLoader.class.getName()).log(Level.SEVERE, null, ex);
        } catch (SAXException ex) {
            Logger.getLogger(OgreLoader.class.getName()).log(Level.SEVERE, null, ex);
        } catch (IOException ex) {
            Logger.getLogger(OgreLoader.class.getName()).log(Level.SEVERE, null, ex);
        }
        
        return null;
    }
    
    private static class Keyframe implements Comparable<Keyframe> {
        Bone bone;
        float time;
        Vector3f translate;
        Quaternion rotate;
        Vector3f scale;

        public int compareTo(Keyframe other) {
            if (this.time > other.time)
                return 1;
            else if (this.time < other.time)
                return -1;
            else
                return 0;
        }
        
        @Override
        public String toString(){
            return "KEY(bone="+bone.getName()+", t="+time+")";
        }
        
    }
    
    private Bone loadSkeleton(URL url){
        try {
            DocumentBuilder builder = DocumentBuilderFactory.newInstance().newDocumentBuilder();
            Document doc = builder.parse(url.openStream());

            NodeList nodes = doc.getElementsByTagName("skeleton");
            
            println("SKELETON("+url.getFile()+")");
            
            return loadSkeleton(nodes.item(0));
        } catch (ParserConfigurationException ex) {
            Logger.getLogger(OgreLoader.class.getName()).log(Level.SEVERE, null, ex);
        } catch (SAXException ex) {
            Logger.getLogger(OgreLoader.class.getName()).log(Level.SEVERE, null, ex);
        } catch (IOException ex) {
            Logger.getLogger(OgreLoader.class.getName()).log(Level.SEVERE, null, ex);
        }
        
        return null;
    }
    
    private BoneAnimation loadAnimation(Node animation, Map<String, Bone> boneMap){
        String name = getAttribute(animation, "name");
        float length = getFloatAttribute(animation, "length");

        BoneAnimation anim = new BoneAnimation(name);
        anim.setInterpolate(true);
        println("ANIM("+name+", len="+length+")");
        
        // list to store keyframes from ALL tracks
        // they will be sorted and later added onto the BoneAnimation
        List<Keyframe> frames = new ArrayList<Keyframe>();
        
        // the bones that have animation tracks associated with them
        // animation processing will only effect those bones
        Set<Bone> bonesWithTracks = new HashSet<Bone>();
        
        Node tracks = getChildNode(animation, "tracks");
        Node track = tracks.getFirstChild();
        while (track != null){
            if (!track.getNodeName().equals("track")){
                track = track.getNextSibling();
                continue;
            }
            
            Bone bone = boneMap.get(getAttribute(track, "bone"));
            Node keyframe = getChildNode(track, "keyframes").getFirstChild();
            
            bonesWithTracks.add(bone);
            
            while (keyframe != null){
                if (!keyframe.getNodeName().equals("keyframe")){
                    keyframe = keyframe.getNextSibling();
                    continue;
                }

                Node translate = getChildNode(keyframe, "translate");
                Node rotate    = getChildNode(keyframe, "rotate");
                Node scale     = getChildNode(keyframe, "scale");
                
                Keyframe frame = new Keyframe();
                frame.time = getFloatAttribute(keyframe, "time");
                frame.bone = bone;
                if (translate != null){
                    // FIXME: The Z and Y axis are flipped here!!
                    frame.translate = new Vector3f(getFloatAttribute(translate,"x"),
                                                   getFloatAttribute(translate,"y"),
                                                   getFloatAttribute(translate,"z"));
                }else{
                    frame.translate = new Vector3f(0,0,0);
                }
                
                if (rotate != null){
                    frame.rotate = new Quaternion();
                    Node raxis = getChildNode(rotate, "axis");
                    // FIXME: The Z and Y axis are flipped here!!
                    Vector3f axis = new Vector3f(getFloatAttribute(raxis, "x"),
                                                 getFloatAttribute(raxis, "y"),
                                                 getFloatAttribute(raxis, "z"));
                    float angle = getFloatAttribute(rotate, "angle");
                    frame.rotate.fromAngleAxis(angle, axis);
                }else{
                    frame.rotate = new Quaternion();
                }
                
                // Scale doesn't do anything !?
                if (scale != null){
                    if (getAttribute(scale, "factor") != null){
                        float factor = Float.parseFloat(getAttribute(scale, "factor"));
                        frame.scale = new Vector3f(factor, factor, factor);
                    }else{
                        frame.scale = new Vector3f(getFloatAttribute(scale,"x"),
                                                   getFloatAttribute(scale,"y"),
                                                   getFloatAttribute(scale,"z"));
                    }
                }else{
                    frame.scale = new Vector3f(1,1,1);
                }
                
                frames.add(frame);
                
                keyframe = keyframe.getNextSibling();
            }

            track = track.getNextSibling();
        }
        
        // sort animation keyframe list
        Keyframe[] animFrames = new Keyframe[frames.size()];
        animFrames = frames.toArray(animFrames);
        Arrays.sort(animFrames);
        
        // create an array of times
        List<Float> frameTimes = new ArrayList<Float>();
        for (int i = 0; i < animFrames.length; i++){
            float time = animFrames[i].time;
            if (frameTimes.isEmpty() || frameTimes.get(frameTimes.size()-1).floatValue() < time){
                frameTimes.add(time);
            }
        }
        
        int[] lerpTimes = new int[frameTimes.size()];
        float[] times = new float[frameTimes.size()];
        for (int i = 0; i < frameTimes.size(); i++){
            times[i] = frameTimes.get(i);
            lerpTimes[i] = BoneAnimation.LINEAR;
        }
        
        anim.setInterpolationTypes(lerpTimes);
        anim.setTimes(times);
        
        for (Bone bone : bonesWithTracks){
            BoneTransform transform = new BoneTransform(bone, times.length);
            transform.setBone(bone);
            
            // k = the keyframe index
            for (int k = 0; k < times.length; k++) {
                Keyframe frame = null;
                
                // find a keyframe for this point of time
                for (int i = 0; i < animFrames.length; i++){
                    if (animFrames[i].bone == bone && animFrames[i].time == times[k]){
                        frame = animFrames[i];
                    }
                }
                
                if (frame == null){
                    throw new UnsupportedOperationException("Cannot find keyframe at time "+times[k]+" and bone "+bone.getName());
                }
                
                transform.setTranslation(k, frame.translate);
                transform.setRotation(k, frame.rotate);
            }
            
            anim.addBoneTransforms(transform);
        }

        anim.setStartFrame(0);
        anim.setEndFrame(times.length - 1);
        
        return anim;
    }
 
    private Bone loadSkeleton(Node skeleton){
        Node bones = getChildNode(skeleton, "bones");
        
        Map<String, Bone> boneMap = new HashMap<String, Bone>();
        Map<Integer, Bone> indexedBoneMap = new HashMap<Integer, Bone>();
        //indexedBoneMap.put(Integer.valueOf(0), rootBone);
        //boneMap.put("OgreSkeleton", rootBone);
        
        Vector3f vpos   = new Vector3f(0, 0, 0);
        Quaternion vrot = new Quaternion();
        Vector3f vscale = new Vector3f(1, 1, 1);
        Vector3f vaxis = new Vector3f(1, 0, 0);
        
        Node boneNode = bones.getFirstChild();
        while (boneNode != null){
            if (!boneNode.getNodeName().equals("bone")){
                boneNode = boneNode.getNextSibling();
                continue;
            }
            
            int id = Integer.parseInt(getAttribute(boneNode, "id"));
            String boneName = getAttribute(boneNode, "name");
            
            Bone bone = new Bone(boneName);
            
            Node pos = getChildNode(boneNode, "position");
            Node rot = getChildNode(boneNode, "rotation");
            Node scale = getChildNode(boneNode, "scale");
            Node axis = getChildNode(rot, "axis");
            
            //bone.lockTransforms();
            
            if (rot != null){
                vaxis.set(getFloatAttribute(axis, "x"),
                          getFloatAttribute(axis, "y"),
                          getFloatAttribute(axis, "z"));

                if (vaxis.length() != 1.0){
                    logger.warning("Rotation axis not normalized");
                    vaxis.normalizeLocal();
                }
                
                vrot.fromAngleNormalAxis(getFloatAttribute(rot, "angle"), vaxis);
                bone.getLocalRotation().set(vrot);
            }
            
            if (pos != null){
                bone.getLocalTranslation().set(getFloatAttribute(pos, "x"),
                                               getFloatAttribute(pos, "y"),
                                               getFloatAttribute(pos, "z"));
            }
            
            if (scale != null){
                bone.getLocalScale().set(getFloatAttribute(scale, "x"),
                                         getFloatAttribute(scale, "y"),
                                         getFloatAttribute(scale, "z"));
            }
            
            boneMap.put(boneName, bone);
            //indexedBoneMap.put(id+1, bone);
            indexedBoneMap.put(id, bone);

            bone.updateWorldVectors();
            
            boneNode = boneNode.getNextSibling();
        }
        
        // read bonehierarchy
        Node bonehierarchy = getChildNode(skeleton, "bonehierarchy");
        Node boneparent = bonehierarchy.getFirstChild();
        while (boneparent != null){
            if (!boneparent.getNodeName().equals("boneparent")){
                boneparent = boneparent.getNextSibling();
                continue;
            }
            
            String bonename = getAttribute(boneparent, "bone");
            String parentname = getAttribute(boneparent, "parent");
            
            // find the bone
            Bone bone = boneMap.get(bonename);
            
            // find the parent
            Bone parent = boneMap.get(parentname);
            
            // attach bone to parent
            parent.attachChild(bone);
            
            // next parent in skeleton file
            boneparent = boneparent.getNextSibling();
        }
        
        boneList = new Bone[indexedBoneMap.size()];
        
        // find bones without a parent and attach them to the skeleton
        // also assign the bones to the bonelist
        for (Map.Entry<Integer, Bone> entry: indexedBoneMap.entrySet()){
            Bone bone = entry.getValue();
            boneList[entry.getKey()] = bone;
            
            //if (bone == rootBone)
            //    continue;
            
            if (bone.getParent() == null){
                //rootBone.attachChild(bone);
                rootBone = bone;
                println("BONE"+entry.getKey()+"(name="+bone.getName()+")");
            }else{
                println("BONE"+entry.getKey()+"(name="+bone.getName()+", parent="+bone.getParent().getName()+")");
            }
        }
        
        // Note: animationlinks not supported
        
        // assign the animations
        AnimationController ac = new AnimationController();
        ac.setRepeatType(Controller.RT_WRAP);
        
//        Node animations = getChildNode(skeleton, "animations");
//        if (animations != null){
//            Node animation = animations.getFirstChild();
//            while (animation != null){
//                if (!animation.getNodeName().equals("animation")){
//                    animation = animation.getNextSibling();
//                    continue;
//                }
//
//                BoneAnimation anim = loadAnimation(animation, boneMap);
//                ac.addAnimation(anim);
//                if (ac.getActiveAnimation() == null)
//                    ac.setActiveAnimation(anim);
//
//                animation = animation.getNextSibling();
//            }
//        }
        
//        rootBone.addController(ac);

        return rootBone;
    }
    
    private TriMesh loadVertexBuffer(Node vertexbuffer, int vertexCount){
        TriMesh mesh = new TriMesh("nil");
        
        FloatBuffer vb = null;
        FloatBuffer nb = null;
        FloatBuffer cb = null;
        FloatBuffer tanb = null;
        FloatBuffer binb = null;
        TexCoords[] tb = null;
        
        // vertex positions
        String hasPositions = getAttribute(vertexbuffer, "positions");
        if (hasPositions != null && hasPositions.equalsIgnoreCase("true")){
            vb = BufferUtils.createFloatBuffer(vertexCount*3);
        }else{
            logger.warning("Mesh is missing vertices! will not render properly");
        }
        
        // vertex normals
        String hasNormals = getAttribute(vertexbuffer, "normals");
        if (hasNormals != null && hasNormals.equalsIgnoreCase("true")){
            nb = BufferUtils.createFloatBuffer(vertexCount*3);
        }
        
        // texture coordinates
        int texbuffersN = 0;
        String texbuffers = getAttribute(vertexbuffer, "texture_coords");
        if (texbuffers != null){
            texbuffersN = Integer.parseInt(texbuffers);
            tb = new TexCoords[texbuffersN];
            for (int i = 0; i < texbuffersN; i++){
                // read dimensions
                int dimensions = Integer.parseInt(getAttribute(vertexbuffer, "texture_coord_dimensions_"+i));
                tb[i] = new TexCoords(BufferUtils.createFloatBuffer(vertexCount*dimensions), dimensions);
            }
        }

        // vertex colors
        String hasColors = getAttribute(vertexbuffer, "colours_diffuse");
        if (hasColors != null && hasColors.equalsIgnoreCase("true")){
            cb = BufferUtils.createFloatBuffer(vertexCount*4);
        }
        
        // specular/secondary colors
        // not supported as of jME 1.0, ignore
        String hasSpecular = getAttribute(vertexbuffer, "colours_specular");
        if (hasSpecular != null && hasSpecular.equalsIgnoreCase("true")){
            logger.warning("Specular colors are not supported!");
        }
        
        // tangents
        String hasTangents = getAttribute(vertexbuffer, "tangents");
        int tangentDimensions = 3;
        if (hasTangents != null && hasTangents.equalsIgnoreCase("true")){
            String dimensionsStr = getAttribute(vertexbuffer, "tangent_dimensions");
            if (dimensionsStr != null)
                tangentDimensions = Integer.parseInt(dimensionsStr);
            
            tanb = BufferUtils.createFloatBuffer(vertexCount*tangentDimensions);
        }
        
        // binormals
        String hasBinormals = getAttribute(vertexbuffer, "binormals");
        if (hasBinormals != null && hasBinormals.equalsIgnoreCase("true")){
            binb = BufferUtils.createFloatBuffer(vertexCount*3);
        }
                
        // Read the vertexbuffer
        Node vertex = vertexbuffer.getFirstChild();
        while (vertex != null){
            if (!vertex.getNodeName().equals("vertex")){  
                vertex = vertex.getNextSibling();
                continue;
            }

            if (vb != null){
                Node position = getChildNode(vertex, "position");
                if (Z_up_to_Y_up){
                    // Note: this is mostly used for conversion
                    // between blender's coordinate system
                    // to jME's. Not very accurate yet.
                    vb.put(-getFloatAttribute(position, "x"))
                      .put(+getFloatAttribute(position, "z"))
                      .put(+getFloatAttribute(position, "y"));
                }else{
                    vb.put(getFloatAttribute(position, "x"))
                      .put(getFloatAttribute(position, "y"))
                      .put(getFloatAttribute(position, "z"));
                }
            }

            if (nb != null){
                Node normal = getChildNode(vertex, "normal");
                nb.put(getFloatAttribute(normal, "x"))
                  .put(getFloatAttribute(normal, "y"))
                  .put(getFloatAttribute(normal, "z"));
            }

            if (tanb != null){
                Node tangent = getChildNode(vertex, "tangent");
                tanb.put(getFloatAttribute(tangent, "x"))
                    .put(getFloatAttribute(tangent, "y"))
                    .put(getFloatAttribute(tangent, "z"));
                if (tangentDimensions == 4)
                    tanb.put(getFloatAttribute(tangent, "w"));
            }
            
            if (binb != null){
                Node binormal = getChildNode(vertex, "binormal");
                binb.put(getFloatAttribute(binormal, "x"))
                    .put(getFloatAttribute(binormal, "y"))
                    .put(getFloatAttribute(binormal, "z"));
            }
            
            if (tb != null){
                Node texcoord = vertex.getFirstChild();
                int texCoordIndex = 0;
                while (texcoord != null){
                    if (texcoord.getNodeName().equals("texcoord")){
                        TexCoords coords = tb[texCoordIndex];
                        FloatBuffer texbuf = coords.coords;
                        
                        texbuf.put(getFloatAttribute(texcoord, "u"));
                        if (coords.perVert > 1){
                            texbuf.put(getFloatAttribute(texcoord, "v"));
                            if (coords.perVert == 3)
                                texbuf.put(getFloatAttribute(texcoord, "w"));
                        }
                        
                        texCoordIndex++;
                    }

                    texcoord = texcoord.getNextSibling();
                }
            }

            if (cb != null){
                Node color = getChildNode(vertex, "colour_diffuse");
                String[] vals = getAttribute(color, "value").split(" ");
                if (vals.length == 4){
                    cb.put(Float.parseFloat(vals[0]))
                      .put(Float.parseFloat(vals[1]))
                      .put(Float.parseFloat(vals[2]))
                      .put(Float.parseFloat(vals[3]));
                }else if (vals.length == 3){
                    cb.put(Float.parseFloat(vals[0]))
                      .put(Float.parseFloat(vals[1]))
                      .put(Float.parseFloat(vals[2]))
                      .put(1.0f);
                }else{
                    logger.warning("Must have 3 or 4 floats for color!");
                }
            }
            
            vertex = vertex.getNextSibling();
        }
        
        mesh.setVertexBuffer(vb);
        mesh.setNormalBuffer(nb);
        mesh.setColorBuffer(cb);
        mesh.setTangentBuffer(tanb);
        mesh.setBinormalBuffer(binb);
        for (int i = 0; i < texbuffersN; i++)
            mesh.setTextureCoords(tb[i], i);
        
        return mesh;
    }
    
    /**
     * Loads a submesh from an XML node
     * 
     * @param submesh XML node
     * @return
     */
    private TriMesh loadSubmesh(Node submesh){
        TriMesh trimesh = new TriMesh("nil");
        
        // try to load a material if it is defined
        trimesh.setName(getAttribute(submesh, "material"));
        
        String material = getAttribute(submesh, "material");
        if (material != null)
            applyMaterial(material, trimesh);
        
        // using shared verticles?
        boolean sharedVerts = true;
        String usesharedvertices = getAttribute(submesh, "usesharedvertices");
        if (usesharedvertices != null){
            if (!usesharedvertices.equalsIgnoreCase("true"))
                sharedVerts = false;
        }
        
        // determine triangle mode
        TriMesh.Mode trimode = null;
        String operationtype = getAttribute(submesh, "operationtype");
        if (operationtype.equals("triangle_list"))
            trimode = Mode.Triangles;
        else if (operationtype.equals("triangle_strip"))
            trimode = Mode.Strip;
        else if (operationtype.equals("triangle_fan"))
            trimode = Mode.Fan;
        
        // check if faces definition exists (required for submesh)
        Node faces    = getChildNode(submesh, "faces");
        if (faces == null){
            logger.severe("Cannot load submesh: faces definition required");
            return null;
        }
        
        TriMesh sourcegeom;
        if (!sharedVerts){
            Node geometry = getChildNode(submesh, "geometry");
            Node vertexbuffer = getChildNode(geometry, "vertexbuffer");
            
            // inherit geometry data from unique vertex buffer defined here
            int vertexCount = Integer.parseInt(getAttribute(geometry, "vertexcount"));
            sourcegeom = loadVertexBuffer(vertexbuffer, vertexCount);
        }else{
            // inherit geometry data from shared geometry
            sourcegeom = sharedgeom;
        }
        
        trimesh.setVertexBuffer(sourcegeom.getVertexBuffer());
        trimesh.setNormalBuffer(sourcegeom.getNormalBuffer());
        trimesh.setColorBuffer(sourcegeom.getColorBuffer());
        trimesh.setTangentBuffer(sourcegeom.getTangentBuffer());
        trimesh.setBinormalBuffer(sourcegeom.getBinormalBuffer());
        trimesh.setTextureCoords(sourcegeom.getTextureCoords());
        
        // Read face/triangle data
        int count = faces.getChildNodes().getLength();
        if (getAttribute(faces, "count") != null){
            count = Integer.parseInt(getAttribute(faces, "count"));
        }
        Node face = faces.getFirstChild();
        IntBuffer ib = BufferUtils.createIntBuffer(count*3);
        while (face != null){
            if (face.getNodeType() == Node.TEXT_NODE
                            || face.getNodeType() == Node.COMMENT_NODE){
                face = face.getNextSibling();
                continue;
            }
            
            String nv1 = getAttribute(face, "v1");
            String nv2 = getAttribute(face, "v2");
            String nv3 = getAttribute(face, "v3");
            
            ib.put(Integer.parseInt(nv1));
            if (nv2 != null && nv3 != null)
                ib.put(Integer.parseInt(nv2)).put(Integer.parseInt(nv3));
                
            face = face.getNextSibling();
        }
        
        // Assign index buffer
        trimesh.setIndexBuffer(ib);

        // Read weights
        Node boneassignments = getChildNode(submesh, "boneassignments");
        
        // ignore weights if no skeleton defined
        if (skinnode != null){
            int geomIndex = skinnode.getSkins().getQuantity();
            
            // add ourselves to the SkinNode
            skinnode.addSkin(trimesh);
            
            if (boneassignments != null){
                // assign weights to skin
                Node assignment = boneassignments.getFirstChild();

                while (assignment != null){
                    if (assignment.getNodeName().equals("vertexboneassignment")){
                        int vertIndex = Integer.parseInt(getAttribute(assignment, "vertexindex"));
                        int boneIndex = Integer.parseInt(getAttribute(assignment, "boneindex"));
                        float weight =  getFloatAttribute(assignment, "weight");

                        // need to add 1 here since boneList[0] is the OgreSkeleton node
                        // (not in the actual hierarchy)
                        //Bone bone = boneList[boneIndex+1];
                        Bone bone = boneList[boneIndex];
                        skinnode.addBoneInfluence(geomIndex, vertIndex, bone, weight);
                    }

                    assignment = assignment.getNextSibling();
                }
            }
        }else{
            // attach ourselves to the rootnode
            rootnode.attachChild(trimesh);
        }
       
        return trimesh;
    }
    
    private Spatial loadMesh(Document doc){
        NodeList nodes = doc.getElementsByTagName("mesh");
        if (nodes.getLength() == 0) {
            logger.severe("Cannot load mesh: missing top element");
            return new com.jme.scene.Node("nothing");
        }

        Node skeletonlink = getChildNode(nodes.item(0), "skeletonlink");
        if (skeletonlink != null){
            String name = getAttribute(skeletonlink, "name") + ".xml";

            skinnode = new SkinNode("OgreSkinnedModel");
            rootnode = skinnode;
            URL skeletonURL = ResourceLocatorTool.locateResource(ResourceLocatorTool.TYPE_MODEL, name);
            rootBone = loadSkeleton(skeletonURL);
            
            skinnode.setSkeleton(rootBone);

            // also attach skeleton
            skinnode.attachChild(rootBone);
        }else{
            rootnode = new com.jme.scene.Node("OgreStaticModel");
        }
        
        Node submeshesNode = getChildNode(nodes.item(0), "submeshes");
        Node submesh = submeshesNode.getFirstChild();
        while (submesh != null){
            if (submesh.getNodeType() != Node.TEXT_NODE
                            && submesh.getNodeType() != Node.COMMENT_NODE) {

                loadSubmesh(submesh);
            }
            
            submesh = submesh.getNextSibling();
        }
        
        Node submeshnamesNode = getChildNode(nodes.item(0), "submeshnames");
        Node submeshname = submeshnamesNode.getFirstChild();
        while (submeshname != null){
            if (!submeshname.getNodeName().equals("submeshname")){
                submeshname = submeshname.getNextSibling();
                continue;
            }
            
            int index = Integer.parseInt(getAttribute(submeshname, "index"));
            String name = getAttribute(submeshname, "name");
            
            rootnode.getChild(index).setName(name);
            
            submeshname = submeshname.getNextSibling();
        }
        
//        if (skin != null){
//            skin.normalizeWeights();
//            skin.regenInfluenceOffsets();
//        }
        
        rootnode.updateGeometricState(0, true);
        rootnode.updateRenderState();
        
        return rootnode;
    }

    
}
