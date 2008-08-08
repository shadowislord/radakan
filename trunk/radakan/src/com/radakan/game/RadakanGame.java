/* Radakan is free software: you can redistribute it and/or modify
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
package com.radakan.game;

import java.util.ArrayList;
import java.util.prefs.Preferences;

import org.apache.log4j.Logger;

import com.jme.scene.Node;
import com.jme.system.DisplaySystem;
import com.jme.system.GameSettings;
import com.radakan.game.util.LoadScreen;
import com.radakan.jme.app.Basic3DGame;

/**
 * @author Joshua Montgomery
 * @version 1.0.0
 * @created Jul 26, 2008
 */
public class RadakanGame extends Basic3DGame
{
	private static Logger logger = Logger.getLogger(RadakanGame.class);
			
	public RadakanGame(GameSettings settings)
	{
		super(settings);		
	}	
	
	protected void initGame()
	{
		logger.debug("Initializing the game");
		
		LoadScreen loadScreen = new LoadScreen();
		
		logger.debug("Game initialization finished");		
	}

	
	protected void initSystem()
	{
		logger.debug("Initializing the system...");
		
		display.createWindow(settings.getWidth(),
							 settings.getHeight(),
							 settings.getDepthBits(), 
							 settings.getFrequency(),
							 settings.isFullscreen());
		
		renderer = display.getRenderer();		
		camera = display.getRenderer().createCamera(display.getWidth(),display.getHeight());
		renderer.setCamera(camera);
		rootNode = new Node("Root Node");
		
		logger.debug("System initialization finished!");
	}
	
    @Override
    protected void getAttributes() {
        if (settings == null) {
            throw new NullPointerException("The game settings are not set!");
        }

        display = DisplaySystem.getDisplaySystem();
        display.setTitle("Radakan");
        display.setWidth(settings.getWidth());
        display.setHeight(settings.getHeight());
        display.setVSyncEnabled(settings.isVerticalSync());
    }
	
	@Override
	protected void quit()
	{
		// TODO Auto-generated method stub
		
	}
	
	@Override
	protected void cleanup()
	{
		// TODO Auto-generated method stub
		
	}

	@Override
	protected void reinit()
	{
		// TODO Auto-generated method stub
		
	}

	protected void render(float arg0)
	{	
		renderer.draw(rootNode);
	}

	@Override
	protected void update(float arg0)
	{
		
		
	}

}
