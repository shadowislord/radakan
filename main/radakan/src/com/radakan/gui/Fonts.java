package com.radakan.gui;

import com.radakan.util.ErrorHandler;
import java.io.FileNotFoundException;
import java.io.IOException;
import org.fenggui.render.Font;

public final class Fonts {

    public static Font STONEHEDGE_SMALL;
    public static Font ARKHAM_BUTTONS;
    
    static {
        try {
            STONEHEDGE_SMALL = new Font("data/themes/small.png", "data/themes/small.font");
            ARKHAM_BUTTONS = new Font("data/themes/bigbutton.png", "data/themes/bigbutton.font");
        } catch (FileNotFoundException ex) {
            ErrorHandler.reportError("Font file cannot be found", ex);
        } catch (IOException ex) {
            ErrorHandler.reportError("Error while reading font file", ex);
        }
    }
    
}
