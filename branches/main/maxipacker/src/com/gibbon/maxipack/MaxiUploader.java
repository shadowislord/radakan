package com.gibbon.maxipack;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.math.BigInteger;
import java.net.MalformedURLException;
import java.security.DigestInputStream;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.Arrays;
import java.util.List;
import javax.swing.JOptionPane;
import javax.swing.SwingWorker;

/**
 * A SwingWorker implementation to upload
 * files to a server using HTTP post
 * @author Momoko_Fan
 */
public class MaxiUploader extends SwingWorker<Void, String> implements PostProgressListener {

    /**
     * Algorithm name to generate the file hash
     */
    private static final String HASH_ALGORITHM = "MD5";
    
    /**
     * The maximum amount of bytes that a server can accept at once
     */
    private static final int CHUNK_SIZE = 2 * 1024 * 1024;
    //private static final int CHUNK_SIZE = 100;
    
    /**
     * The chunk to be uploaded next
     */
    private int curChunk = 0;
    
    /**
     * Remaining bytes needed to be uploaded
     */
    private long remaining;
    
    /**
     * Remaining bytes needed to be uploaded, more accurate than 'remaining'
     * as it is updated live as the bytes are sent to the server.
     * Used by MaxiProgressOutputStream when updating progress bar to 
     * show more accurate progress of the upload.
     */
    private long liveRemaining;
    
    /**
     * File size
     */
    private long filesize;
    
    /**
     * Input stream for the data to be sent
     */
    private InputStream in;
    
    /**
     * MessageDigest to create the file hashes
     */
    private MessageDigest digester;
    
    /**
     * Name of the file to upload
     */
    private String filename;
    
    
    /**
     * buffer for doing read/write operations.
     * Size is half the amount of bytes the server can accept, as a safeguard.
     */
    private byte[] buffer = new byte[CHUNK_SIZE];
    
    /**
     * If the uploading is proceeding smoothly, this is true.
     * If an error has occured, this is false.
     */
    private boolean success;
    
    /**
     * The url returned by the server that says where the file is.
     * This is non-null only after the uploading has finished.
     */
    private String fileServerPath;
    
    /**
     * The MD5 hash for the file returned by the server.
     * This is non-null only after the uploading has finished.
     */
    private String fileServerMD5;
    
    private String speedString = "0 B/s";
    
    private String makeMD5Hash(){
        BigInteger bi = new BigInteger(digester.digest());
        return bi.toString(16);
    }
    
    private boolean checkMD5Match(){
        BigInteger bi = new BigInteger(fileServerMD5, 16);
        byte[] hisMd5 = bi.toByteArray();
        byte[] myMd5  = digester.digest();
        
        return Arrays.equals(hisMd5, myMd5);
    }
    
    @Override
    public void process(List<String> chunks){
        firePropertyChange("speed", speedString, chunks.get(chunks.size()-1));
        speedString = chunks.get(chunks.size()-1);
    }
    
    public void bytesSent(PostOutputStream out, int num){
        liveRemaining -= num;
        
        
        int progress = (int) (((filesize - liveRemaining) * 100) / filesize);
        
        if (progress < 0)
            progress = 0;
        else if (progress > 100)
            progress = 100;
        
        setProgress(progress);
    }
    
    public void transferSpeed(PostOutputStream out, int speed) {
        StringBuffer sb = new StringBuffer();
        
        if      (speed < 1024)        sb.append(speed).append(" B/s");
        else if (speed < 1024 * 1024) sb.append(speed / 1024).append(" KB/s");
        else                          sb.append(speed / 1024 * 1024).append(" MB/s"); 
        
        publish(sb.toString());
    }
    
    public MaxiUploader(File toUpload){
        try {
            digester = MessageDigest.getInstance(HASH_ALGORITHM);
            filesize = toUpload.length();
            remaining = filesize;
            liveRemaining = remaining;
            filename = toUpload.getName();
            
            in = new BufferedInputStream(new FileInputStream(toUpload));
            DigestInputStream dis = new DigestInputStream(in, digester);
            in = dis;
        } catch (FileNotFoundException ex) {
            // shouldn't happen since we check
            // for errors elsewhere
            ex.printStackTrace();
            MaxiApplet.showError(ex.getLocalizedMessage());
        } catch (NoSuchAlgorithmException ex){
            // if you specify a HASH_ALGORITHM that doesn't exist
            // or is not supported on this system it happens
            ex.printStackTrace();
            MaxiApplet.showError(ex.getLocalizedMessage());
        }
    }
    
    @Override
    protected Void doInBackground() throws Exception {
        setProgress(0);
        
        while (remaining > 0 && !isCancelled()){
            success = sendNextChunk(curChunk++); 
            if (!success){
                break;
            }
        }
        
        if (success){
            success = reform();
        }
        setProgress(100);
        
        return null;
    }
    
    @Override
    protected void done(){
        //Toolkit.getDefaultToolkit().beep();
        if (success){
            StringBuffer sb = new StringBuffer();
            sb.append("File uploaded successfuly.\n")
              .append("File path is: "+fileServerPath+"\n")
              .append("MD5 hash: "+fileServerMD5+"\n")
              .append("Matches with server? "+(checkMD5Match() ? "OK" : "NO"));
            
            JOptionPane.showMessageDialog(MaxiUploadPanel.INSTANCE, 
                                          sb.toString(),
                                          "Success",
                                          JOptionPane.INFORMATION_MESSAGE);
        }
        
        MaxiUploadPanel.INSTANCE.exitUploadState();
    }
    
    /**
     * Force server to reconstruct file
     */
    protected boolean reform(){
        Server.requestMessage("Reform requested for file "+filename);
        String encodedFilename = filename.replaceAll(" ", "+");
        Response r = Server.doRequest("reform", "filename="+encodedFilename);

        if (r != null){
            if (r.containsKey("url")){
                fileServerPath = r.getProperty("url");
                System.out.println("URL: "+fileServerPath);
            }
            if (r.containsKey("md5")){
                fileServerMD5 = r.getProperty("md5");
                System.out.println("Server MD5: "+fileServerMD5);
                System.out.println("My MD5: "+makeMD5Hash());
            }
            
            return true;
        }
        
        return false;
    }
    
    /**
     * Sends the next chunk with the provided ID
     * @param chunkID
     */
    private boolean sendNextChunk(int chunkID) {
        try {
            // we only use half the allowed filesize for uploading
            int chunkRemaining = (int) Math.min(remaining, CHUNK_SIZE / 2);
            int bufferOffset = 0;
            
            while (chunkRemaining > 0) {
                int bytesRead = in.read(buffer, bufferOffset, chunkRemaining);
                
                bufferOffset += bytesRead;
                remaining -= bytesRead;
                chunkRemaining -= bytesRead;
            }

            Server.requestMessage("Sending upload for user \"" + Server.getUserName() + "\" and chunk#"+chunkID);
            Response r = Server.postFile("upload", true, "chunk_id="+chunkID, filename, buffer, bufferOffset, this);
            
            if (r == null){
                MaxiApplet.showError("A server error has occured!");
                return false; // we failed
            }
            
            liveRemaining = remaining;
            setProgress((int) (((filesize - remaining) * 100) / filesize));
            
            return true; // we succeeded
        } catch (MalformedURLException ex) {
            ex.printStackTrace();
            MaxiApplet.showError(ex.getLocalizedMessage());
        } catch (IOException ex) {
            // This would be a common error
            // add some error messages here..
            ex.printStackTrace();
            if (ex instanceof FileNotFoundException) {
                MaxiApplet.showError("File does not exist: " + ex.getLocalizedMessage());
            } else {
                MaxiApplet.showError(ex.getLocalizedMessage());
            }
        } catch (Throwable ex){
            ex.printStackTrace();
        }

        return false;
    }

}
