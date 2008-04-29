package com.gibbon.radakan.ai;

/**
 *
 * @author t
 */
public abstract class Communication {
    public Communication (Character newFrom, Character newTo) {
        from = newFrom;
        to = newTo;
    }
    
    final public Character from;
    final public Character to;
}
