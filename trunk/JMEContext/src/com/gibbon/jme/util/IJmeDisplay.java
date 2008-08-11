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

/**
 * An interface for accessing a display context
 */
public interface IJmeDisplay {
    
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
