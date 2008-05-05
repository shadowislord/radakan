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

package com.gibbon.jme.context;

/**
 * An interface for accessing a display context
 */
public interface JmeDisplay {
    
    /**
     * Returns true if the display is active.
     */
    public boolean isActive();
    
    /**
     * Returns true if the display was requested to close by the user.
     */
    public boolean isClosing();

    /**
     * Returns the display width in pixels.
     */
    public int getWidth();
    
    /**
     * Returns the display height in pixels.
     */
    public int getHeight();
    
    /**
     * Returns the bits per pixel value (color depth) for the display.
     */
    public int getBitsPerPixel();
    
    /**
     * Returns the update frequency of the display
     */
    public int getFrequency();
    
    /**
     * Returns true if the display is in fullscreen mode
     */
    public boolean isFullscreen();
    
    /**
     * Return true if vertical synchronization is enabled for the display.
     */
    public boolean isVsync();
    
    /**
     * Sets the display position on the screen if its a windowed display
     */
    public void setPosition(final int x, final int y);
}
