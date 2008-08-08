/*
 * Radakan is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Radakan is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Radakan.  If not, see <http://www.gnu.org/licenses/>.
 */

package com.gibbon.jme.util;

import com.jme.image.Image;
import java.nio.ByteBuffer;
import java.nio.IntBuffer;

/**
 * Offscreen drawing surface.
 * 
 * @author Momoko_Fan
 */
public interface IJmePixelBuffer {
 
    /**
     * Returns true if the pbuffer is active.
     */
    public boolean isActive();

    /**
     * Returns the pbuffer width in pixels.
     */
    public int getWidth();
    
    /**
     * Returns the pbuffer height in pixels.
     */
    public int getHeight();
    
    /**
     * Returns the bits per pixel value (color depth) for the pbuffer.
     */
    public int getBitsPerPixel();
    
    /**
     * Binds the pbuffer.<br/>
     * This allows you to send draw calls though the pbuffer's renderer class.
     */
    public void enterContext();
    
    /**
     * Unbind the pbuffer.<br/>
     * Draw calls will go to the previously bound context.
     */
    public void exitContext();
    
     /**
     * Read pixels from the screen to a ByteBuffer. <br/>
     * The ByteBuffer contains interlaced RGB 8 bit color components.
     */
    public ByteBuffer getPixels(ByteBuffer store, int x, int y, int w, int h);
    
    /**
     * Read pixels from the drawing surface to an IntBuffer. <br/>
     * Each integer in the IntBuffer contains RGBA 8 bit color components.
     */
    public IntBuffer getPixels(IntBuffer store, int x, int y, int w, int h);
    
    /**
     * Read pixels from the drawing surface to an image.
     * 
     * @param store The image to which framebuffer data should be stored.
     * @param x X offset.
     * @param y Y offset.
     */
    public void getPixels(Image store, int x, int y);
    
    /**
     * Read pixels from the screen to an image.
     * 
     * @param x X offset.
     * @param y Y offset
     * @param w Width of the returned image.
     * @param h Height of the returned image.
     * @param imageType The image type of the returned image, see constants in the jME Image class.
     * @return A jME image object containing the framebuffer data.
     */
    public Image getPixels(int x, int y, int w, int h, int imageType);
}
