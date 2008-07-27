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
package com.radakan.jme.app;

import org.apache.log4j.Logger;

import com.jme.app.AbstractGame;

/**Provides functionality for a basic 3D game.
 * 
 * @author Joshua Montgomery
 * @version 1.0.0
 * @created Jul 27, 2008
 */
public abstract class Basic3DGame extends AbstractGame
{
	private Logger logger = Logger.getLogger(Basic3DGame.class);
	
	public void start()
	{
		logger.debug("Starting main game loop...");
		
		getAttributes();
		
		if(!finished)
		{
			//initialize necessary game components
			initSystem();
			assertDisplayCreated();
			initGame();
			
			//Main Loop
			while(!finished)
			{
			
				Thread.yield();
			}
		}
	}
}
