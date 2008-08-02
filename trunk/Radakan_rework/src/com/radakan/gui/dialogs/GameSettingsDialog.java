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
import java.awt.GraphicsDevice;
import java.awt.GraphicsEnvironment;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.GridLayout;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JComboBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTabbedPane;

import com.jme.system.GameSettings;

/**A dialog box for configuring the game settings.
 * 
 * @author Joshua Montgomery
 * @version 1.0.0
 * @created Jul 28, 2008
 */
public class GameSettingsDialog 
{
	/**The game settings to set.*/
	private GameSettings settings;

	/**Determines if the dialog is active.*/
	private boolean active = true;

	/**Frame that shows config dialog.*/
	private JFrame frame = new JFrame("Configure Radakan - Developers Edition");
	
	/**Determines if the game should start.*/
	private boolean startGame = false;
	
	/**Constructor - Creates a dialog for setting up the game.
	 * 
	 * @param settings The game settings to set.
	 */
	public GameSettingsDialog(GameSettings settings)
	{
		if(settings == null)
			throw new NullPointerException("Settings can not be null.");
		
		this.settings = settings;	
		setupFrame();
	}
	
	/**Determines if the dialog box is open.
	 * 
	 * @return True if it is open.
	 */
	public boolean isOpen()
	{
		return active;
	}
		
	/**Sets up the main frame.*/
	private void setupFrame()
	{
		//SETUP JFRAME		
		frame.addWindowListener(new WindowAdapter(){			
			public void windowClosing(WindowEvent e)
			{
				closeDialog();
			}			
		});
		
		frame.setSize(600,400);
		frame.setPreferredSize(new Dimension(600,400));
		frame.setLocationByPlatform(true);
		
		//SETUP COMPONENTS OF FRAME		
		GraphicsDevice gd = GraphicsEnvironment.getLocalGraphicsEnvironment().getDefaultScreenDevice();
		
		JPanel imagePl = new JPanel();
		/*{	
			private Image bgImg = ImageCache.retrieveCachedImage("/data/images/background.png");
			
			public void paintComponent(Graphics g)
			{
				g.drawImage(bgImg,getWidth(),getHeight(),this);
			}
		}*/
		//imagePl.setSize(500,300);
				
		JPanel graphicsPl = new JPanel();
		JPanel audioPl = new JPanel();
		JPanel commandPl = new JPanel();
		JTabbedPane tabs = new JTabbedPane();
				
		//Add panels to tabs
		tabs.addTab("Graphics",graphicsPl);
		tabs.addTab("Audio", audioPl);
		
		graphicsPl.setLayout(new GridBagLayout());
		GridBagConstraints c = new GridBagConstraints();
		
		JLabel selectLb = new JLabel("Display Mode: ");
				
		c.fill = GridBagConstraints.HORIZONTAL;		
		c.gridx = 0;
		c.gridy = 0;	
		graphicsPl.add(selectLb,c);
		
		
		//setup up display modes
		JComboBox listCb = new JComboBox();	
		listCb.setSize(60,20);
		listCb.setPreferredSize(new Dimension(150,20));
		
		DisplayMode displays[] = gd.getDisplayModes();		
		for(int i=0;i < displays.length;i++)
		{
			listCb.addItem(new DisplayWrapper(displays[i]));
			
		}
		listCb.addItemListener(new ItemListener()
		{
			public void itemStateChanged(ItemEvent e)
			{
				if(e.getItem() instanceof DisplayWrapper)
				{
					DisplayMode mode = ((DisplayWrapper)e.getItem()).mode;
					settings.setDepthBits(mode.getBitDepth());
					settings.setHeight(mode.getHeight());
					settings.setWidth(mode.getWidth());
					settings.setFrequency(mode.getRefreshRate());
				}				
				
			}
			
		});
		listCb.setSelectedItem(new DisplayWrapper(gd.getDisplayMode()));
	
		c.insets = new Insets(0,0,0,20);
		c.gridx = 1;
		graphicsPl.add(listCb, c);
		
		//setup full screen check box		
		JLabel fullLb = new JLabel("Full Screen: ");
		
		c.insets = new Insets(0,0,0,0);
		c.gridx = 2;
		graphicsPl.add(fullLb,c);
		
		JCheckBox cb = new JCheckBox();
		cb.addItemListener(new ItemListener()
		{
			public void itemStateChanged(ItemEvent e)
			{				
				settings.setFullscreen(e.getStateChange() == ItemEvent.SELECTED);
			}			
		});
		
		c.gridx = 3;
		graphicsPl.add(cb,c);
		
		//setup vertical sync check box
		JLabel syncLb = new JLabel("Vertical Sync: ");
		
		c.insets = new Insets(0,10,0,0);
		c.gridx = 4;
		graphicsPl.add(syncLb,c);
		
		cb = new JCheckBox();
		cb.addItemListener(new ItemListener()
		{
			public void itemStateChanged(ItemEvent e)
			{				
				settings.setFullscreen(e.getStateChange() == ItemEvent.SELECTED);
			}			
		});
		
		c.insets = new Insets(0,0,0,0);
		c.gridx = 5;
		graphicsPl.add(cb,c);
		
		//SETUP THE COMMAND PANEL
		commandPl.setLayout(new FlowLayout(FlowLayout.RIGHT));
		commandPl.setMaximumSize(new Dimension(600,100));
		
		JButton startBt = new JButton("Start");
		startBt.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				startGame = true;
				closeDialog();				
			}			
		});
		commandPl.add(startBt);
		
		JButton quitBt = new JButton("Quit");
		quitBt.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				closeDialog();				
			}			
		});
		commandPl.add(quitBt);
		
		//finalize the frame by adding panenls to it
		frame.getContentPane().setLayout(new BorderLayout());
		
		JPanel subPanel = new JPanel(new GridLayout(2,0));
		subPanel.add(tabs);
		subPanel.add(commandPl);
		
		frame.add(imagePl);		
		frame.add(subPanel,BorderLayout.SOUTH);		
	}
	
	/**Configures the game settings.*/
	public void configure()
	{
		frame.pack();
		frame.setVisible(true);
	}	
	
	/**Closes the dialog box.*/
	public void closeDialog()
	{
		frame.setVisible(false);
		frame.dispose();
		active = false;
	}
	
	/**Determines if the game is allowed to initialize.
	 * 
	 * @return True if the game is allowed to initialize.
	 */
	public boolean isInitGameAllowed()
	{
		return startGame;
	}

	/**A simple wrapper for a <code>DisplayMode</code>. 
	 * 
	 * @author Joshua Montgomery
	 * @version 1.0.0
	 * @created Jul 28, 2008
	 */
	private class DisplayWrapper
	{
		/**The wrapped <code>DisplayMode</code>.*/
		DisplayMode mode;
		
		/**Constructor - Constructs a wrapper for a <code>DisplayMode</code>.
		 * 
		 * @param mode The <code>DisplayMode</code> to wrap.
		 */
		public DisplayWrapper(DisplayMode mode)
		{
			this.mode = mode;
		}
		
		public boolean equals(Object o)
		{
			if(o instanceof DisplayWrapper)
				return mode.equals(((DisplayWrapper)o).mode);
			return mode.equals(o);
		}
		
		public String toString()
		{
			return (mode.getWidth() + "x" + mode.getHeight() + 
					"x" + mode.getBitDepth()+ " @" + mode.getRefreshRate());
		}
	}
}
