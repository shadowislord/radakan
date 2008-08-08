package com.gibbon.tools.world;

import java.awt.Component;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.prefs.Preferences;
import javax.swing.ButtonGroup;
import javax.swing.JMenu;
import javax.swing.JRadioButtonMenuItem;
import javax.swing.SwingUtilities;
import javax.swing.UIManager;

/**
 *
 * @author Tomás Lázaro
 */
public class LafMenu extends JMenu{

	private LafMenu(String name, boolean b){
		super(name, b);
	}
	
	public static LafMenu setupLafMenu(final Component caller, final String propNodeName){
		final String nodeName;
		if(propNodeName != null){
			nodeName = propNodeName;
		} else {
			nodeName = caller.getClass().getName();
		}
		
		String currentLafName;
		if ((currentLafName = loadLookAndFeel(nodeName)) != null) {
			try {
				UIManager.setLookAndFeel(currentLafName);
			} catch (Exception e) {
				System.out.println(e);
			}
			SwingUtilities.updateComponentTreeUI(caller);
		}
		
		LafMenu lnf = new LafMenu("Look & Feel", true);
		ButtonGroup buttonGroup = new ButtonGroup();
		final UIManager.LookAndFeelInfo[] looks = UIManager.getInstalledLookAndFeels();
		for (int i = 0; i < looks.length; i++) {
			JRadioButtonMenuItem item = new JRadioButtonMenuItem(looks[i].getName(), i == 0);
			final String lafName = looks[i].getClassName();
			item.addActionListener(new ActionListener() {

				@Override
				public void actionPerformed(ActionEvent ae) {
					try {
						UIManager.setLookAndFeel(lafName);
						saveLookAndFeel(nodeName, lafName);
					} catch (Exception e) {
						System.out.println(e);
					}
					SwingUtilities.updateComponentTreeUI(caller);
				}
			});
			buttonGroup.add(item);
			if(currentLafName != null && lafName.equals(currentLafName)){
				buttonGroup.setSelected(item.getModel(), true);
			}
			lnf.add(item);
		}
		
		return lnf;
	}
	
	private static void saveLookAndFeel(String node, String name) {
		Preferences prefs = Preferences.userRoot().node(node);
		prefs.put("laf", name);
	}
	
	private static String loadLookAndFeel(String node){
		return Preferences.userRoot().node(node).get("laf", "unknown");
	}
}
