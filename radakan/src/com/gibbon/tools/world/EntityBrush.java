package com.gibbon.tools.world;

import com.gibbon.radakan.entity.EntityFactory;
import com.gibbon.radakan.entity.EntityFactory.EntityType;
import com.jme.math.Vector3f;
import com.jme.scene.TriMesh;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Collection;

public class EntityBrush {

    private static EntityFactory factory;
    
    public static Collection<EntityType> loadEntityTypes() throws IOException{
        factory = new EntityFactory();
        
        File entitiesFile = new File(System.getProperty("user.dir")+"/entities/");
        for (File f : entitiesFile.listFiles()){
            if (f.getName().endsWith(".xml")){
                InputStream in = new FileInputStream(f);
                factory.load(in);
                in.close();
            }
        }
        
        return factory.getLoadedTypes();
    }
    
    public static void onMouseAction(TriMesh collided, Vector3f point){
        if (!collided.getName().startsWith("TERRAIN"))
            return;
        
        
    }
    
}
