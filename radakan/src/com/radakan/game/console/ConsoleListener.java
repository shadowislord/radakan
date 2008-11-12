package com.radakan.game.console;

/**
 * ConsoleListener listens to various events of the game console.
 * 
 * @see JmeConsole
 * @author Kirill Vainer
 */
public interface ConsoleListener {

    /**
     * Called when a command has been typed in the console.
     * 
     * @param console The console that had the command typed in.
     * @param command The actual command that was typed by the user.
     */
    public void commandTyped(JmeConsole console, String command);
    
}
