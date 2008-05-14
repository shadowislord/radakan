package com.gibbon.maxipack;

import java.io.IOException;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;

public final class Server {

    private static String maxipackPath = "http://www.radakan.org/maxipacker/";
    private static String user, pass;
    
    public static void setHost(URL host) {
        // does not care
    }
    
    public static void setLoginInfo(String user, String pass){
        Server.user = user;
        Server.pass = pass;
    }
    
    public static String getUserName(){
        return user;
    }
    
    public static final String decodeErrorCode(int code){
        switch(code){
            case 0:
                return "OK";
            case 10: 
                return "Directory Error";
            case 11:
                return "Could not create base operation directory";
            case 12:
                return "Cound not create user directory";
            case 13:
                return "The user directory that was specefied does not exist";
            case 14:
                return "The upload queue that was specefied does not exist";
            case 21:
                return "Could not open final file for writing";
            case 22:
                return "Could not write to final file";
            case 23:
                return "Could not write to chunk file, or read uploaded chunk";
            case 24:
                return "Could not open chunk list for append";
            case 25:
                return "Could not write to chunk list";
            case 30:
                return "Login verified";
            case 31:
                return "Incorrect username/password";
            default:
                return "Unknown error code: "+code;
        }
    }
    
    public static final void responseMessage(String cause, int code){
        System.out.println("<-: "+cause+" - "+decodeErrorCode(code)+" ("+code+")");
    }
    
    public static final void requestMessage(String cause){
        System.out.println("->: "+cause);
    }
    
    public static Response postFile(String id, boolean sendLogin, String params, String filename, byte[] data, int length, PostProgressListener listener){
        StringBuffer urlBuf = new StringBuffer();
        urlBuf.append(maxipackPath);
        urlBuf.append(id).append(".php");
        if (sendLogin){
            urlBuf.append("?user=").append(user).append("&pass=").append(pass);
        }
        if (params != null){
            if (!sendLogin) urlBuf.append("?");
            else urlBuf.append("&");
            urlBuf.append(params);
        }
        
        URL request = null;
        try{
            request = new URL(urlBuf.toString());
        } catch (MalformedURLException ex){
            ex.printStackTrace();
        }
        
        try{
            HttpURLConnection conn = (HttpURLConnection) request.openConnection();
            PostOutputStream out = new PostOutputStream(conn, listener);
            out.setActive(true);
            
            out.postFile("file", filename, data, length);
            out.postComplete();
            out.close();
            
            InputStream in = conn.getInputStream();
//            File temp = File.createTempFile("file_test", ".html");
//            OutputStream fout = new FileOutputStream(temp);
//            
//            byte[] buf = new byte[512];
//            int avail = in.available();
//            while (avail > 0){
//                int read = in.read(buf, 0, Math.min(buf.length, avail));
//                avail -= read;
//                fout.write(buf, 0, read);
//            }
//            fout.close();
//            Desktop.getDesktop().browse(temp.toURI());
            
            Response r = new Response();
            r.load(in);
            in.close();
            
            if (r.getStatus() == 0)
                return r;
        } catch (IOException ex){
            ex.printStackTrace();
            MaxiApplet.showError("Error on request: "+ex.getLocalizedMessage());
        }
        
        return null;
    }
    
    public static Response doRequest(String id){
        return doRequest(id, null);
    }
            
    
    public static Response doRequest(String id, String params){
        return doRequest(id, true, params);
    }
    
    public static Response doRequest(String id, boolean sendLogin, String params){
        StringBuffer urlBuf = new StringBuffer();
        urlBuf.append(maxipackPath);
        urlBuf.append(id).append(".php");
        if (sendLogin){
            urlBuf.append("?user=").append(user).append("&pass=").append(pass);
        }
        if (params != null){
            if (!sendLogin) urlBuf.append("?");
            else urlBuf.append("&");
            urlBuf.append(params);
        }
        
        URL request = null;
        try{
            request = new URL(urlBuf.toString());
        } catch (MalformedURLException ex){
            ex.printStackTrace();
        }
        
        try{
            InputStream in = request.openStream();
            Response r = new Response();
            r.load(in);
            in.close();
            
            responseMessage(id, r.getStatus());
            
            if (r.getStatus() == 0)
                return r;
        } catch (IOException ex){
            ex.printStackTrace();
            MaxiApplet.showError("Error on request: "+ex.getLocalizedMessage());
        }
        
        return null;
    }

}
