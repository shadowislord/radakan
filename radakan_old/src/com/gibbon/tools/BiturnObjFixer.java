package com.gibbon.tools;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

/**
 * Fixes Biturn's MDX to OBJ conversion bug,
 * which causes all objects in model to always use first material.
 * 
 * @author Momoko_Fan
 */
public final class BiturnObjFixer {

    public static void main(String[] args) throws IOException{
        //batchFixFolder(new File(args[0]));
        batchFixFolder(new File("E:\\B2M\\ConvertedOBJ\\"));
    }
    
    public static void batchFixFolder(File folder) throws IOException{
        for (File file : folder.listFiles()){
            String name = file.getName();
            int index = name.lastIndexOf('.');
            if (index > 0){
                if (name.substring(index+1).equalsIgnoreCase("obj")){
                    fixFile(file);
                }
            }
        }
    }
    
    private static String[] loadMaterialNames(File matfile) throws IOException{
        List<String> matNames = new ArrayList<String>();
        
        BufferedReader r = new BufferedReader(new FileReader(matfile));
        while (r.ready()){
            String ln = r.readLine();
            if (ln.startsWith("newmtl")){
                String[] brk = ln.split(" ");
                matNames.add(brk[1]);
                
            }
        }
        
        //System.out.println(matfile);
        //System.out.println("Material: "+matNames);
        return matNames.toArray(new String[0]);
    }
    
    public static void fixFile(File objfile) throws IOException{
        File matfile = null;
        
        File tmpfile = new File(objfile+".tmp");
        BufferedWriter w = new BufferedWriter(new FileWriter(tmpfile));
        
        // first find the material library for the obj file
        BufferedReader r = new BufferedReader(new FileReader(objfile));
        while (r.ready()){
            String ln = r.readLine();
            w.write(ln);
            w.newLine();
            if (ln.startsWith("mtllib")){
                String[] brk = ln.split(" ");
                matfile = new File(objfile.getParent(), brk[1]);
                break;
            }
        }
        
        String[] matNames = loadMaterialNames(matfile);
        
        // the bug only appears in models with more than 1 material only
        // so avoid wasting work and quit here
        if (matNames == null || matNames.length <= 1){
            r.close();
            w.close();
            tmpfile.delete();
            return;
        }
        
        
        int count = 0;
        while (r.ready()){
            String ln = r.readLine();

            if (ln.startsWith("usemtl")){
                // replace material name here with correct material name
                // based on the current index
                w.write("usemtl "+matNames[count]);
                count++;
            }else{
                w.write(ln);
            }
            w.newLine();
        }
        r.close();
        w.close();
        objfile.delete();
        tmpfile.renameTo(objfile);
        //System.exit(1);
    }
    
}
