package com.radakan.util.protocols.pak;

import com.radakan.res.PakFileSystem;
import java.io.IOException;
import java.io.InputStream;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLConnection;
import java.net.URLStreamHandler;
import java.util.HashMap;
import java.util.Map;
import java.util.WeakHashMap;

public class Handler extends URLStreamHandler {

    private static final Map<URL, PakFileSystem> cache =
                                new HashMap<URL, PakFileSystem>();
    
    private static class PAKConnection extends URLConnection {

        private PakFileSystem pakFile;
        private String fileName;
        
        private InputStream in;
        
        public PAKConnection(URL url){
        	super(url);
        }
        
        public void connect() throws IOException {
            
        }
        
    }
    
    @Override
    protected URLConnection openConnection(URL url) throws IOException {
        String fullFile = url.getFile();
        URL archiveURL = new URL(fullFile.substring(0, fullFile.indexOf('!')));
        
        PakFileSystem pakFile = cache.get(archiveURL);
        if (pakFile == null){
            
        }
        
        return null;
    }

    public static void main(String[] args) throws MalformedURLException, IOException{
        URL url = new URL("pak:file:/E:/test.pak!/myfile.txt");
        url.openStream();
    }
    
}
