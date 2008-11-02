package com.radakan.game.console;

import com.jme.renderer.ColorRGBA;
import com.jme.scene.Spatial.CullHint;
import com.jme.scene.Text;

/**
 * LogText enables fading and interaction with JmeConsole
 */
public class LogText extends Text {

    /**
     * Time in milliseconds before the text dissaprears.
     */
    private int time = 0;
    /**
     * The parent console of the LogText
     */
    private final JmeConsole parent;

    /**
     * Create a new LogText
     * @param parent The parent JmeConsole who owns this LogText
     * @param name Name of the text node (Jme specific, can be anything)
     * @param text The text to be displayed by the LogText
     */
    public LogText(JmeConsole parent, String name, String text) {
        super(name, text);
        this.parent = parent;
    }

    /**
     * @return True if the text is visible (not faded yet)
     */
    public boolean isActive() {
        return time > 0;
    }

    /**
     * @return True if the text is currently in it's fading animation
     */
    public boolean isFading() {
        return time < 1000 && time > 0;
    }

    /**
     * Update the LogText. The frame time is deducted and fading is applied (if neccessary).
     * @param tpf Time in seconds since the last frame (Time Per Frame)
     */
    public void update(float tpf) {
        time -= (int) (tpf * 1000);
        if (time <= 0) {
            // Died
            setCullHint(CullHint.Always);
        }

        // apply fading
        if (time < 1000) {
            if (time < 0) {
                time = 0;
            }
            ColorRGBA c = getTextColor();
            c.a = ((float) time) / 1000f;
            updateGeometricState(0, true);
        }
    }

    /**
     * Changes the color and/or the text displayed by the LogText
     */
    public void setText(ColorRGBA color, String str) {
        if (color != null) {
            getTextColor().set(color.r, color.g, color.b, getTextColor().a);
        //System.out.println("color: " + getTextColor());
        }
        print(str);
        updateGeometricState(0, true);
    }

    /**
     * Sets expiration time. 
     */
    public void setTime(int time) {
        this.time = time;
    }

    /**
     * Reset the LogText to be fully visible again.
     */
    public void reset() {
        setCullHint(CullHint.Never);
        getTextColor().a = 1.0f;
    }
}