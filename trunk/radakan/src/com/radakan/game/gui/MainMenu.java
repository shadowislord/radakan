package com.radakan.game.gui;

import com.gibbon.jme.context.JmeContext;
import com.radakan.game.Game;
import com.radakan.game.debug.GameDebugManager;
import com.radakan.game.debug.GameSysInfoManager;
import com.radakan.game.res.GameResourceManager;
import java.io.IOException;
import java.net.URL;
import org.fenggui.Button;
import org.fenggui.Container;
import org.fenggui.FengGUI;
import org.fenggui.Label;
import org.fenggui.binding.render.Binding;
import org.fenggui.binding.render.ITexture;
import org.fenggui.binding.render.Pixmap;
import org.fenggui.decorator.background.PixmapBackground;
import org.fenggui.event.ButtonPressedEvent;
import org.fenggui.event.IButtonPressedListener;
import org.fenggui.layout.RowLayout;
import org.fenggui.layout.StaticLayout;
import org.fenggui.util.Dimension;
import org.fenggui.util.Spacing;
import org.lwjgl.opengl.GL11;

public class MainMenu extends UIContext {
    
    private boolean startGameInvoked = false;

    public void setBilinearFilter(ITexture tex){
        tex.bind();
        GL11.glTexParameteri(GL11.GL_TEXTURE_2D, GL11.GL_TEXTURE_MIN_FILTER, GL11.GL_LINEAR);
        GL11.glTexParameteri(GL11.GL_TEXTURE_2D, GL11.GL_TEXTURE_MAG_FILTER, GL11.GL_LINEAR);
    }
    
    public Container buildButtons(){
        final GameGUIManager gui = Game.getGUIManager();
        
        Container c = new Container(new RowLayout(false));
        float width = gui.width;
        float height = gui.height;
        
        Dimension btnSize = new Dimension((int)(width / 5.2f), (int)(height / 15f));
        
        Button newGame = FengGUI.createButton("New Game");
        //newGame.getAppearance().setFont(Fonts.ARKHAM_BUTTONS);
        newGame.setSize(btnSize);
        newGame.setExpandable(false);
        newGame.setShrinkable(false);
        newGame.getAppearance().setMargin(new Spacing(5, 10));
        c.addWidget(newGame);
        newGame.addButtonPressedListener(new IButtonPressedListener() {
            public void buttonPressed(ButtonPressedEvent e) {
                if (startGameInvoked)
                    return;
                
                startGameInvoked = true;
                LoadScreen loading = new LoadScreen(new GameLoadTask(), true);
                gui.setContext(loading, true);
            }
        });
        
        Button contGame = FengGUI.createButton("Continue Game");
        //contGame.getAppearance().setFont(Fonts.ARKHAM_BUTTONS);
        contGame.setSize(btnSize);
        contGame.setExpandable(false);
        contGame.setShrinkable(false);
        contGame.getAppearance().setMargin(new Spacing(5, 10));
        c.addWidget(contGame);
        
        Button credits = FengGUI.createButton("Credits");
        //credits.getAppearance().setFont(Fonts.ARKHAM_BUTTONS);
        credits.setSize(btnSize);
        credits.setExpandable(false);
        credits.setShrinkable(false);
        credits.getAppearance().setMargin(new Spacing(5, 10));
        c.addWidget(credits);
        
        Button exit = FengGUI.createButton("Exit");
        //exit.getAppearance().setFont(Fonts.ARKHAM_BUTTONS);
        exit.setSize(btnSize);
        exit.setExpandable(false);
        exit.setShrinkable(false);
        exit.getAppearance().setMargin(new Spacing(5, 10));
        c.addWidget(exit);
        exit.addButtonPressedListener(new IButtonPressedListener() {
                public void buttonPressed(ButtonPressedEvent arg0) {
                    DoExitScreen exit = new DoExitScreen();
                    gui.setContext(exit, true);
                }
        });
        
        //c.layout();
        c.pack();
        
        return c;
    }
    
    public void buildGUI(){
        GameGUIManager gui = Game.getGUIManager();
        GameSysInfoManager sysInfo = Game.getSysInfoManager();
        GameResourceManager resource = Game.getResourceManager();
        
        try{
            setLayoutManager(new StaticLayout());
            float width = gui.width;
            float height = gui.height;
            
            //Label logo = FengGUI.createLabel();
            Label version = FengGUI.createLabel();
            version.setText(sysInfo.getValue("game.fullname"));
            
            URL backgroundURL = resource.getResource("menu_bg.png");
            
            // Configure Background
            Pixmap bgImage = new Pixmap(Binding.getInstance().getTexture(backgroundURL));
            setBilinearFilter(bgImage.getTexture());
            PixmapBackground bg = new PixmapBackground(bgImage);
            bg.setScaled(true);
            getAppearance().add(bg);
            
//            // Configure Logo
//            Pixmap logoImage = new Pixmap(Binding.getInstance().getTexture("data/images/logo.png"));
//            setBilinearFilter(logoImage.getTexture());
//            PixmapBackground bgLogo = new PixmapBackground(logoImage);
//            bgLogo.setScaled(true);
//            logo.getAppearance().add(bgLogo);
//            
//            logo.setExpandable(false);
//            logo.setShrinkable(false);
//            
//            
//            int logoWidth = (int)((width / 4.5f) * 3f);
//            int logoHeight = (int)(height / 4.5f);
//            
//            logo.setSize(logoWidth, logoHeight);
//            logo.setXY(width - logoWidth + 15, height - logoHeight - 10);
//            addWidget(logo);
            
            //version.getAppearance().setFont(Fonts.STONEHEDGE_SMALL);
            //version.getAppearance().setTextColor(Color.WHITE);
            version.setSizeToMinSize();
            version.setXY( (int)(width - version.getWidth() - 20), 20);
            addWidget(version);
            
            Container buttons = buildButtons();
            // put in the center-left
            // add some margin from the left
            buttons.setXY( (int)(width / 2f - buttons.getWidth() / 2f), 
                           (int)(height / 2f - buttons.getHeight() / 2f - height / 16f) );
            addWidget(buttons);
            
            setMinSize((int)width, (int)height);
            setSizeToMinSize();
        } catch (IOException ex){
            Game.getDebugManager().reportError("Failed to locate background image", ex);
        }
    }

    @Override
    public void update(float tpf) {
    }
    
}
