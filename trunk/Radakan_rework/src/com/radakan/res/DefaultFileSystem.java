package com.radakan.res;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.RandomAccessFile;
import java.net.MalformedURLException;
import java.net.URISyntaxException;
import java.net.URL;
import java.nio.ByteBuffer;
import java.nio.channels.Channels;
import java.nio.channels.ReadableByteChannel;
import java.util.Map;
import java.util.TreeMap;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * DefaultFileSystem is just a wrapper for the usual MyClass.class.getClassLoader().getResource().
 * It builds a map of resources so references are made by filename. AccessMode is not taken into
 * account for any of the FileSystem methods yet.
 * 
 * A folder name must be provided as root, it has to be part of the resources.
 *
 * @author Tomás Lázaro
 */
public class DefaultFileSystem implements FileSystem {

	private File root;
	private Map<String, String> resources = new TreeMap<String, String>();

	public static void main(String args[]) {
		DefaultFileSystem sys = new DefaultFileSystem("/META-INF/MANIFEST.MF");
		System.err.println("Found: " + sys.findResource("Monkey.jpg"));
	}

   public DefaultFileSystem(String root) {
        try {
            this.root = new File(DefaultFileSystem.class.getClassLoader().getResource(root).toURI());
            buildResourceMap(this.root);
        } catch (URISyntaxException ex) {
            Logger.getLogger(DefaultFileSystem.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

	public InputStream openStream(AccessMode mode, String name) throws IOException {
		return new FileInputStream(getFile(mode, name));
	}

	private File getFile(AccessMode mode, String name) {
		File result = null;
		try {
			result = new File(findResource(name));
		} catch (Exception ex) {
			Logger.getLogger(DefaultFileSystem.class.getName()).log(Level.SEVERE, "failed to load: " + name + " found at " + findResource(name), ex);
		}
		return result;
	}

	private void buildResourceMap(File dir) {
		if (!dir.isDirectory()) {
			return;
		}

		File[] files = dir.listFiles();
		File file;
		for (int i = 0; i < files.length; i++) {
			file = files[i];
			if (file.isDirectory()) {
				buildResourceMap(file);
			} else {
				if (resources.put(file.getName().toLowerCase(), file.getPath()) != null) {
					Logger.getLogger(DefaultFileSystem.class.getName()).log(Level.SEVERE, "Duplicate Resource: " + file.getPath());
				}
			}
		}
	}

	private String findResource(String name) {
		return resources.get(name.toLowerCase());
	}

	private static byte[] getBytes(File file) {
		byte[] bytes = new byte[(int) file.length()];
		RandomAccessFile raf = null;
		try {
			raf = new RandomAccessFile(file, "r");
			raf.readFully(bytes);
		} catch (IOException ex) {
			Logger.getLogger(DefaultFileSystem.class.getName()).log(Level.SEVERE, null, ex);
		} finally {
			try {
				raf.close();
			} catch (IOException ex) {
				Logger.getLogger(DefaultFileSystem.class.getName()).log(Level.SEVERE, null, ex);
			}
		}
		return bytes;
	}

	public ReadableByteChannel openChannel(AccessMode mode, String name) throws IOException {
		return Channels.newChannel(openStream(mode, name));
	}

	public ByteBuffer openBuffer(AccessMode mode, String name) throws IOException {
		return ByteBuffer.wrap(getBytes(getFile(mode, name)));
	}

	public boolean exists(String name) {
		return getFile(AccessMode.STREAM, name).exists();
	}
}
