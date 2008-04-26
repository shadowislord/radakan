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
import com.jme.scene.TriMesh;
import com.jme.scene.batch.GeomBatch;
import com.jme.scene.batch.TriangleBatch;
import com.jme.util.BoneDebugger;
import com.jme.util.geom.BufferUtils;

import com.jme.util.resource.ResourceLocatorTool;
import java.applet.Applet;
import java.applet.AppletContext;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;

import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import java.io.IOException;
import java.net.MalformedURLException;
import java.net.URL;
import java.nio.FloatBuffer;
import java.nio.IntBuffer;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
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
    
    private Map<String, Material> materialMap;
    private boolean Z_up_to_Y_up = true;
    private static final Matrix4f convertMatrix;
    
    static {
        convertMatrix = new Matrix4f();
        convertMatrix.fromAngleAxis(FastMath.HALF_PI, Vector3f.UNIT_X);
        Matrix4f rot2 = new Matrix4f();
        rot2.fromAngleAxis(FastMath.PI, Vector3f.UNIT_Z);
        convertMatrix.multLocal(rot2);
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
            System.out.println("ATTRIB: "+map.item(i).getNodeName());
        }
    }
    
    private float getFloatAttribute(Node node, String name){
        return Float.parseFloat(getAttribute(node,name));
    }
    
    private void applyMaterial(String name, TriangleBatch target){
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

            System.out.println("MESH: "+url.getFile());
            
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
        
    }
    
    private Bone[] loadSkeleton(URL url){
        try {
            DocumentBuilder builder = DocumentBuilderFactory.newInstance().newDocumentBuilder();
            Document doc = builder.parse(url.openStream());

            NodeList nodes = doc.getElementsByTagName("skeleton");
            
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
        
        Node tracks = getChildNode(animation, "tracks");
        Node track = tracks.getFirstChild();
        while (track != null){
            if (!track.getNodeName().equals("track")){
                track = track.getNextSibling();
                continue;
            }

            List<Keyframe> frames = new ArrayList<Keyframe>();
            Bone bone = boneMap.get(getAttribute(track, "bone"));

            Node keyframe = getChildNode(track, "keyframes").getFirstChild();
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
                    frame.translate = new Vector3f(getFloatAttribute(translate,"x"),
                                                   getFloatAttribute(translate,"y"),
                                                   getFloatAttribute(translate,"z"));
                }else{
                    frame.translate = new Vector3f(0,0,0);
                }
                
                if (rotate != null){
                    frame.rotate = new Quaternion();
                    Node raxis = getChildNode(rotate, "axis");
                    Vector3f axis = new Vector3f(getFloatAttribute(raxis, "x"),
                                                 getFloatAttribute(raxis, "y"),
                                                 getFloatAttribute(raxis, "z"));
                    float angle = getFloatAttribute(rotate, "angle");
                    frame.rotate.fromAngleAxis(angle, axis);
                }else{
                    frame.rotate = new Quaternion();
                }
                
                if (scale != null){
                    frame.scale = new Vector3f(getFloatAttribute(scale,"x"),
                                               getFloatAttribute(scale,"y"),
                                               getFloatAttribute(scale,"z"));
                }else{
                    frame.scale = new Vector3f(1,1,1);
                }
                

                frames.add(frame);
                
                keyframe = keyframe.getNextSibling();
            }

            BoneTransform transform = new BoneTransform(bone, frames.size());
            
            float[] times = new float[frames.size()];
            for (int k = 0; k < frames.size(); k++){
                Keyframe frame = frames.get(k);
                times[k] = frame.time;
                transform.setTranslation(k, frame.translate);
                transform.setRotation(k, frame.rotate);
            }
            
            anim.addBoneTransforms(transform);
            anim.setTimes(times);
            anim.setEndFrame(frames.size()-1);
            
            // needs to have the same times as the bone transform

            track = track.getNextSibling();
        }
        
        return anim;
    }
 
    private Bone[] loadSkeleton(Node skeleton){
        Node bones = getChildNode(skeleton, "bones");
        
        Bone rootBone = new Bone("OgreSkeleton");
        Map<String, Bone> boneMap = new HashMap<String, Bone>();
        Map<Integer, Bone> indexedBoneMap = new HashMap<Integer, Bone>();
        indexedBoneMap.put(Integer.valueOf(0), rootBone);
        boneMap.put("OgreSkeleton", rootBone);
        
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

                vaxis.normalizeLocal();
                
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
            indexedBoneMap.put(id+1, bone);

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
            
            boneparent = boneparent.getNextSibling();
        }
        
        Bone[] boneList = new Bone[indexedBoneMap.size()];
        
        // find bones without a parent and attach them to the skeleton
        // also assign the bones to the bonelist
        for (Map.Entry<Integer, Bone> entry: indexedBoneMap.entrySet()){
            Bone bone = entry.getValue();
            boneList[entry.getKey()] = bone;
            
            if (bone == rootBone)
                continue;
                
            if (bone.getParent() == null){
                rootBone.attachChild(bone);
            }
        }
        
        // assign the animations
        AnimationController ac = new AnimationController();
        ac.setRepeatType(Controller.RT_WRAP);
        
        Node animations = getChildNode(skeleton, "animations");
        if (animations != null){
            Node animation = animations.getFirstChild();
            while (animation != null){
                if (!animation.getNodeName().equals("animation")){
                    animation = animation.getNextSibling();
                    continue;
                }

                BoneAnimation anim = loadAnimation(animation, boneMap);
                ac.addAnimation(anim);
                if (ac.getActiveAnimation() == null)
                    ac.setActiveAnimation(anim);

                animation = animation.getNextSibling();
            }
        }
        
        //boneList[0].addController(ac);

        return boneList;
    }
    
    private TriangleBatch loadSubmesh(Node submesh, SkinNode skin, Bone[] boneList){
        TriangleBatch batch = new TriangleBatch();
        batch.setName(getAttribute(submesh, "material"));
        
        String material = getAttribute(submesh, "material");
        if (material != null)
            applyMaterial(material, batch);
        
        Node geometry = getChildNode(submesh, "geometry");
        Node faces    = getChildNode(submesh, "faces");
        
        if (geometry == null || faces == null){
            logger.severe("Cannot load submesh: missing geometry data");
            return null;
        }
        
        int vcount = Integer.parseInt(getAttribute(geometry, "vertexcount"));
        Node vertexbuffer = getChildNode(geometry, "vertexbuffer");
        FloatBuffer vb = null;
        FloatBuffer nb = null;
        FloatBuffer cb = null;
        FloatBuffer[] tb = null;
        if (getAttribute(vertexbuffer, "positions").equalsIgnoreCase("true")){
            vb = BufferUtils.createFloatBuffer(vcount*3);
        }else{
            logger.warning("Mesh is missing vertices! will not render properly");
        }
        if (getAttribute(vertexbuffer, "normals").equalsIgnoreCase("true")){
            nb = BufferUtils.createFloatBuffer(vcount*3);
        }
        int texbuffersN = 0;
        String texbuffers = getAttribute(vertexbuffer, "texture_coords");
        if (texbuffers != null){
            texbuffersN = Integer.parseInt(texbuffers);
            tb = new FloatBuffer[texbuffersN];
            for (int i = 0; i < texbuffersN; i++)
                tb[i] = BufferUtils.createFloatBuffer(vcount*2);
        }
        String hasColors = getAttribute(vertexbuffer, "colours_diffuse");
        if (hasColors != null && hasColors.equalsIgnoreCase("true")){
            cb = BufferUtils.createFloatBuffer(vcount*4);
        }
        
        // Read vertex data
        Node vertex = vertexbuffer.getFirstChild();
        while (vertex != null){
            if (vertex.getNodeName().equals("vertex")){       
                if (vb != null){
                    Node position = getChildNode(vertex, "position");
                    if (Z_up_to_Y_up){
                        vb.put(-getFloatAttribute(position, "x"))
                          .put(getFloatAttribute(position, "z"))
                          .put(getFloatAttribute(position, "y"));
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

                if (tb != null){
                    for (int i = 0; i < texbuffersN; i++){
                        Node texcoord = getChildNode(vertex, "texcoord");
                        tb[i].put(getFloatAttribute(texcoord, "u"))
                             .put(getFloatAttribute(texcoord, "v"));
                    }
                }

                if (cb != null){
                    Node color = getChildNode(vertex, "colour_diffuse");
                    String[] vals = getAttribute(color, "value").split(" ");
                    cb.put(Float.parseFloat(vals[0]))
                      .put(Float.parseFloat(vals[1]))
                      .put(Float.parseFloat(vals[2]))
                      .put(Float.parseFloat(vals[3]));
                }
            }
            
            vertex = vertex.getNextSibling();
        }
        
        // Read faces
        int count = Integer.parseInt(getAttribute(faces, "count"));
        Node face = faces.getFirstChild();
        IntBuffer ib = BufferUtils.createIntBuffer(count*3);
        while (face != null){
            if (face.getNodeType() == Node.TEXT_NODE
                            || face.getNodeType() == Node.COMMENT_NODE){
                face = face.getNextSibling();
                continue;
            }
            
            Node nv1 = face.getAttributes().getNamedItem("v1");
            Node nv2 = face.getAttributes().getNamedItem("v2");
            Node nv3 = face.getAttributes().getNamedItem("v3");
            
            int v1 = Integer.parseInt(nv1.getNodeValue());
            int v2 = Integer.parseInt(nv2.getNodeValue());
            int v3 = Integer.parseInt(nv3.getNodeValue());
            
//            if (Z_up_to_Y_up){
//                ib.put(v3).put(v2).put(v1);
//            }else{
                ib.put(v1).put(v2).put(v3);
//            }
            
            face = face.getNextSibling();
        }
        
        // Assign mesh data
        batch.setVertexBuffer(vb);
        batch.setNormalBuffer(nb);
        batch.setColorBuffer(cb);
        batch.setIndexBuffer(ib);
        for (int i = 0; i < texbuffersN; i++)
            batch.setTextureBuffer(tb[i], i);
        
        // Read weights
        Node boneassignments = getChildNode(submesh, "boneassignments");
        
        // ignore weights if no skeleton defined
        if (skin != null){
            int batchNum = skin.getSkin().getBatchCount();
            
            // add ourselves to the SkinNode
            skin.getSkin().addBatch(batch);
            
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
                        Bone bone = boneList[boneIndex+1];
                        skin.addBoneInfluence(batchNum, vertIndex, bone, weight);
                    }

                    assignment = assignment.getNextSibling();
                }
            }
        }
       
        return batch;
    }
    
    private Spatial loadMesh(Document doc){
        NodeList nodes = doc.getElementsByTagName("mesh");
        if (nodes.getLength() == 0) {
            logger.severe("Cannot load mesh: missing top element");
            return new com.jme.scene.Node("nothing");
        }
        com.jme.scene.Node root = new com.jme.scene.Node("OgreModel");
        
        Node skeletonlink = getChildNode(nodes.item(0), "skeletonlink");
        Bone[] boneList = null;
        SkinNode skin = null;
        if (skeletonlink != null){
            String name = getAttribute(skeletonlink, "name") + ".xml";

            URL skeletonURL = ResourceLocatorTool.locateResource(ResourceLocatorTool.TYPE_MODEL, name);
            boneList = loadSkeleton(skeletonURL);
            
            TriMesh geom = new TriMesh("OgreMesh");
            geom.clearBatches();
            
            skin = new SkinNode("OgreSkin");
            skin.setSkeleton(boneList[0]);
            skin.setSkin(geom);
            
            // skin is the root now
            root.attachChild(skin);
            
            // also attach skeleton
            root.attachChild(boneList[0]);
        }
        
        Node submeshesNode = getChildNode(nodes.item(0), "submeshes");
        Node submesh = submeshesNode.getFirstChild();
        while (submesh != null){
            if (submesh.getNodeType() != Node.TEXT_NODE
                            && submesh.getNodeType() != Node.COMMENT_NODE) {

                TriangleBatch batch = loadSubmesh(submesh, skin, boneList);
                if (batch != null && skin == null) {
                    TriMesh mesh = new TriMesh(batch.getName());
                    mesh.removeBatch(0);
                    mesh.addBatch(batch);

//                    Quaternion q = new Quaternion();
//                    q.fromAngles(-FastMath.HALF_PI, 0, 0);
//                    mesh.setLocalRotation(q);

                    root.attachChild(mesh);
                }
            }
            
            submesh = submesh.getNextSibling();
        }
        
        if (skin != null){
            skin.normalizeWeights();
            skin.regenInfluenceOffsets();
        }
            
        root.updateGeometricState(0, true);
        root.updateRenderState();
        
        return root;
    }

    
}
