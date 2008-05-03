/*
 * Radakan RPG is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Radakan RPG is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Radakan RPG.  If not, see <http://www.gnu.org/licenses/>.
 */

package com.gibbon.radakan;

import com.jmex.audio.AudioSystem;
import org.lwjgl.openal.AL10;
import org.lwjgl.opengl.ContextCapabilities;
import org.lwjgl.opengl.GL11;
import org.lwjgl.opengl.GL20;
import org.lwjgl.opengl.GLContext;

public class Radakan {

    public static ContextCapabilities caps;
    
    public static String vendor;
    public static String renderer;
    public static String apiVer;
    public static String glslVer;
    
    public static String alVendor;
    public static String alRenderer;
    public static String alVersion;
    
    public static void querySystemInfo(){
        caps = GLContext.getCapabilities();
        
        if (AudioSystem.isCreated()){
            alVersion = AL10.alGetString(AL10.AL_VERSION);
            alRenderer = AL10.alGetString(AL10.AL_RENDERER);
            alVendor = AL10.alGetString(AL10.AL_VENDOR);
        }
        
        glslVer = GL11.glGetString(GL20.GL_SHADING_LANGUAGE_VERSION);
        apiVer = GL11.glGetString(GL11.GL_VERSION);
        renderer = GL11.glGetString(GL11.GL_RENDERER);
        vendor = GL11.glGetString(GL11.GL_VENDOR);
    }
    
    public static String getGameVersion(){
        return "0.15";
    }
    
    public static String getVersionPrefix(){
        return "Prototype";
    }
    
    public static String getGameName(){
        return "Radakan";
    }
    
}
