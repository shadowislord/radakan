/* Radakan is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Radakan is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Radakan.  If not, see <http://www.gnu.org/licenses/>.
 */
package com.radakan.gui.dialogs;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.DisplayMode;
import java.awt.FlowLayout;
import java.awt.Graphics;
import java.awt.GraphicsDevice;
import java.awt.GraphicsEnvironment;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Image;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.util.logging.Logger;

import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JComboBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTabbedPane;
import javax.swing.SwingUtilities;

import com.jme.system.GameSettings;
import com.radakan.game.Game;
import com.radakan.util.ImageCache;

/**A dialog box for configuring the game settings.
 * 
 * @author Joshua Montgomery
 * @version 1.0.0
 * @created Jul 28, 2008
 */
public class GameSettingsDialog extends JFrame 
{
	private static final long serialVersionUID = -5863149042365826108L;
	private static Logger logger = Logger.getLogger(GameSettingsDialog.class.getName());
    private static final int FRAME_WIDTH = 573;
    
    /**The game settings to set.*/
    private GameSettings settings;
    
    /**Determines if the dialog is open.*/
    private boolean isOpen = true;
        
    /**
     * Lock for waiting till the window closes
     */
    private Object openLock = new Object();
    
    /**Determines if the game should start.*/
    private boolean startGame = false;
    
    private JComboBox listCb;
    private JCheckBox cb, cb2;

    /**Constructor - Creates a dialog for setting up the game.
     * 
     * @param settings The game settings to set.
     */
    public GameSettingsDialog(GameSettings settings) {
        super("Configuration - "+Game.getGameName());
        
        if (settings == null) {
            throw new NullPointerException("Settings can not be null.");
        }

        this.settings = settings;
        setupFrame();
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

    /**
     * Determines if the dialog box is open.
     * 
     * @return True if it is open.
     */
    public boolean isOpen() {
        return isOpen;
    }

    /**Sets up the main frame.*/
    private void setupFrame() {
        logger.fine("Loading the configuration dialog...");

        //SETUP JFRAME		
        addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosing(WindowEvent e) {
                closeDialog();
            }
        });

        setPreferredSize(new Dimension(FRAME_WIDTH, 350));
        setResizable(false);
        setLocationByPlatform(true);

        //SETUP COMPONENTS OF FRAME		
        GraphicsDevice gd = GraphicsEnvironment.getLocalGraphicsEnvironment().getDefaultScreenDevice();

        JPanel imagePl = new JPanel() {
            private static final long serialVersionUID = 6279143708867242615L;
            private Image bgImg = ImageCache.retrieveCachedImage("/com/radakan/data/images/splash.png");

            @Override
            public void paint(Graphics g) {
                g.drawImage(bgImg, 0, 0, bgImg.getWidth(this), bgImg.getHeight(this), this);
            }
        };

        JPanel graphicsPl = new JPanel();
        JPanel audioPl = new JPanel();
        JPanel commandPl = new JPanel();
        JTabbedPane tabs = new JTabbedPane();

        //Add panels to tabs
        tabs.addTab("Graphics", graphicsPl);
        tabs.addTab("Audio", audioPl);

        graphicsPl.setLayout(new GridBagLayout());
        graphicsPl.setPreferredSize(new Dimension(FRAME_WIDTH, 75));
        GridBagConstraints c = new GridBagConstraints();

        JLabel selectLb = new JLabel("Display Mode: ");

        c.fill = GridBagConstraints.HORIZONTAL;
        c.gridx = 0;
        c.gridy = 0;
        graphicsPl.add(selectLb, c);

        //SETUP GRAPHICS PANEL
        //setup up display modes
        listCb = new JComboBox();
        listCb.setSize(60, 20);
        listCb.setPreferredSize(new Dimension(150, 20));

        DisplayMode displays[] = gd.getDisplayModes();
        for (int i = 0; i < displays.length; i++) {
            listCb.addItem(new DisplayWrapper(displays[i]));
        }
        
        listCb.setSelectedItem(new DisplayWrapper(gd.getDisplayMode()));

        c.insets = new Insets(0, 0, 0, 20);
        c.gridx = 1;
        graphicsPl.add(listCb, c);

        //setup full screen check box		
        JLabel fullLb = new JLabel("Full Screen: ");

        c.insets = new Insets(0, 0, 0, 0);
        c.gridx = 2;
        graphicsPl.add(fullLb, c);

        cb = new JCheckBox();

        c.gridx = 3;
        graphicsPl.add(cb, c);

        //setup vertical sync check box
        JLabel syncLb = new JLabel("Vertical Sync: ");

        c.insets = new Insets(0, 10, 0, 0);
        c.gridx = 4;
        graphicsPl.add(syncLb, c);

        cb2 = new JCheckBox();

        c.insets = new Insets(0, 0, 0, 0);
        c.gridx = 5;
        graphicsPl.add(cb2, c);

        //SETUP THE AUDIO PANEL


        //SETUP THE COMMAND PANEL
        commandPl.setLayout(new FlowLayout(FlowLayout.RIGHT));
        commandPl.setPreferredSize(new Dimension(FRAME_WIDTH, 35));

        JButton startBt = new JButton("Start");
        startBt.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                startGame = true;
                saveSettings();
                closeDialog();
            }
        });
        commandPl.add(startBt);

        JButton quitBt = new JButton("Quit");
        quitBt.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                closeDialog();
            }
        });
        commandPl.add(quitBt);

        //finalize the frame by adding panenls to it
        getContentPane().setLayout(new BorderLayout());

        JPanel subPanel = new JPanel(new BorderLayout());
        subPanel.add(tabs);
        subPanel.add(commandPl, BorderLayout.SOUTH);

        add(imagePl);
        add(subPanel, BorderLayout.SOUTH);
    }

    /**
     * Configures the game settings.
     */
    public void open() {
        pack();
        setVisible(true);
    }

    /**
     * Closes the dialog box.
     */
    public void closeDialog() {
        setVisible(false);
        dispose();
        isOpen = false;
        synchronized (openLock){
            openLock.notifyAll();
        }
        logger.fine("Configuration Finished!");
    }
    
    /**Saves the game settings set with the dialog.*/
    public void saveSettings(){
        if (listCb.getSelectedItem() instanceof DisplayWrapper) {
            DisplayMode mode = ((DisplayWrapper) listCb.getSelectedItem()).mode;
            settings.setDepth(mode.getBitDepth());
            settings.setHeight(mode.getHeight());
            settings.setWidth(mode.getWidth());
            settings.setFrequency(mode.getRefreshRate());
        }
        
        settings.setFullscreen(cb.isSelected());
        settings.setVerticalSync(cb2.isSelected());
    }

    /**Determines if the game is allowed to initialize.
     * 
     * @return True if the game is allowed to initialize.
     */
    public boolean isInitGameAllowed() {
        return startGame;
    }

    /**A simple wrapper for a <code>DisplayMode</code>. 
     * 
     * @author Joshua Montgomery
     * @version 1.0.0
     * @created Jul 28, 2008
     */
    private class DisplayWrapper {

        /**The wrapped <code>DisplayMode</code>.*/
        DisplayMode mode;

        /**Constructor - Constructs a wrapper for a <code>DisplayMode</code>.
         * 
         * @param mode The <code>DisplayMode</code> to wrap.
         */
        public DisplayWrapper(DisplayMode mode) {
            this.mode = mode;
        }

        public boolean equals(Object o) {
            if (o instanceof DisplayWrapper) {
                return mode.equals(((DisplayWrapper) o).mode);
            }
            return mode.equals(o);
        }

        public String toString() {
            return (mode.getWidth() + "x" + mode.getHeight() +
                    "x" + mode.getBitDepth() + " @" + mode.getRefreshRate());
        }
    }
}
