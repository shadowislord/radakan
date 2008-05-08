/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package com.gibbon.test;

import com.gibbon.jme.context.JmeContext;
import org.fenggui.Display;
import org.fenggui.event.IMenuItemPressedListener;
import org.fenggui.event.MenuItemPressedEvent;
import org.fenggui.menu.Menu;
import org.fenggui.menu.MenuBar;
import org.fenggui.menu.MenuItem;

/**
 *
 * @author Kirill
 */
public class EditorUI {

    public static void create(Display display){
        Menu menuFile = new Menu();
                MenuItem itemFileExit = new MenuItem("Exit");
                itemFileExit.addMenuItemPressedListener(
                        new IMenuItemPressedListener(){
                            public void menuItemPressed(MenuItemPressedEvent event){
                                JmeContext.get().dispose();
                                //System.exit(0);
                            }
                        }
                );
                        
                
                menuFile.addItem(itemFileExit);
                
                MenuBar menuBar = new MenuBar();
                menuBar.registerSubMenu(menuFile, "File");
                
                display.addWidget(menuBar);
                
                menuBar.updateMinSize(); // we have not layouted anything yet...
		menuBar.setX(0);
		menuBar.setY(display.getHeight() - menuBar.getMinHeight());
		menuBar.setSize(display.getWidth(), menuBar.getMinHeight());
		menuBar.setShrinkable(false);
    }
    
}
