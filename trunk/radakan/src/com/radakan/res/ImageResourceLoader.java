/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package com.radakan.res;

import java.awt.Image;
import java.io.IOException;
import java.io.InputStream;
import javax.imageio.ImageIO;

/**
 *
 * @author tomas
 */
public class ImageResourceLoader implements ResourceLoader{

	public Image load(String resourceName) throws IOException {
		return ImageIO.read(ResourceManager.getFileSystem().openStream(AccessMode.RAW, resourceName));
    }
}
