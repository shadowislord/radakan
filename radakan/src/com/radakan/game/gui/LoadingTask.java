package com.radakan.game.gui;

/**
 * 
 * 
 * @author Kirill Vainer
 */
public abstract class LoadingTask {
    
    /**
     * Lock to maintain synchronization
     */
    private Object accessLock = new Object();
    
    private int progressPercent = 0;
    private String reportText = "Loading...";
    
    /**
     * Do intensive and long work in background. GL commands are not allowed.
     */
    public abstract void doInBackground();
    
    /**
     * Called in GL thread when the task is complete. This is where
     * the loaded resources are put into use.
     */
    public abstract void done();
    
    /**
     * Called from doInBackground when some progress was done. 
     * @param increases the percentage of work done, up to 100%.
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
    
    /**
     * Combines both 1-argument versions of the progress method.
     * 
     * @param amount increases the percentage of work done, up to 100%.
     * @param text Description of the component currently being loaded.
     */
    public final void progress(int amount, String text){
        synchronized (accessLock){
            progressPercent += amount;
            reportText = text;
        }
    }
    
    /**
     * @return the current progress as a percentage.
     */
    public final int getProgress(){
        synchronized (accessLock){
            return progressPercent;
        }
    }
    
    /**
     * @return The text last set with the String based progress methods.
     */
    public final String getText(){
        synchronized (accessLock){
            return reportText;
        }
    }
    
}
