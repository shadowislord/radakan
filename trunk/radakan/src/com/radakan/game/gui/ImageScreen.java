package com.radakan.game.gui;

import com.radakan.game.Game;
import com.radakan.game.res.GameResourceManager;
import java.io.IOException;
import java.net.URL;
import java.util.logging.Level;
import java.util.logging.Logger;
import org.fenggui.binding.render.Binding;
import org.fenggui.binding.render.ITexture;
import org.fenggui.binding.render.Pixmap;
import org.fenggui.decorator.background.PixmapBackground;
import org.lwjgl.opengl.GL11;

public class ImageScreen extends UIContext {

    private URL imageURL;
    
    public ImageScreen(String imagePath){
        GameResourceManager res = Game.getResourceManager();
        imageURL = res.getResource(imagePath);
    }
    
    public void setBilinearFilter(ITexture tex){
        tex.bind();
        GL11.glTexParameteri(GL11.GL_TEXTURE_2D, GL11.GL_TEXTURE_MIN_FILTER, GL11.GL_LINEAR);
        GL11.glTexParameteri(GL11.GL_TEXTURE_2D, GL11.GL_TEXTURE_MAG_FILTER, GL11.GL_LINEAR);
    }
    
    @Override
    public void buildGUI() {
        try {
            Pixmap bgImage = new Pixmap(Binding.getInstance().getTexture(imageURL));
            setBilinearFilter(bgImage.getTexture());
            PixmapBackground bg = new PixmapBackground(bgImage);
            bg.setScaled(true);
            getAppearance().add(bg);
        } catch (IOException ex) {
            Logger.getLogger(ImageScreen.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    @Override
    public void update(float tpf) {
    }

}
