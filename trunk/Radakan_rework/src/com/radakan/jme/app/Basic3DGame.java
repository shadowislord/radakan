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
package com.radakan.jme.app;

import org.apache.log4j.Logger;

import com.jme.app.AbstractGame;
import com.jme.system.GameSettings;
import com.radakan.game.util.ExceptionHandler;

/**Provides functionality for a basic 3D game.
 * 
 * @author Joshua Montgomery
 * @version 1.0.0
 * @created Jul 27, 2008
 */
public abstract class Basic3DGame extends AbstractGame
{
	private Logger logger = Logger.getLogger(Basic3DGame.class);
	
	/**Handles exceptions that occur within the game.*/
	protected ExceptionHandler exceptionHandler;
	
	/**Constructor - Constructs a basic 3D game.
	 * 
	 * @param settings The settings for the game.
	 */
	public Basic3DGame(GameSettings settings)
	{
		this.settings = settings;
	}
	
	public void start()
	{
		logger.debug("Starting main game loop...");
		
		try
		{			
			getAttributes();
			
			if(!finished)
			{
				//initialize necessary game components
				initSystem();
				assertDisplayCreated();
				initGame();
				
				//Main Loop
				while(!finished && !display.isClosing())
				{
					update(-1.0f);
					render(-1.0f);
					
					display.getRenderer().displayBackBuffer();
					
					Thread.yield();
				}
				
				cleanup();
				quit();
		}
		}catch(Exception e)
		{
			logger.fatal("An exception occured in the main game loop.",e);			
			if(exceptionHandler != null)
				exceptionHandler.handleException(e);
		}
	}
	
	/**Sets the <code>ExceptionHandler</code> for the <code>Basic3DGame</code>.
	 * 
	 * @param handler The <code>ExceptionHandler</code>.
	 */
	public void setExceptionHandler(ExceptionHandler handler)
	{
		exceptionHandler = handler;
	}
}
