package com.gibbon.maxipack;

public interface PostProgressListener {

    public void bytesSent(PostOutputStream out, int numBytes);
    public void transferSpeed(PostOutputStream out, int bytesPerSec);
    
}
