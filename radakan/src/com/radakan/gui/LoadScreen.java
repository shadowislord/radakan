package com.radakan.gui;

import com.jme.image.Texture;
import com.jme.image.Texture.Type;
import com.jme.input.KeyInput;
import com.jme.input.KeyInputListener;
import com.jme.scene.state.lwjgl.LWJGLTextureState;
import com.radakan.res.ResourceManager;
import com.radakan.util.ErrorHandler;
import java.io.IOException;
import java.net.URL;
import java.util.concurrent.Callable;
import java.util.concurrent.Future;
import org.fenggui.FengGUI;
import org.fenggui.Label;
import org.fenggui.ProgressBar;
import org.fenggui.binding.render.Binding;
import org.fenggui.binding.render.ITexture;
import org.fenggui.binding.render.Pixmap;
import org.fenggui.decorator.background.PixmapBackground;
import org.fenggui.layout.StaticLayout;
import org.lwjgl.opengl.GL11;

public class LoadScreen extends UIContext {

    private LoadingTask task;
    private Future<Object> taskMonitor;
    
    private ProgressBar bar;
    
    private LoadScreenState state = LoadScreenState.PREINIT;
    
    private enum LoadScreenState {
        PREINIT, 
        LOADING,
        DONE,
        ERROR,
        FADE_OUT,
    }
    
    public LoadScreen(LoadingTask task){
        this.task = task;
    }
    
    
    public void setBilinearFilter(ITexture tex){
        tex.bind();
        GL11.glTexParameteri(GL11.GL_TEXTURE_2D, GL11.GL_TEXTURE_MIN_FILTER, GL11.GL_LINEAR);
        GL11.glTexParameteri(GL11.GL_TEXTURE_2D, GL11.GL_TEXTURE_MAG_FILTER, GL11.GL_LINEAR);
    }

    @Override
    public void contextAttach(){
        taskMonitor = ResourceManager.runTaskLater("LoadScreenTask", new Callable<Object>(){
            public Object call(){
                task.doInBackground();
                return null;
            }
        });
        
        state = LoadScreenState.LOADING;
    }
    
    @Override
    public void buildGUI() {
        assert state == LoadScreenState.PREINIT;
        
        try{
            setLayoutManager(new StaticLayout());
            
            float width = UIManager.width;
            float height = UIManager.height;
            
            //
            //Label version = new Label();
            //version.setText(SysInfo.getVersionPrefix() + " " +
            //                SysInfo.getGameVersion());
            
            URL backgroundURL = LoadScreen.class.getResource("/com/radakan/data/textures/logo.png");
            URL nowLoadingURL = LoadScreen.class.getResource("/com/radakan/data/images/loadingtext.png");
            
            // Configure Background
            Pixmap bgImage = new Pixmap(Binding.getInstance().getTexture(backgroundURL));
            setBilinearFilter(bgImage.getTexture());
            PixmapBackground bg = new PixmapBackground(bgImage);
            bg.setScaled(true);
            getAppearance().add(bg);
            
            // Configure "Now Loading" text
            Label logo = FengGUI.createLabel();
            Pixmap logoImage = new Pixmap(Binding.getInstance().getTexture(nowLoadingURL));
            setBilinearFilter(logoImage.getTexture());
            PixmapBackground bgLogo = new PixmapBackground(logoImage);
            bgLogo.setScaled(true);
            logo.getAppearance().add(bgLogo);
            
            logo.setExpandable(false);
            logo.setShrinkable(false);

            float aspect = width / height;
            
            float logoWidth = (width / 2.0f);
            float logoHeight = height / 2.0f / 2f * aspect;
            
            float posX = width / 2f - logoWidth / 2f;
            float posY = height / 2f - logoHeight / 2f;
            
            logo.setSize((int)logoWidth, (int)logoHeight);
            logo.setXY((int)posX, (int)posY);
            addWidget(logo);
//            
//            version.getAppearance().setFont(Fonts.STONEHEDGE_SMALL);
//            version.getAppearance().setTextColor(Color.WHITE);
//            version.setSizeToMinSize();
//            version.setXY(width - version.getWidth() - 20, 20);
//            addWidget(version);
//            
//            Container buttons = buildButtons();
//            // put in the center-left
//            // add some margin from the left
//            buttons.setXY(40, height / 2 - buttons.getHeight() / 2);
//            addWidget(buttons);
            
            bar = FengGUI.createProgressBar(this);
            bar.setSize((int)(width / 2f), (int)(height / 18));
            bar.setXY((int)(width / 2) - bar.getWidth() / 2, 
                     logo.getY() /*- bar.getHeight() / 18*/);
            bar.setText(" ");
            bar.setValue(0);
            addWidget(bar);
            
            setMinSize((int)width, (int)height);
            setSizeToMinSize();
        } catch (IOException ex){
            ErrorHandler.reportError("Failed to locate background image", ex);
        }
    }

    @Override
    public void update(float tpf) {
        if (state == LoadScreenState.LOADING){
            if (taskMonitor.isDone()) {
                if (taskMonitor.isCancelled()) {
                    bar.setText("ERROR!");
                    state = LoadScreenState.ERROR;
                } else {
                    bar.setValue(1.0);
                    //bar.setText("Press any key to continue.");
                    bar.setText("DONE!");
                    
                    task.done();
                    state = LoadScreenState.FADE_OUT;
//                    KeyInput.get().addListener(new KeyInputListener(){
//                        public void onKey(char key, int keyId, boolean pressed) {
//                            if (pressed){
//                                //bar.setText("");
//                                state = LoadScreenState.FADE_OUT;
//                                UIManager.setContext(new DoExitScreen(), true);
//                                KeyInput.get().removeListener(this);
//                            }
//                        }
//                    });
                }
            } else {
                bar.setValue(task.getProgress() / 100.0);
                bar.setText(task.getText());
            }
        }
        
    }

}
