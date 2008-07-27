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
package com.radakan.game;

import org.apache.log4j.Level;
import org.apache.log4j.Logger;
import org.apache.log4j.PropertyConfigurator;

/**
 * @author Joshua Montgomery
 * @version 1.0.0
 * @created Jul 26, 2008
 */
public class Main
{
	private static Logger logger = Logger.getLogger(Main.class);
	
	/**Initializes the Radakan game.*/
	private void init()
	{
		//TODO: Initialize game
		shutDown();
	}
	
	public static void main(String args[])
	{
		//load the logger system	
		PropertyConfigurator.configure(Main.class.getResource("/com/radakan/game/config/log4jConfig.txt"));
			
	
		Logger.getRootLogger().setLevel(Level.DEBUG);
		logger.info("Starting up Radakan...");		
		
		Main main = new Main();
		main.init();
	}
	
	/**Shuts down the Radakan game.*/
	private void shutDown()
	{
		logger.info("Shutting down Radakan...");
		//TODO: Shutdown game
	}
}
