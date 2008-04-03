package com.gibbon.maxipack;

import java.awt.Desktop;
import java.awt.Toolkit;
import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.math.BigInteger;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLConnection;
import java.net.URLEncoder;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import javax.swing.JOptionPane;
import javax.swing.SwingWorker;

/**
 * A SwingWorker implementation to upload
 * files to a server using HTTP post
 * @author Momoko_Fan
 */
public class MaxiUploader extends SwingWorker<Void, Void> {

    /**
     * Algorithm name to generate the file hash
     */
    private static final String HASH_ALGORITHM = "MD5";
    
    /**
     * The chunk to be uploaded next
     */
    private int curChunk = 0;
    
    /**
     * Remaining bytes needed to be uploaded
     */
    private long remaining;
    
    /**
     * File size
     */
    private long filesize;
    
    /**
     * Input stream for the data to be sent
     */
    private InputStream in;
    
    /**
     * The URL to upload the file to
     */
    private URL uploadURL;
    
    /**
     * MessageDigest to create the file hashes
     */
    private MessageDigest digester;
    
    /*
     * 2 MB buffer
     * Each chunk is 2MB
     */
    private byte[] buffer = new byte[1024 * 1024 * 2];
    
    private String filename = "debug.txt";
    
    // Some constants used for sending multipart messages
    private static final String ENDLINE = "\r\n";
    private static final String HYPHENS = "--";
    private static final String BOUNDARY = "*****";
    
    /**
     * Create a hash in hex format for the given data
     * 
     * @param data Input data to create the hash from
     * @return Hash as a string of hex
     */
    private String makeMD5Hash(byte[] data){
        digester.update(data);
        BigInteger bi = new BigInteger(digester.digest());
        return bi.toString(16);
    }
    
    private void sendNextChunk(int chunkID) {
        try {
            // should be something like
            // http://www.radakan.org/upload.php?chunk_id=15
            URL chunkURL = new URL(uploadURL.toString() + "?chunk_id=" + chunkID);

            HttpURLConnection conn = (HttpURLConnection) chunkURL.openConnection();
            
            conn.setDoInput(true);
            conn.setDoOutput(true);
            conn.setUseCaches(false);

            // Use a post method.
            conn.setRequestMethod("POST");

            conn.setRequestProperty("Connection", "Keep-Alive");
            conn.setRequestProperty("Content-Type", "multipart/form-data;boundary=" + BOUNDARY);
            conn.setRequestProperty("Cache-Control", "no-cache");
            
            DataOutputStream dos = new DataOutputStream(conn.getOutputStream());
            
            dos.writeBytes(HYPHENS + BOUNDARY + ENDLINE);
            dos.writeBytes("Content-Disposition: form-data; name=\"file\"; filename=\"" + filename + "\"" + ENDLINE);
            dos.writeBytes(ENDLINE);

            dos.writeBytes("Content-Type: application/octet-stream");
            dos.writeBytes(ENDLINE);
            dos.writeBytes(ENDLINE);
            
            
            dos.writeBytes("FILE DATA GOES HERE");
            remaining = 0;
            
//            int chunkRemaining = (int) Math.min(remaining, buffer.length);
//            int bytesRead = in.read(buffer, 0, chunkRemaining);
//            
//            while (chunkRemaining > 0) {
//                dos.write(buffer, 0, bytesRead);
//                remaining -= bytesRead;
//                chunkRemaining -= bytesRead;
//                System.out.println("Uploaded: "+bytesRead+" bytes");
//                bytesRead = in.read(buffer, 0, chunkRemaining);
//            }

            // send multipart form data necesssary after file data...
            dos.writeBytes(ENDLINE);
            dos.writeBytes(HYPHENS + BOUNDARY + HYPHENS + ENDLINE);

            // close streams
            dos.flush();
            dos.close();

            File tempFile = File.createTempFile("chunk"+chunkID, ".html");
            FileWriter fos = new FileWriter(tempFile);

            // Get response data.
            BufferedReader r = new BufferedReader(new InputStreamReader(conn.getInputStream()));
            while (r.ready()) {
                fos.write(r.readLine());
                fos.write("\r\n");
            }
            r.close();
            fos.close();

            Desktop.getDesktop().browse(tempFile.toURI());
            
            // update progress bar
            setProgress((int) (((filesize - remaining) * 100) / filesize));
        } catch (MalformedURLException ex) {
            ex.printStackTrace();
            MaxiPanel.showError(ex.getLocalizedMessage());
        } catch (IOException ex) {
            // This would be a common error
            // add some error messages here..
            ex.printStackTrace();

            if (ex instanceof FileNotFoundException) {
                MaxiPanel.showError("File does not exist: " + ex.getLocalizedMessage());
            } else {
                MaxiPanel.showError(ex.getLocalizedMessage());
            }
        }

    }
    
    public MaxiUploader(File toUpload, URL where){
        try {
            in = new BufferedInputStream(new FileInputStream(toUpload));
            digester = MessageDigest.getInstance(HASH_ALGORITHM);
            uploadURL = where;
            filesize = toUpload.length();
            remaining = filesize;
        } catch (FileNotFoundException ex) {
            // shouldn't happen since we check
            // for errors elsewhere
            ex.printStackTrace();
            MaxiPanel.showError(ex.getLocalizedMessage());
        } catch (NoSuchAlgorithmException ex){
            // if you specify a HASH_ALGORITHM that doesn't exist
            // or is not supported on this syste it happens
            ex.printStackTrace();
            MaxiPanel.showError(ex.getLocalizedMessage());
        }
    }
    
    @Override
    protected Void doInBackground() throws Exception {
        setProgress(1);
        
        while (remaining > 0 && !isCancelled())
            sendNextChunk(curChunk++);
        
        setProgress(100);
        
        return null;
    }
    
    @Override
    protected void done(){
        //Toolkit.getDefaultToolkit().beep();
        JOptionPane.showMessageDialog(MaxiPanel.INSTANCE, 
                                      "File uploaded successfuly",
                                      "Success",
                                      JOptionPane.INFORMATION_MESSAGE);
        
        MaxiPanel.INSTANCE.exitUploadState();
    }

}
