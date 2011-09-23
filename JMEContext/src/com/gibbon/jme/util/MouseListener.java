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

import com.jme.input.MouseInputListener;
import org.fenggui.Display;
import org.fenggui.event.mouse.MouseButton;

public class MouseListener implements MouseInputListener {

        private boolean down;
        private int lastButton;
        private int lastX = -100;
        private int lastY = -100;
        private Display disp;
        private boolean mouseHandled;
        
        public MouseListener(Display disp){
            this.disp = disp;
        }

        public void onButton(int button, boolean pressed, int x, int y) {
            down = pressed;
            lastButton = button;
            if (pressed) {
                lastX = x;
                lastY = y;
                mouseHandled = disp.fireMousePressedEvent(x, y, getMouseButton(button), 1);
                
                System.out.println(lastX+" "+lastY+": "+mouseHandled);
            } else {
                mouseHandled = disp.fireMouseReleasedEvent(x, y, getMouseButton(button), 1);

                if (x == lastX && y == lastY && getMouseButton(button) == MouseButton.LEFT) {
                    boolean rtnVal = disp.fireMouseClickEvent(x, y, getMouseButton(button), 1);

                    if (mouseHandled == false) {
                        mouseHandled = rtnVal;
                    }
                }

                lastX = -100;
                lastY = -100;
            }
        }

        public void onMove(int xDelta, int yDelta, int newX, int newY) {
            // If the button is down, the mouse is being dragged
            if (down) {
                mouseHandled = disp.fireMouseDraggedEvent(newX, newY, getMouseButton(lastButton), 0);
            } else {
                mouseHandled = disp.fireMouseMovedEvent(newX, newY, null, 0);
            }
        }

        public void onWheel(int wheelDelta, int x, int y) {
            // wheelDelta is positive if the mouse wheel rolls up, negative otherwise
            // we need to flip the delta, because FengGUI expects a positive wheelDelta
            // certain widgets will only scroll in one direction otherwise.
            if (wheelDelta > 0) {
                mouseHandled = disp.fireMouseWheel(x, y, true, wheelDelta, 1);
            } else {
                mouseHandled = disp.fireMouseWheel(x, y, false, -wheelDelta, 1);
            }
        }

        /**
         * Helper method that maps the mouse button to the equivalent
         * FengGUI MouseButton enumeration.
         * @param button The button pressed or released.
         * @return The FengGUI MouseButton enumeration matching the
         * button.
         */
        private MouseButton getMouseButton(int button) {
            switch (button) {
                case 0:
                    return MouseButton.LEFT;
                case 1:
                    return MouseButton.RIGHT;
                case 2:
                    return MouseButton.MIDDLE;
                default:
                    return MouseButton.LEFT;
            }
        }
    }