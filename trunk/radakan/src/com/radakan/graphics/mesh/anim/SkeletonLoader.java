/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package com.radakan.graphics.mesh.anim;

import com.jme.math.Quaternion;
import com.jme.math.Vector3f;
import java.net.URL;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import java.util.logging.Logger;
import org.w3c.dom.Node;
import static com.radakan.util.XMLUtil.*;

/**
 *
 * @author Kirill
 */
public class SkeletonLoader {
    
    private static final Logger logger = Logger.getLogger(SkeletonLoader.class.getName());
    
    /**
     * Loads a skeleton object given XML node.
     * All BONEs bind and inverseBind matrices contain valid values.
     * 
     * @param skeletonNode
     * @return
     */
    public static final Skeleton loadSkeleton(Node skeletonNode){
        // maps the name of a bone to itself
        Map<String, Bone> boneMap = new HashMap<String, Bone>();
        
        // maps the index of a bone to itself
        Map<Integer, Bone> indexedBoneMap = new HashMap<Integer, Bone>();
        
        // some work variables
        Vector3f vpos   = new Vector3f(0, 0, 0);
        Quaternion vrot = new Quaternion();
        Vector3f vscale = new Vector3f(1, 1, 1);
        Vector3f vaxis  = new Vector3f(1, 0, 0);
        
        // skeleton -> bones -> bone
        Node bonesNode = getChildNode(skeletonNode, "bones");
        Node boneNode = bonesNode.getFirstChild();
        while (boneNode != null){
            if (!boneNode.getNodeName().equals("bone")){
                boneNode = boneNode.getNextSibling();
                continue;
            }
            
            int id = getIntAttribute(boneNode, "id");
            Bone bone = new Bone(getAttribute(boneNode, "name"));
            
            boneMap.put(bone.name, bone);
            //indexedBoneMap.put(id+1, bone);
            indexedBoneMap.put(id, bone);
            
            Node rot = getChildNode(boneNode, "rotation");
            if (rot != null){
                Node axis = getChildNode(rot, "axis");
                vaxis.set(getFloatAttribute(axis, "x"),
                          getFloatAttribute(axis, "y"),
                          getFloatAttribute(axis, "z"));

                if (vaxis.length() != 1.0){
                    logger.warning("Rotation axis not normalized");
                    vaxis.normalizeLocal();
                }
                
                vrot.fromAngleNormalAxis(getFloatAttribute(rot, "angle"), vaxis);
            }else{
                vrot.loadIdentity();
            }
            
            Node pos = getChildNode(boneNode, "position");
            if (pos != null){
                vpos.set(getFloatAttribute(pos, "x"),
                         getFloatAttribute(pos, "y"),
                         getFloatAttribute(pos, "z"));
            }else{
                vpos.zero();
            }
                
            Node scale = getChildNode(boneNode, "scale");
            if (scale != null){
                vscale.set(getFloatAttribute(scale, "x"),
                           getFloatAttribute(scale, "y"),
                           getFloatAttribute(scale, "z"));
            }else{
                vscale.set(Vector3f.UNIT_XYZ);
            }
            
            // compile individual transformations into bind matrix
            bone.bindMat.setTranslation(vpos);
            bone.bindMat.setRotationQuaternion(vrot);
            bone.bindMat.setScale(vscale);
            
            boneNode = boneNode.getNextSibling();
        }
        
        // skeleton -> bonehierarchy -> boneparent
        Node bonehierarchy = getChildNode(skeletonNode, "bonehierarchy");
        Node boneparent = bonehierarchy.getFirstChild();
        while (boneparent != null){
            if (!boneparent.getNodeName().equals("boneparent")){
                boneparent = boneparent.getNextSibling();
                continue;
            }

            Bone bone = boneMap.get(getAttribute(boneparent, "bone"));
            Bone parent = boneMap.get(getAttribute(boneparent, "parent"));
            
            parent.addChild(bone);
            
            boneparent = boneparent.getNextSibling();
        }
        
        ArrayList<Bone> bones = new ArrayList<Bone>(indexedBoneMap.size());
        
        // find bones without a parent and attach them to the skeleton
        // also assign the bones to the bonelist
        for (Map.Entry<Integer, Bone> entry: indexedBoneMap.entrySet()){
            Bone bone = entry.getValue();
            bones.add(entry.getKey(), bone);
        }

        // FIXME: animationlinks not supported currently
        
        // assign the animations
//        AnimationController ac = new AnimationController();
//        ac.setRepeatType(Controller.RT_WRAP);
//        
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
//        
//        rootBone.addController(ac);

        return new Skeleton(bones);
    }
    
}
