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

import java.util.logging.Logger;

import com.radakan.util.ErrorHandler;

/**
 * @author Joshua Montgomery
 * @version 1.0.0
 * @created Jul 26, 2008
 */
public class Main2
{
	private static Logger logger = Logger.getLogger(Main2.class.getName());
	
	/**The Main game*/
	RadakanGame game = new RadakanGame();
	
	/**Initializes the Radakan game.*/
	private void init()
	{		
		game.initSystem();
		game.initGame();
		shutDown();		
	}
	
	public static void main(String args[])
	{			
		ErrorHandler.bindUncaughtExceptionHandler();
        Game.setDebug(true);	
		
		Main2 main = new Main2();
		main.init();
	}
	
	/**Shuts down the Radakan game.*/
	private void shutDown()
	{
		logger.info("Shutting down Radakan...");
		
		game.cleanUp();
		
		logger.info("Goodbye!");
		System.exit(0);
	}
}
