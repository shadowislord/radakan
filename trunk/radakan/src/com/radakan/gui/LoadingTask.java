package com.radakan.gui;

public abstract class LoadingTask {
    
    /**
     * Lock to maintain synchronization
     */
    private Object accessLock = new Object();
    
    private int progressPercent = 0;
    private String reportText = "Loading...";
    
    /**
     * Do intensive work in background. GL commands are not allowed.
     */
    public abstract void doInBackground();
    
    /**
     * Called in GL thread when the task is complete.
     */
    public abstract void done();
    
    /**
     * Called from doInBackground when some progress was done. Amount increases the percentage of work done, up to 100%.
     */
    public final void progress(int amount){
        synchronized (accessLock){
            progressPercent += amount;
        }
    }
    
    /**
     * Report what component is currently being loaded.
     * @param text Description of the component currently being loaded.
     */
    public final void progress(String text){
        synchronized (accessLock){
            reportText = text;
        }
    }
    
    public final void progress(int amount, String text){
        synchronized (accessLock){
            progressPercent += amount;
            reportText = text;
        }
    }
    
    public final int getProgress(){
        synchronized (accessLock){
            return progressPercent;
        }
    }
    
    public final String getText(){
        synchronized (accessLock){
            return reportText;
        }
    }
    
}
