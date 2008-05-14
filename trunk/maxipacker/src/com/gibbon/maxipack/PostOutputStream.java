package com.gibbon.maxipack;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.net.HttpURLConnection;

public class PostOutputStream extends OutputStream {

    /**
     * End line used in the HTTP multipart POST content area<br/>
     */
    private static final String ENDLINE = "\r\n";
    
    /**
     * Hyphenes..
     */
    private static final String HYPHENS = "--";
    
    /**
     * Boundary that seperates the parts in a multipart message
     */
    private static final String BOUNDARY = "*****";
    
    /**
     * Total number of bytes written to the stream
     */
    private int written = 0;
    
    /**
     * Whether the listener should be notified of writes
     */
    private boolean active = false;
    
    /**
     * The listener to listen for byte writes
     */
    private PostProgressListener listener;
    
    /**
     * The connection this post output stream belonds to
     */
    private HttpURLConnection conn;
    
    /**
     * The output stream being filtered
     */
    private OutputStream out;
    
    /**
     * Internal input stream for buffering
     * @param conn
     * @param listener
     */
    private ByteArrayOutputStream buf;
    
    private int bytesSentPerSec = 0;
    
    private long lastTimeUpdated = 0;
    
    public PostOutputStream(HttpURLConnection conn, PostProgressListener listener){
        this.listener = listener;
        this.conn = conn;
        
        buf = new ByteArrayOutputStream();
    }
    
    public void setActive(boolean enabled){
        active = enabled;
    }
    
    public void postFile(String name, String filename, byte[] data, int length) throws IOException{
        writeBytesSilent(HYPHENS + BOUNDARY + ENDLINE);
        writeBytesSilent("Content-Disposition: form-data; name=\"" + name + "\"; filename=\"" + filename + "\"" + ENDLINE);
        writeBytesSilent("Content-Transfer-Encoding: binary" + ENDLINE);
        writeBytesSilent("Content-Type: application/octet-stream" + ENDLINE);
        writeBytesSilent(ENDLINE);
        
        write(data, 0, length);
        
        writeBytesSilent(ENDLINE);
    }
    
    public void postField(String name, String value) throws IOException{
        writeBytesSilent(HYPHENS + BOUNDARY + ENDLINE);
        writeBytesSilent("Content-Disposition: form-data; name=\""+name+"\"" + ENDLINE);
        writeBytesSilent(ENDLINE);
        
        writeBytesSilent(value);
        
        writeBytesSilent(ENDLINE);
    }
    
    public void postComplete() throws IOException{
        writeBytesSilent(HYPHENS + BOUNDARY + HYPHENS + ENDLINE);
    }
    
    protected void writeBytesSilent(String data) throws IOException{
        int len = data.length();
        for (int i = 0; i < len; i++)
            write(data.charAt(i));
    }
    
    @Override
    public void close() throws IOException{
        conn.setRequestMethod("POST");

        conn.setDoInput(true);
        conn.setDoOutput(true);
        conn.setUseCaches(false);
        
        conn.setRequestProperty("Connection",    "Keep-Alive");
        conn.setRequestProperty("Content-Type",  "multipart/form-data; boundary=" + BOUNDARY);
        conn.setRequestProperty("Cache-Control", "no-cache");
        
        // enable streaming of HTTP data
        conn.setFixedLengthStreamingMode(written);
        
        out = conn.getOutputStream();
        
        byte[] data = buf.toByteArray();
        int remaining = data.length;
        int offset = 0;
        
        lastTimeUpdated = System.currentTimeMillis();
        long currentTime = System.currentTimeMillis();
        
        // stream by 1024 byte size chunks
        while (remaining > 0){
            int toWrite = Math.min(remaining, 1024);
            out.write(data, offset, toWrite);
            listener.bytesSent(this, toWrite);
            bytesSentPerSec += toWrite;
            currentTime = System.currentTimeMillis();
            if (currentTime - lastTimeUpdated > 1000){
                lastTimeUpdated = currentTime;
                listener.transferSpeed(this, bytesSentPerSec);
                bytesSentPerSec = 0;
            }
            
            offset += toWrite;
            remaining -= toWrite;
        }

        out.close();
    }
    
    public void write(int b) throws IOException{
        buf.write(b);
        written++;
    }
    
    @Override
    public void write(byte[] b, int off, int len) throws IOException {
        buf.write(b, off, len);
        written+=len;
    }
    
    @Override
    public void write(byte[] b) throws IOException{
        write(b, 0, b.length);
    }

    @Override
    public void flush() throws IOException{
        buf.flush();
    }
    
}
