/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package com.gibbon.radakan.ai;

/**
 *
 * @author t
 */
public final class Nothing {
    private Nothing () {
        assert false;
    };
    
    public static Nothing get () {
        return null;
    }
}
