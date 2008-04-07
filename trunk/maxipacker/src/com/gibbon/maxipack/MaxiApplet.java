package com.gibbon.maxipack;

import java.applet.Applet;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.JFrame;
import javax.swing.SwingUtilities;
import javax.swing.UIManager;
import javax.swing.UnsupportedLookAndFeelException;

public class MaxiApplet extends Applet {

    static URL uploadURL;
    static MaxiApplet theApplet;
    static MaxiPanel thePanel;
    
    public static final long serialVersionUID = 1;
    
    public static void preinit(){
        try {
            UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
        } catch (ClassNotFoundException ex) {
            ex.printStackTrace();
        } catch (InstantiationException ex) {
            ex.printStackTrace();
        } catch (IllegalAccessException ex) {
            ex.printStackTrace();
        } catch (UnsupportedLookAndFeelException ex) {
            ex.printStackTrace();
        }
    }
    
    @Override
    public void init(){
        preinit();
        
        // load the url where the applet is hosted
        // so we know where to upload stuff
        try{
            uploadURL = new URL(getCodeBase(), "test_page.php");
        } catch (MalformedURLException ex) {
            // shouldn't happen
            ex.printStackTrace();
        }
        
        thePanel = new MaxiPanel();
    }
    
    @Override
    public void start(){
        add(thePanel);
    }
    
    
    /**
     * Create the GUI and show it. 
     * As with all GUI code, this must run on the event-dispatching thread.
     */
    private static void startFrame() {
        try {
            //Create and set up the window.
            JFrame frame = new JFrame("MaxiPacker v0.2");
            frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
            
            //Create and set up the content pane.
            thePanel = new MaxiPanel();
            thePanel.setOpaque(true); 
            frame.setContentPane(thePanel);

            // places the frame in the center of the screen
            frame.setLocationRelativeTo(null);
            
            //Display the window.
            frame.pack();
            frame.setResizable(false);
            frame.setVisible(true);
            
            uploadURL = new URL("http://www.radakan.org/file_test.php");
        } catch (MalformedURLException ex) {
            // should not happen
            ex.printStackTrace();
            MaxiPanel.showError(ex.getLocalizedMessage());
        }
    }

    /**
     * Main method in-case we are executing as application
     * @param args
     */
    public static void main(String[] args) {
        //Schedule a job for the event-dispatching thread:
        //creating and showing this application's GUI.
        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                preinit();
                startFrame();
            }
        });
    }
    
}
