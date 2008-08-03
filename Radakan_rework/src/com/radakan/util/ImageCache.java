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
package com.radakan.util;

import java.awt.Image;
import java.awt.Toolkit;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.Set;

/** A cache of images. This class provides <code>static</code> methods only.
 *  The cache only stores images that have a reference number greater then 0. This cuts
 *  down on the amount of memory used. Any objects that remove an image from the cache 
 *  need to alert the cache when they are no longer using them by using the method 
 *  <code>imageFreed()</code>. 
 * 
 * @created Jun 24, 2008
 * @version 1.0.0
 * @author Joshua Montgomery
 */
public class ImageCache
{
	/**Cache of images.*/
	private static Map<KeyContainer,Image> cache = new HashMap<KeyContainer,Image>();
	
	/**The keys in the cache.*/
	private static Set<KeyContainer> keys = cache.keySet();
		
	/**Caches an Image with a specified key assigned to it. If the key already exist the image
	 * associated with it is replaced.
	 * 
	 * @param key Key to identify image
	 * @param image Image to cache
	 */
	public static void cacheImage(Object key,Image image)
	{	
		cache.put(new KeyContainer(key),image);
	}
	
	/**Removes a cached image associated with the key.
	 * 
	 * @param key Key for the image to remove.
	 */
	public static void removeCachedImage(Object key)
	{		
		cache.remove(key);
	}
	
	/**Decreases the references to an image in the cache. This method should be called
	 * when an object is no longer using the image it initially retrieved from the cache.
	 * 
	 * @param key Key of the image no longer being used by an object.
	 */
	public static void imageFreed(Object key)
	{
		KeyContainer c = getKeyFromSet(key);
		
		if(c != null)
		{
			c.decreaseRefs();
			
			if(!c.hasReferences())
			{
				Image i = cache.get(c);
				if(i != null)
					i.flush();
				cache.remove(c);
			}
				
		}
	}

	/**Retrieves the key from the key set that contains the real key passed in.
	 * 
	 * @param key Real key
	 * @return KeyContainer holding real key.
	 */
	private static KeyContainer getKeyFromSet(Object key)
	{
		Iterator<KeyContainer> i = keys.iterator();
		KeyContainer temp;
	
		while(i.hasNext())
		{
			temp = i.next();
			
			if(temp.equals(key))
				return temp;
		}
		return null;
	}
	
	/**Checks the cache for the passed in key and returns the image associated with it if
	 * the key exist.
	 * 
	 * @param key Key to check cache for.
	 * @return The image associated with the key or NULL if the key does not exist in the cache.
	 */
	public static Image retrieveCachedImage(Object key)
	{
		KeyContainer c = getKeyFromSet(key);
		
		if(c != null)
		{
			c.increaseRefs();
			return cache.get(c);
		}		
		
		return null;
	}

	/**Checks for a cache of the passed in image. If a cache of an image can not be found an
	 * attempt to load it from the bundle will be made.
	 * 
	 * @param location The class whose resource directory contains the file.
	 * @param fileName File name of the image to retrieve from cache (fileName = Key).
	 * @return The image
	 * @throw NullPointerException if the image could not be found. 
	 */
	public static Image retrieveCachedImage(String filePath)
	{
		KeyContainer c = getKeyFromSet(filePath);
		
		if(c != null)
		{
			c.increaseRefs();
			Image img  = cache.get(c);
			return img;
		}
				
		//Try to load the image from the bundle		
		Image img = Toolkit.getDefaultToolkit().getImage(ImageCache.class.getResource(filePath));
		
		if(img == null)
			throw new NullPointerException("Could not find file [" + filePath +"].");
		
		cache.put(new KeyContainer(filePath), img);//store image in cache
		return img;
	}
		
	/**Clears and disposes of all the images in the cache. Any reference to an image 
	 * within the cache will be invalid after this is called.
	 */
	public static void emptyCache()
	{
		int len = cache.size();
		Image img;
		
		for(int i=0;i<len;i++)
		{
			img = cache.get(i);
			if(img != null)
				img.flush();
		}
		cache.clear();
	}
		
	/**<b>This object is only meant to be used with the <code>ImageCache</code> class.</b><br>
	 * A container for a key that points to an image. This container keeps track of the number
	 * of references that the image, pointed to by the real key, has. Once a key has 
	 * 0 references it should be removed from the cache.
	 * 
	 * @created Jun 25, 2008
	 * @version 1.0.0
	 * @author Joshua Montgomery
	 */
	private static class KeyContainer
	{
		/**A key to an image.*/
		private Object realKey;
		
		/**Number of objects that have a reference to the image pointed to by the key.*/
		private int refs = 1;
		
		/**
		 * Constructor - Constructs the key container with one reference.
		 *
		 * @param key A key that points to an image.
		 */
		public KeyContainer(Object key)
		{
			realKey = key;
		}
		
		public boolean equals(Object object)
		{
			return realKey.equals(object);
		}
		
		/**Increases the number of references the image has.*/
		public void increaseRefs()
		{
			refs++;
		}
		
		/**Decreases the number of references the image has.*/
		public void decreaseRefs()
		{
			refs--;
		}
		
		/**Checks if the image has any references.
		 * 
		 * @return True if the number of references is greater then 0.
		 */
		public boolean hasReferences()
		{
			return refs > 0;
		}
	}
}
