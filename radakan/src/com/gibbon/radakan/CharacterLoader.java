/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package com.gibbon.radakan;

import com.jme.bounding.BoundingBox;
import com.jme.scene.Controller;
import com.jme.scene.Spatial;
import com.jme.util.export.binary.BinaryExporter;
import com.jme.util.export.binary.BinaryImporter;
import com.model.md5.JointAnimation;
import com.model.md5.ModelNode;
import com.model.md5.controller.JointController;
import com.model.md5.importer.MD5Importer;
import com.model.md5.importer.resource.AnimImporter;
import com.model.md5.importer.resource.MeshImporter;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.StreamTokenizer;
import java.net.URL;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author Momoko_Fan
 */
public class CharacterLoader {

    private static StreamTokenizer createReader(InputStream stream) {
        InputStreamReader streamReader = new InputStreamReader(stream);
        StreamTokenizer reader = new StreamTokenizer(streamReader);
        reader.quoteChar('"');
        reader.ordinaryChar('{');
        reader.ordinaryChar('}');
        reader.ordinaryChar('(');
        reader.ordinaryChar(')');
        reader.parseNumbers();
        reader.slashSlashComments(true);
        reader.eolIsSignificant(true);
        return reader;
    }
    
    public static ModelNode loadCharacter(URL model, URL anim_stand, URL anim_walk, File optFile, String name) {
        try {
            ModelNode node;
            
            if (!optFile.exists()){
                StreamTokenizer st = createReader(model.openStream());
                MeshImporter mi = new MeshImporter(st);
                node = mi.loadMesh(name);
                node.setModelBound(new BoundingBox());
                node.updateModelBound();
                node.updateWorldBound();
                assert node.getWorldBound() != null;

                st = createReader(anim_stand.openStream());
                AnimImporter ai = new AnimImporter(st);
                JointAnimation stand = ai.loadAnim("stand");
                stand.setFrameRate(10);

                st = createReader(anim_walk.openStream());
                ai = new AnimImporter(st);
                JointAnimation walk = ai.loadAnim("walk");
                walk.setFrameRate(40);

                JointController controller = new JointController(node.getJoints());
                controller.setRepeatType(Controller.RT_WRAP);
                controller.addAnimation(walk);
                controller.addAnimation(stand);
                controller.setActive(true);
                node.addController(controller);
                
                optFile.createNewFile();
                BinaryExporter.getInstance().save(node, optFile);
            }else{
                node = (ModelNode) BinaryImporter.getInstance().load(optFile);
                
                if (node.getWorldBound() == null){
                    node.updateModelBound();
                    node.updateWorldBound();
                    assert node.getWorldBound() != null;
                }
            }
            
            return node;
        } catch (IOException ex) {
            Logger.getLogger(CharacterLoader.class.getName()).log(Level.SEVERE, null, ex);
        }
        
        return null;
    }
    
}
