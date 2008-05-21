package com.gibbon.radakan.ui;

import java.io.IOException;

import org.fenggui.Container;
import org.fenggui.Label;
import org.fenggui.background.PixmapBackground;
import org.fenggui.layout.StaticLayout;
import org.fenggui.render.Binding;
import org.fenggui.render.ITexture;
import org.fenggui.render.Pixmap;
import org.lwjgl.opengl.GL11;

import com.gibbon.radakan.SysInfo;
import com.gibbon.radakan.error.ErrorReporter;

public class MainMenu2 extends UIContext {
	
	public void buildGUI(){
		try{
            Label version = new Label();
            version.setText(SysInfo.getVersionPrefix() + " " +
                            SysInfo.getGameVersion());
			
	        Pixmap bgImage = new Pixmap(Binding.getInstance().getTexture("data/images/background2.png"));
	        setBilinearFilter(bgImage.getTexture());
	        PixmapBackground bg = new PixmapBackground(bgImage);
	        bg.setScaled(true);
	        getAppearance().add(bg);   
		}catch(IOException e){
			ErrorReporter.reportError("Failed building Main Menu", e);
		}
	}
	
    public void setBilinearFilter(ITexture tex){
        tex.bind();
        GL11.glTexParameteri(GL11.GL_TEXTURE_2D, GL11.GL_TEXTURE_MIN_FILTER, GL11.GL_LINEAR);
        GL11.glTexParameteri(GL11.GL_TEXTURE_2D, GL11.GL_TEXTURE_MAG_FILTER, GL11.GL_LINEAR);
    }
}
