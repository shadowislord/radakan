package com.radakan.res;

import java.io.IOException;
import java.io.InputStream;

public class RawResourceLoader implements ResourceLoader<InputStream> {

    public InputStream load(String resourceName) throws IOException {
        return ResourceManager.getFileSystem().openStream(AccessMode.RAW, resourceName);
    }

    public boolean useCache() {
        return true;
    }

}
