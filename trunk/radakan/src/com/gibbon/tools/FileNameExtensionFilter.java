package com.gibbon.tools;

import java.io.File;
import javax.swing.filechooser.FileFilter;


public class FileNameExtensionFilter extends FileFilter {

    private String description;
    private String[] formats;
    
    public FileNameExtensionFilter(String description, String ... formats){
        this.description = description;
        this.formats = formats;
    }
    
    @Override
    public boolean accept(File f) {
        for (String format : formats)
            if (f.getName().endsWith(format))
                return true;
        
        return false;
    }

    @Override
    public String getDescription() {
        return description;
    }

}
