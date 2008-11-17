package com.radakan.game.gui;

import java.io.IOException;
import java.net.URL;

import org.fenggui.FengGUI;
import org.fenggui.Label;
import org.fenggui.binding.render.Binding;
import org.fenggui.binding.render.ITexture;
import org.fenggui.binding.render.Pixmap;
import org.fenggui.decorator.background.PixmapBackground;
import org.fenggui.util.Color;
import org.lwjgl.opengl.GL11;

import com.radakan.game.Game;

/**
 * The InGameScreen manages all in-game gui components.
 * E.g health bar, mini map, character face picture, menu bar, and other
 * in-game gui widgets.
 * 
 * @author Kirill Vainer
 */
public class InGameScreen extends UIContext {

	private Label sideView;
	private PixmapBackground sideViewBg;
	
	public void setBilinearFilter(ITexture tex){
        tex.bind();
        GL11.glTexParameteri(GL11.GL_TEXTURE_2D, GL11.GL_TEXTURE_MIN_FILTER, GL11.GL_LINEAR);
        GL11.glTexParameteri(GL11.GL_TEXTURE_2D, GL11.GL_TEXTURE_MAG_FILTER, GL11.GL_LINEAR);
    }
	
    /**
     * Does nothing. For now..
     */
    @Override
    public void buildGUI() {
//    	URL pressStartURL = Game.getResourceManager().getResource("stupid.gif");
//    	
//    	try {
//	    	sideView = FengGUI.createLabel();
//	    	sideView.setText("Hello Thar!");
//	    	
//	    	ITexture tex = Binding.getInstance().getTexture(pressStartURL);
//	        Pixmap logoImage = new Pixmap(tex);
//		
//	        setBilinearFilter(logoImage.getTexture());
//	        sideViewBg = new PixmapBackground(logoImage);
//	        sideViewBg.setScaled(true);
//	        sideView.getAppearance().add(sideViewBg);
//	        setVisible(false);
//	        //getAppearance().add(sideViewBg);
//	        
//	        float width  = Game.getGUIManager().width,
//            	height = Game.getGUIManager().height;
//	        float aspect = width / height;
//		
//		    float logoWidth = (width / 4.2f) * 4f / aspect;
//		    float logoHeight = height / 4.2f;
//		
//		    float posX = width / 2f - logoWidth / 2f;
//		    float posY = height / 2f - logoHeight / 2f - height / 4f;
//	        
//		    sideView.setSize((int)logoWidth, (int)logoHeight);
//		    sideView.setXY((int)posX, (int)posY);
//	        addWidget(sideView);
//		} catch (IOException ex) {
//			Game.getDebugManager().reportError("Error while creating In-Game GUI", ex);
//		}
    }

    @Override
    public void update(float tpf) {
    }

}
