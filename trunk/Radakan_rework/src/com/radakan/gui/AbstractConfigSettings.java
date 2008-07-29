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
package com.radakan.gui;

import com.jme.system.GameSettings;

/**Abstract class to be used for classes setting the game settings.
 * 
 * @author Joshua Montgomery
 * @version 1.0.0
 * @created Jul 28, 2008
 */
public abstract class AbstractConfigSettings
{
	protected GameSettings settings;
	
	/**Constructs the <code>AbstractConfigSettings</code>
	 * 
	 * @param settings Settings to use.
	 */
	public AbstractConfigSettings(GameSettings settings)
	{
		this.settings = settings;
		
		if(settings == null)
			throw new NullPointerException("Settings can not be null.");
		
		initDefaultSettings();
	}
	
	/**Sets the default settings for the system. Automatically called by the constructor.*/
	protected abstract void initDefaultSettings();
	
	/**Configures the game settings.*/
	public abstract void configure();
}
