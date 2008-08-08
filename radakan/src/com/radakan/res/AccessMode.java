package com.radakan.res;

public enum AccessMode {

    /**
     * Raw access mode.
     * The data will be stored the exact way it has been read from the file system.
     */
    RAW,
    
    /**
     * Parsing access mode.
     * The data will be parsed, decompressed, or converted into a result.
     */
    PARSE,
    
    /**
     * Random access mode.
     * The data will be randomly accessed from various parts of the file.
     */
    RANDOM,
    
    /**
     * Stream access mode.
     * The data will be streamed at a relatively constant speed from the file.
     */
    STREAM
    
}
