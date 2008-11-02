package com.radakan.gui.dialogs;

import com.jme.input.controls.GameControl;
import com.jme.input.controls.GameControlManager;
import com.jme.system.GameSettings;
import com.jmex.editors.swing.controls.GameControlEditor;
import java.awt.BorderLayout;
import java.awt.Container;
import java.awt.FlowLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.SwingUtilities;

public class ControlSettingsDialog extends JFrame {

    private static GameControlManager manager;
    private GameSettings settings;
    private boolean accepted = false;
    
    /**Determines if the dialog is open.*/
    private boolean isOpen = true;
        
    /**
     * Lock for waiting till the window closes
     */
    private Object openLock = new Object();
    
    public ControlSettingsDialog(GameControlManager manager, GameSettings settings){
        setTitle("Configure Controls");
        this.manager = manager;
        this.settings = settings;
        
        addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosing(WindowEvent e) {
                accepted = false;
                closeDialog();
            }
        });
        
        Container c = getContentPane();
        c.setLayout(new BorderLayout());
        
        final GameControlEditor editor = new GameControlEditor(manager, 2);
        c.add(editor, BorderLayout.CENTER);
        
        JPanel bottom = new JPanel();
        bottom.setLayout(new FlowLayout());
        JButton button = new JButton("Cancel");
        button.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent evt) {
                        accepted = false;
                        closeDialog();
                }
        });
        bottom.add(button);
        button = new JButton("Clear");
        button.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent evt) {
                        editor.clear();
                }
        });
        bottom.add(button);
        button = new JButton("Reset");
        button.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent evt) {
                        editor.reset();
                }
        });
        bottom.add(button);
        button = new JButton("Save");
        button.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent evt) {
                        editor.apply();	// Apply bindings
                        GameControlManager.save(this.manager, this.settings);	// Save them
                        
                        accepted = true;
                        closeDialog();
                }
        });
        bottom.add(button);
        c.add(bottom, BorderLayout.SOUTH);
    }
    
    /**
     * Waits until the dialog box is closed.
     */
    public void waitFor() throws InterruptedException {
        if (SwingUtilities.isEventDispatchThread()) {
            throw new UnsupportedOperationException("Cannot be called from gui thread");
        }

        while (isOpen()) {
            synchronized (openLock){
                openLock.wait();
            }
        }
    }
    
    public boolean isAccepted(){
        return accepted;
    }
    
    /**
     * Determines if the dialog box is open.
     * 
     * @return True if it is open.
     */
    public boolean isOpen() {
        return isOpen;
    }
    
    /**
     * Configures the controls.
     */
    public void open() {
        pack();
        setVisible(true);
    }

    /**
     * Closes the dialog box.
     */
    protected void closeDialog() {
        setVisible(false);
        dispose();
        isOpen = false;
        synchronized (openLock){
            openLock.notifyAll();
        }
    }
    
}
