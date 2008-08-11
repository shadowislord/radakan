package com.radakan.gui;

import com.radakan.util.ErrorHandler;
import com.radakan.util.SysInfo;
import java.io.FileNotFoundException;
import java.io.IOException;
import org.fenggui.Button;
import org.fenggui.Container;
import org.fenggui.Label;
import org.fenggui.background.PixmapBackground;
import org.fenggui.layout.RowLayout;
import org.fenggui.layout.StaticLayout;
import org.fenggui.render.Binding;
import org.fenggui.render.Font;
import org.fenggui.render.ITexture;
import org.fenggui.render.Pixmap;
import org.fenggui.util.Color;
import org.fenggui.util.Spacing;
import org.lwjgl.opengl.GL11;

public class MainMenu extends UIContext {

    public void setBilinearFilter(ITexture tex){
        tex.bind();
        GL11.glTexParameteri(GL11.GL_TEXTURE_2D, GL11.GL_TEXTURE_MIN_FILTER, GL11.GL_LINEAR);
        GL11.glTexParameteri(GL11.GL_TEXTURE_2D, GL11.GL_TEXTURE_MAG_FILTER, GL11.GL_LINEAR);
    }
    
    public Container buildButtons(){
        Container c = new Container(new RowLayout(false));
        
        Button newGame = new Button("New Game");
        newGame.getAppearance().setFont(Fonts.ARKHAM_BUTTONS);
        newGame.getAppearance().setMargin(new Spacing(5, 5));
        c.addWidget(newGame);
        
        Button contGame = new Button("Continue Game");
        contGame.getAppearance().setFont(Fonts.ARKHAM_BUTTONS);
        contGame.getAppearance().setMargin(new Spacing(5, 5));
        c.addWidget(contGame);
        
        Button credits = new Button("Credits");
        credits.getAppearance().setFont(Fonts.ARKHAM_BUTTONS);
        credits.getAppearance().setMargin(new Spacing(5, 5));
        c.addWidget(credits);
        
        Button exit = new Button("Exit");
        exit.getAppearance().setFont(Fonts.ARKHAM_BUTTONS);
        exit.getAppearance().setMargin(new Spacing(5, 5));
        c.addWidget(exit);
        
        //c.layout();
        c.pack();
        
        return c;
    }
    
    public void buildGUI(){
        try{
            setLayoutManager(new StaticLayout());
            
            Label logo = new Label();
            Label version = new Label();
            version.setText(SysInfo.getVersionPrefix() + " " +
                            SysInfo.getGameVersion());
            
            // Configure Background
            Pixmap bgImage = new Pixmap(Binding.getInstance().getTexture("data/images/background.png"));
            setBilinearFilter(bgImage.getTexture());
            PixmapBackground bg = new PixmapBackground(bgImage);
            bg.setScaled(true);
            getAppearance().add(bg);
            
            // Configure Logo
            Pixmap logoImage = new Pixmap(Binding.getInstance().getTexture("data/images/logo.png"));
            setBilinearFilter(logoImage.getTexture());
            PixmapBackground bgLogo = new PixmapBackground(logoImage);
            bgLogo.setScaled(true);
            logo.getAppearance().add(bgLogo);
            
            logo.setExpandable(false);
            logo.setShrinkable(false);
            
            int width = UIManager.width;
            int height = UIManager.height;
            
            if (width != 800 || height != 600)
                throw new IOException("ur an idiot: "+width+"x"+height);
            
            int logoWidth = (int)((width / 4.5f) * 3f);
            int logoHeight = (int)(height / 4.5f);
            
            logo.setSize(logoWidth, logoHeight);
            logo.setXY(width - logoWidth + 15, height - logoHeight - 10);
            addWidget(logo);
            
            version.getAppearance().setFont(Fonts.STONEHEDGE_SMALL);
            version.getAppearance().setTextColor(Color.WHITE);
            version.setSizeToMinSize();
            version.setXY(width - version.getWidth() - 20, 20);
            addWidget(version);
            
            Container buttons = buildButtons();
            // put in the center-left
            // add some margin from the left
            buttons.setXY(40, height / 2 - buttons.getHeight() / 2);
            addWidget(buttons);
            
            setMinSize(width, height);
            setSizeToMinSize();
        } catch (IOException ex){
            ErrorHandler.reportError("Failed to locate background image", ex);
        }
    }

    @Override
    public void update(float tpf) {
    }
    
}
