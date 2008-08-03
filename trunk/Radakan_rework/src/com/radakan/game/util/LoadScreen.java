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
package com.radakan.game.util;

import com.jme.image.Texture;
import com.jme.renderer.Renderer;
import com.jme.scene.BillboardNode;
import com.jme.scene.Node;
import com.jme.scene.shape.Quad;
import com.jme.scene.state.TextureState;
import com.jme.util.TextureManager;


/**
 * @author Joshua Montgomery
 * @version 1.0.0
 * @created Aug 2, 2008
 */
public class LoadScreen
{	
	private Quad loadScreen;
	
	public LoadScreen(Node parentNode,TextureState ts)
	{		
		ts.setEnabled(true);
		Texture t = TextureManager.loadTexture(
					LoadScreen.class.getResource("/com/radakan/data/textures/logo.tga"));
		ts.setTexture(t);
		
		loadScreen = new Quad("Load Screen",t.getImage().getWidth(),t.getImage().getHeight());
		loadScreen.setRenderState(ts);
		
		BillboardNode imgNode = new BillboardNode("Load Screen Node");
		imgNode.setAlignment(BillboardNode.CAMERA_ALIGNED);
		imgNode.attachChild(loadScreen);
		
		parentNode.attachChild(imgNode);
	}
}
