/*
 * Copyright (c) 2007, MFKARPG
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     - Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     - Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     - Neither the name of the Gibbon Entertainment nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY 'Gibbon Entertainment' "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL 'Gibbon Entertainment' BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
