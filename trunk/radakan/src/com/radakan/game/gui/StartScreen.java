package com.radakan.game.gui;

import com.gibbon.jme.context.JmeContext;
import com.jme.input.KeyInput;
import com.jme.input.KeyInputListener;
import com.jme.math.Vector3f;
import com.jme.renderer.Camera;
import com.radakan.game.Game;
import com.radakan.game.debug.GameDebugManager;
import java.io.IOException;
import java.net.URL;
import org.fenggui.Container;
import org.fenggui.FengGUI;
import org.fenggui.Label;
import org.fenggui.binding.render.Binding;
import org.fenggui.binding.render.ITexture;
import org.fenggui.binding.render.Pixmap;
import org.fenggui.decorator.background.PixmapBackground;
import org.fenggui.util.Color;
import org.lwjgl.opengl.GL11;

public class StartScreen extends UIContext {

    private float time = 0f;
    private int animStage = 0;
    
    private Container logo;
    
    private PixmapBackground logoBg;
    private Label startLabel;
    private PixmapBackground labelBg;

    private static final float TITLE_FADE_TIME = 2f;
    private static final float TITLE_SHINE_TIME = 2f;
    
    private StartScreenPass ssPass;
    
    public StartScreen(){
        ssPass = new StartScreenPass();
        JmeContext.get().getPassManager().add(ssPass);
    }
    
    @Override
    public void contextRemoved(){
        JmeContext.get().getPassManager().remove(ssPass);
        
        // make sure to reset camera vectors, because StartScreenPass modifies them
        Camera cam = JmeContext.get().getRenderer().getCamera();
        cam.setAxes(Vector3f.UNIT_X, Vector3f.UNIT_Y, Vector3f.UNIT_Z);
    }
    
    public void setupPressStartText() throws IOException{
        URL pressStartURL = Game.getResourceManager().getResource("press_start.png");
        
        startLabel = FengGUI.createLabel();
        Pixmap logoImage = new Pixmap(Binding.getInstance().getTexture(pressStartURL));
        setBilinearFilter(logoImage.getTexture());
        labelBg = new PixmapBackground(logoImage);
        labelBg.setScaled(true);
        labelBg.setBlendingColor(new Color(1f, 1f, 1f, 0f));
        startLabel.getAppearance().add(labelBg);

        //pressStart.setExpandable(false);
        //pressStart.setShrinkable(false);

        float width  = Game.getGUIManager().width,
              height = Game.getGUIManager().height;
        float aspect = width / height;

        float logoWidth = (width / 4.2f) * 4f / aspect;
        float logoHeight = height / 4.2f;

        float posX = width / 2f - logoWidth / 2f;
        float posY = height / 2f - logoHeight / 2f - height / 4f;

        startLabel.setSize((int)logoWidth, (int)logoHeight);
        startLabel.setXY((int)posX, (int)posY);
        addWidget(startLabel);
    }
    
    public void buildGUI() {
        try {
            /* logo setup */ {
                // get the image and set bilinear filtering
                URL titleURL = Game.getResourceManager().getResource("radakan_title_big.png");
                Pixmap bgImage = new Pixmap(Binding.getInstance().getTexture(titleURL));
                setBilinearFilter(bgImage.getTexture());

                // create the background
                logoBg = new PixmapBackground(bgImage);
                logoBg.setScaled(true);

                logo = new Container();
                int logoHeight = (int) (Game.getGUIManager().height / 4.5f);
                int logoWidth = (int) (logoHeight * 4.962837f);

                logo.setSize(logoWidth, logoHeight);
                logo.setXY(Game.getGUIManager().width / 2 - logoWidth / 2, Game.getGUIManager().height / 2 + logoHeight / 2);
                logo.getAppearance().add(logoBg);
                addWidget(logo);
            }
            
            try{
                setupPressStartText();
            } catch (IOException ex){
                Game.getDebugManager().reportError("IO Error while creating Press Start text", ex);
            }
            
            
            // create the copyright label
            Label copyright = FengGUI.createLabel("(C) Gibbon Entertainment Inc. 2008");
            //copyright.getAppearance().setFont(Fonts.STONEHEDGE_SMALL);
            //copyright.getAppearance().setTextColor(Color.WHITE_HALF_OPAQUE);
            copyright.setSizeToMinSize();
            copyright.setXY(20,
                            copyright.getHeight() / 2);
            addWidget(copyright);
            
            // create the version label
            Label version = new Label();
            //version.getAppearance().setFont(Fonts.STONEHEDGE_SMALL);
            version.setText(Game.getVersionPrefix() + " " + 
            			    Game.getGameVersion());
            //version.getAppearance().setTextColor(Color.WHITE_HALF_OPAQUE);
            version.setSizeToMinSize();
            version.setXY(Game.getGUIManager().width - version.getWidth() - 20,
                          version.getHeight() / 2);
            addWidget(version);
        } catch (IOException ex) {
            Game.getDebugManager().reportError("Error while loading GUI", ex);
        }
        
    }

    
    public void setBilinearFilter(ITexture tex){
        tex.bind();
        GL11.glTexParameteri(GL11.GL_TEXTURE_2D, GL11.GL_TEXTURE_MIN_FILTER, GL11.GL_LINEAR);
        GL11.glTexParameteri(GL11.GL_TEXTURE_2D, GL11.GL_TEXTURE_MAG_FILTER, GL11.GL_LINEAR);
    }
    
    @Override
    public void update(float tpf) {
        time += tpf;
        
        if (animStage == 2)
            return;
        
        if (KeyInput.get().isKeyDown(KeyInput.KEY_RETURN)){
            MainMenu menu = new MainMenu();
            Game.getGUIManager().setContext(menu, true);
            animStage = 2;
        }
        
        if (time < TITLE_FADE_TIME){
            animStage = 0;
        }else if (time >= TITLE_FADE_TIME && animStage == 0){
            animStage = 1;
            logoBg.setBlendingColor(Color.WHITE);
        }
        
        float localTime;
        if (animStage == 0){
            localTime = time / TITLE_FADE_TIME;
            logoBg.setBlendingColor(new Color(1f, 1f, 1f, localTime));
        }else if (animStage == 1){
            localTime = (time - TITLE_FADE_TIME);
            if (((int)localTime) % 2 == 1){
                localTime = ((time - TITLE_FADE_TIME) % TITLE_SHINE_TIME) / TITLE_SHINE_TIME;
                labelBg.setBlendingColor(new Color(1f, 1f, 1f, 1f - localTime));
            }else{
                localTime = ((time - TITLE_FADE_TIME) % TITLE_SHINE_TIME) / TITLE_SHINE_TIME;
                labelBg.setBlendingColor(new Color(1f, 1f, 1f, localTime));
            }
        }
    }

}
