package com.radakan.gui;

import com.radakan.util.ErrorHandler;
import com.radakan.util.SysInfo;
import java.io.IOException;
import org.fenggui.Container;
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

    private static final float TITLE_FADE_TIME = 2f;
    private static final float TITLE_SHINE_TIME = 2f;
    
    public StartScreen(){
    }
    
    public void buildGUI() {
        try {
            /* logo setup */ {
                // get the image and set bilinear filtering
                Pixmap bgImage = new Pixmap(Binding.getInstance().getTexture("data/images/radakan_title_big.png"));
                setBilinearFilter(bgImage.getTexture());

                // create the background
                logoBg = new PixmapBackground(bgImage);
                logoBg.setScaled(true);

                logo = new Container();
                int logoHeight = (int) (UIManager.height / 4.5f);
                int logoWidth = (int) (logoHeight * 4.962837f);

                logo.setSize(logoWidth, logoHeight);
                logo.setXY(UIManager.width / 2 - logoWidth / 2, UIManager.height / 2 + logoHeight / 2);
                logo.getAppearance().add(logoBg);
                addWidget(logo);
            }
            
            /* start label setup */ {
                // create the press start label
                startLabel = new Label();
                startLabel.setText("Press Start");
                //startLabel.getAppearance().setFont(Fonts.ARKHAM_BUTTONS);
                //startLabel.getAppearance().setTextColor(Color.OPAQUE);
                startLabel.setSizeToMinSize();
                startLabel.setXY(UIManager.width / 2 - startLabel.getWidth() / 2,
                                 UIManager.height / 5);
                addWidget(startLabel);
            }
            
            // create the copyright label
            Label copyright = new Label();
            //copyright.getAppearance().setFont(Fonts.STONEHEDGE_SMALL);
            copyright.setText("(C) Gibbon Entertainment Inc. 2008");
            //copyright.getAppearance().setTextColor(Color.WHITE_HALF_OPAQUE);
            copyright.setSizeToMinSize();
            copyright.setXY(20,
                            copyright.getHeight() / 2);
            addWidget(copyright);
            
            // create the version label
            Label version = new Label();
            //version.getAppearance().setFont(Fonts.STONEHEDGE_SMALL);
            version.setText(SysInfo.getVersionPrefix() + " " + SysInfo.getGameVersion());
            //version.getAppearance().setTextColor(Color.WHITE_HALF_OPAQUE);
            version.setSizeToMinSize();
            version.setXY(UIManager.width - version.getWidth() - 20,
                          version.getHeight() / 2);
            addWidget(version);
        } catch (IOException ex) {
            ErrorHandler.reportError("Error while loading GUI", ex);
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
                //startLabel.getAppearance().setTextColor(new Color(1f, 1f, 1f, 1f - localTime));
            }else{
                localTime = ((time - TITLE_FADE_TIME) % TITLE_SHINE_TIME) / TITLE_SHINE_TIME;
                //startLabel.getAppearance().setTextColor(new Color(1f, 1f, 1f, localTime));
            }
        }
    }

}
