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
import com.jme.scene.Spatial.CullHint;
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
    
    /**
     * sharedgeom contains all the sharedgeometry vertexbuffers defined in the mesh
     * file combined together. The vertexes are referenced by the submeshes through the index buffers.
     * Since jME does not really support shared vertex buffers yet, those are reference
     * copied into the submeshes' trimeshes.
     */
    private TriMesh sharedgeom = new TriMesh("sharedgeom");
    
    /**
     * The root bone of the skeleton, or null if a skeleton is not defined for this mesh file.
     * The assumption is made that all skeletons contain a single root bone.
     */
    private Bone rootBone;
    
    /**
     * An array containing all the bones in the skeleton. 
     * The bone index in the skeleton file is the same as the index in this array.
     */
    private Bone[] boneList;
    
    /**
     * The SkinNode for the skeleton, or null if no skeleton is defined.
     * The skins in this node are the submeshes with no geometry sharing.
     * Submeshes that share geometry must be attached directly to the skinnode
     * as they do not have any bone assignments (bone assignments are defined
     * only for the shared geometry instead of it's submeshes).
     */
    private SkinNode skinnode;
    
    /**
     * The node representing this mesh file. This is eqivelant to the skinnode
     * variable if a skeleton is defined.
     */
    private com.jme.scene.Node rootnode;
    
    /**
     * A mapping of the material names to the material object.
     * The mapping of materials must be seperately loaded by the MaterialLoader,
     * or generated in code.
     */
    private Map<String, Material> materialMap;
    
    /**
     * If this variable is true, the loader will attempt 
     * to convert from Z-up (e.g Blender3D) coordinate system 
     * to a Y-up (e.g jME, OpenGL) coordinate system.
     * This parameter is now obsolete as the Blender3D mesh exporter
     * is able to do this operation.
     */
    private boolean Z_up_to_Y_up = true;
    
    /**
     * Show debugging messages of OgreLoaders or not.
     */
    private static final boolean DEBUG = true;
    
    /**
     * Print a debugging message to standard output.
     */
    public void println(String str){
        if (DEBUG)
            System.out.println(str);
    }
    
    /**
     * Returns the first XML child tag with the specified name.
     * 
     * @param node The node to search children of
     * @param name The name of the node to search for, case-sensitive.
     * @return The child with the specified name, or null if none exists.
     */
    private Node getChildNode(Node node, String name) {
        Node child = node.getFirstChild();
        while (child != null && !child.getNodeName().equals(name) ){
            child = child.getNextSibling();
        }
        return child;
    }
    
    /**
     * Returns an attribute of the specified tag with the name provided.
     * 
     * @param node
     * @param name
     * @return
     */
    private String getAttribute(Node node, String name){
        Node att = node.getAttributes().getNamedItem(name);
        return att == null ? null : att.getNodeValue();
    }
    
    private float getFloatAttribute(Node node, String name){
        return Float.parseFloat(getAttribute(node,name));
    }
    
    private int getIntAttribute(Node node, String name){
        return Integer.parseInt(getAttribute(node,name));
    }
    
    /**
     * Applies a named material to the specified spatial.
     * 
     * @param name
     * @param target
     */
    private void applyMaterial(String name, Spatial target){
        // if the name contains an extension, make sure to remove it from the name
        if (name.contains(".")){
            int index = name.lastIndexOf(".");
            name = name.substring(0, index);
        }
        
        // no materials mapping defined, do not apply any materials
        if (materialMap == null)
            return;
        
        // find the named material and apply it to the target
        Material mat = materialMap.get(name);
        if (mat != null){
            mat.apply(target);
        }else{
            throw new IllegalStateException("Cannot find material: "+name);
        }
    }
    
    /**
     * Specify the mapping of materials to use when reading submeshes.
     * @param materials
     */
    public void setMaterials(Map<String, Material> materials){
        materialMap = materials;
    }
    
    /**
     * Load a MESH.XML model from the specified URL.
     * 
     * @param url The URL that specifies the mesh.xml file
     * @param Z_up Whether to flip the model from Z-up to Y-up
     * @return
     */
    public Spatial loadModel(URL url, boolean Z_up){
        try {
            Z_up_to_Y_up = Z_up;
            
            // read the xml document
            DocumentBuilder builder = DocumentBuilderFactory.newInstance().newDocumentBuilder();
            Document doc = builder.parse(url.openStream());

            println("MESH("+url.getFile()+")");
            
            // return the loaded mesh
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
    
    /**
     * A Keyframe class which is only used for sorting all the tracks' keyframes.
     * Used for bone animation only.
     */
    private static class Keyframe implements Comparable<Keyframe> {
        Bone bone;
        float time;
        Vector3f translate;
        Quaternion rotate;
        Vector3f scale;

        /**
         * A keyframe is greater than another keyframe if it's time is greater.
         * 
         * @param other
         * @return
         */
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
    
    /**
     * Load a SKELETON.XML model from the specified URL.
     * 
     * @param url The URL that specifies the skeleton.xml file
     * @return
     */
    private Bone loadSkeleton(URL url){
        try {
            // create a document
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
            
            int id = getIntAttribute(boneNode, "id");
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
    
    private void loadVertexBuffer(TriMesh target, Node vertexbuffer){
        FloatBuffer vb = target.getVertexBuffer();
        FloatBuffer nb = target.getNormalBuffer();
        FloatBuffer cb = target.getColorBuffer();
        FloatBuffer tanb = target.getTangentBuffer();
        FloatBuffer binb = target.getBinormalBuffer();
        ArrayList<TexCoords> tb = target.getTextureCoords();
        int startCoordIndex = tb.size();
        
        // vertex positions
        String hasPositions = getAttribute(vertexbuffer, "positions");
        if (hasPositions != null && hasPositions.equalsIgnoreCase("true")){
            if (vb == null)
                vb = BufferUtils.createFloatBuffer(target.getVertexCount() * 3);
            //vb = BufferUtils.createFloatBuffer(vertexCount*3);
        }
        
        // vertex normals
        String hasNormals = getAttribute(vertexbuffer, "normals");
        if (hasNormals != null && hasNormals.equalsIgnoreCase("true")){
            if (nb == null)
                nb = BufferUtils.createFloatBuffer(target.getVertexCount() * 3);
            //nb = BufferUtils.createFloatBuffer(vertexCount*3);
        }
        
        // texture coordinates
        int texbuffersN = 0;
        String texbuffers = getAttribute(vertexbuffer, "texture_coords");
        if (texbuffers != null){
            texbuffersN = Integer.parseInt(texbuffers);
            if (tb == null)
                tb = new ArrayList<TexCoords>();
            
            for (int i = 0; i < texbuffersN; i++){
                // read dimensions
                String dim = getAttribute(vertexbuffer, "texture_coord_dimensions_"+i);
                int dimensions = 2; //default
                if (dim != null){
                    dimensions = Integer.parseInt(dim);
                }
                
                TexCoords coords = new TexCoords(BufferUtils.createFloatBuffer(target.getVertexCount()*dimensions), dimensions);
                tb.add(coords);
            }
        }

        // vertex colors
        String hasColors = getAttribute(vertexbuffer, "colours_diffuse");
        if (hasColors != null && hasColors.equalsIgnoreCase("true")){
            if (cb == null)
                cb = BufferUtils.createFloatBuffer(target.getVertexCount()*4);
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
            
            if (tanb == null)
                tanb = BufferUtils.createFloatBuffer(target.getVertexCount()*tangentDimensions);
        }
        
        // binormals
        String hasBinormals = getAttribute(vertexbuffer, "binormals");
        if (hasBinormals != null && hasBinormals.equalsIgnoreCase("true")){
            if (binb == null)
                binb = BufferUtils.createFloatBuffer(target.getVertexCount()*3);
        }
        
        // set the buffers if any were created
        target.setVertexBuffer(vb);
        target.setNormalBuffer(nb);
        target.setColorBuffer(cb);
        target.setTangentBuffer(tanb);
        target.setBinormalBuffer(binb);
        target.setTextureCoords(tb);
        
        // Read the vertexbuffer
        Node vertex = vertexbuffer.getFirstChild();
        while (vertex != null){
            if (!vertex.getNodeName().equals("vertex")){  
                vertex = vertex.getNextSibling();
                continue;
            }

            Node position = getChildNode(vertex, "position");
            if (position != null){
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

            Node normal = getChildNode(vertex, "normal");
            if (normal != null){
                nb.put(getFloatAttribute(normal, "x"))
                  .put(getFloatAttribute(normal, "y"))
                  .put(getFloatAttribute(normal, "z"));
            }

            Node tangent = getChildNode(vertex, "tangent");
            if (tangent != null){
                tanb.put(getFloatAttribute(tangent, "x"))
                    .put(getFloatAttribute(tangent, "y"))
                    .put(getFloatAttribute(tangent, "z"));
                if (tangentDimensions == 4)
                    tanb.put(getFloatAttribute(tangent, "w"));
            }
            
            Node binormal = getChildNode(vertex, "binormal");
            if (binormal != null){ 
                binb.put(getFloatAttribute(binormal, "x"))
                    .put(getFloatAttribute(binormal, "y"))
                    .put(getFloatAttribute(binormal, "z"));
            }
            
            if (tb != null){
                Node texcoord = vertex.getFirstChild();
                int texCoordIndex = 0;
                while (texcoord != null){
                    if (texcoord.getNodeName().equals("texcoord")){
                        TexCoords coords = tb.get(startCoordIndex + texCoordIndex);
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

            Node color = getChildNode(vertex, "colour_diffuse");
            if (color != null){
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
        if (operationtype == null || operationtype.equals("triangle_list"))
            trimode = Mode.Triangles;
        else if (operationtype.equals("triangle_strip"))
            trimode = Mode.Strip;
        else if (operationtype.equals("triangle_fan"))
            trimode = Mode.Fan;
        trimesh.setMode(trimode);
        
        // check if faces definition exists (required for submesh)
        Node faces = getChildNode(submesh, "faces");
        if (faces == null){
            logger.severe("Cannot load submesh: faces definition required");
            return null;
        }
        
        if (!sharedVerts){
            Node geometry = getChildNode(submesh, "geometry");
            int vertexCount = getIntAttribute(geometry, "vertexcount");
            
            trimesh.setVertexCount(vertexCount);
            
            Node vertexbuffer = geometry.getFirstChild();
            while (vertexbuffer != null){
                if (vertexbuffer.getNodeName().equals("vertexbuffer")){
                    // inherit geometry data from unique vertex buffer defined here
                    loadVertexBuffer(trimesh, vertexbuffer);
                }
                vertexbuffer = vertexbuffer.getNextSibling();
            }
        }else{
            // inherit geometry data from shared geometry
            trimesh.setVertexBuffer(sharedgeom.getVertexBuffer());
            trimesh.setNormalBuffer(sharedgeom.getNormalBuffer());
            trimesh.setColorBuffer(sharedgeom.getColorBuffer());
            trimesh.setTangentBuffer(sharedgeom.getTangentBuffer());
            trimesh.setBinormalBuffer(sharedgeom.getBinormalBuffer());
            trimesh.setTextureCoords(sharedgeom.getTextureCoords());
        }
        
        // Read face/triangle data
        int count = faces.getChildNodes().getLength();
        if (getAttribute(faces, "count") != null){
            count = getIntAttribute(faces, "count");
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
        // also ignore if geometry data is shared, 
        // as weights will be taken care of under loadMesh method
        if (skinnode != null && !sharedVerts){
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
            com.jme.scene.Node skins = new com.jme.scene.Node("OgreSkinsList");
            skinnode.setSkins(skins);
            URL skeletonURL = ResourceLocatorTool.locateResource(ResourceLocatorTool.TYPE_MODEL, name);
            rootBone = loadSkeleton(skeletonURL);
            
            skinnode.setSkeleton(rootBone);

            // also attach skeleton
            skinnode.attachChild(rootBone);
        }else{
            rootnode = new com.jme.scene.Node("OgreStaticModel");
        }
        
        Node sharedgeometryNode = getChildNode(nodes.item(0), "sharedgeometry");
        if (sharedgeometryNode != null){
            int vertexCount = getIntAttribute(sharedgeometryNode, "vertexcount");
            sharedgeom.setVertexCount(vertexCount);
            sharedgeom.setCullHint(CullHint.Always);
            
            Node vertexbuffer = sharedgeometryNode.getFirstChild();
            while (vertexbuffer != null){
                if (vertexbuffer.getNodeName().equals("vertexbuffer")){
                    loadVertexBuffer(sharedgeom, vertexbuffer);
                }
                
                vertexbuffer = vertexbuffer.getNextSibling();
            }
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
        
        Node boneassignments = getChildNode(nodes.item(0), "boneassignments");
        if (boneassignments != null){
            // bone assignments defined for shared geometry,
            // make sure it's added to the skin list
            skinnode.getSkins().attachChild(sharedgeom);
            
            Node vertexboneassignment = boneassignments.getFirstChild();
            while (vertexboneassignment != null){
                if (vertexboneassignment.getNodeName().equals("vertexboneassignment")){
                    int vertIndex = getIntAttribute(vertexboneassignment, "vertexindex");
                    int boneIndex = getIntAttribute(vertexboneassignment, "boneindex");
                    float weight  = getFloatAttribute(vertexboneassignment, "weight");

                    Bone bone = boneList[boneIndex];
                    skinnode.addBoneInfluence(0, vertIndex, bone, weight);
                }

                vertexboneassignment = vertexboneassignment.getNextSibling();
            }
        }
        
        Node submeshnamesNode = getChildNode(nodes.item(0), "submeshnames");
        if (submeshnamesNode != null){
            Node submeshname = submeshnamesNode.getFirstChild();
            while (submeshname != null){
                if (!submeshname.getNodeName().equals("submeshname")){
                    submeshname = submeshname.getNextSibling();
                    continue;
                }

                int index = getIntAttribute(submeshname, "index");
                String name = getAttribute(submeshname, "name");

                rootnode.getChild(index).setName(name);

                submeshname = submeshname.getNextSibling();
            }
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
