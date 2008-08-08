package com.gibbon.maxipack;

import javax.swing.JApplet;
import javax.swing.JFrame;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.SwingUtilities;
import javax.swing.UIManager;
import javax.swing.UnsupportedLookAndFeelException;

public class MaxiApplet extends JApplet {

    static MaxiApplet theApplet;
    static JFrame theFrame;
    
    static MaxiUploadPanel upPanel;
    static MaxiLoginPanel loginPanel;
    
    static JPanel currentPanel;
    static boolean asApplet = false;

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
        asApplet = true;
        theApplet = this;
        preinit();
        
        Server.setHost(getCodeBase());
        
        loginPanel = new MaxiLoginPanel();
        upPanel = new MaxiUploadPanel();
    }
    
    @Override
    public void start(){
        add(loginPanel);
        currentPanel = loginPanel;
    }
    
    public static void enterUploadForm(){
        if (asApplet){
            theApplet.remove(loginPanel);
            theApplet.add(upPanel);
            theApplet.validate();
        }else{
            theFrame.setContentPane(upPanel);
//            theFrame.getContentPane().remove(loginPanel);
//            theFrame.getContentPane().add(upPanel);
            theFrame.pack();
            theFrame.validate();
        }
        currentPanel = upPanel;
    }
    
    /**
     * Create the GUI and show it. 
     * As with all GUI code, this must run on the event-dispatching thread.
     */
    private static void startFrame() {
        //Create and set up the window.
        theFrame = new JFrame("MaxiPacker v0.7");
        theFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        //Create and set up the content pane.
        upPanel = new MaxiUploadPanel();
        upPanel.setOpaque(true); 

        loginPanel = new MaxiLoginPanel();
        loginPanel.setOpaque(true);

//      theFrame.getContentPane().add(loginPanel);
        theFrame.setContentPane(loginPanel);

        // places the frame in the center of the screen
        theFrame.setLocationRelativeTo(null);

        //Display the window.
        theFrame.pack();
        theFrame.setResizable(false);
        theFrame.setVisible(true);
    }

    public static void showError(String message){
        JOptionPane.showMessageDialog(currentPanel, 
                                      message,
                                      "Error",
                                      JOptionPane.ERROR_MESSAGE);
                                      
    }
    
    /**
     * Main method in-case we are executing as application
     * @param args
     */
    public static void main(String[] args) {
        asApplet = false;
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
