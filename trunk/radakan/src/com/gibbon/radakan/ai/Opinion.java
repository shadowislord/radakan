package com.gibbon.radakan.ai;

/**
 *
 * @author t
 */
public final class Opinion {
    public Opinion(Character new_about) {
        about = new_about;
    }

    final public Character about;

    public Float like = new Float(0.5);
    public Float fear = new Float(0);
}


