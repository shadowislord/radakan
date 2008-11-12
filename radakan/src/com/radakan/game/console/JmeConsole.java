package com.radakan.game.console;

import com.gibbon.jme.context.JmeContext;
import com.gibbon.jme.pass.PassType;
import com.gibbon.jme.pass.RenderPass;
import com.jme.image.Texture;
import com.jme.image.Texture.MagnificationFilter;
import com.jme.image.Texture.MinificationFilter;
import com.jme.input.KeyInput;
import com.jme.input.KeyInputListener;
import com.jme.renderer.ColorRGBA;
import com.jme.scene.Node;
import com.jme.scene.Spatial.CullHint;
import com.jme.scene.Spatial.TextureCombineMode;
import com.jme.scene.Text;
import com.jme.scene.state.BlendState;
import com.jme.scene.state.BlendState.DestinationFunction;
import com.jme.scene.state.BlendState.SourceFunction;
import com.jme.scene.state.BlendState.TestFunction;
import com.jme.scene.state.TextureState;
import com.jme.system.DisplaySystem;
import com.jme.util.TextureManager;
import java.io.IOException;
import java.io.OutputStream;
import java.io.PrintStream;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.logging.Handler;
import java.util.logging.Level;
import java.util.logging.LogRecord;
import java.util.logging.Logger;

/**
 * JmeConsole is an in-game console that displays logging messages
 * and allows execution of scripting commands using Radakan's scripting system.
 * JmeConsole is implemented as a RenderPass and is therefore independent
 * of any other systems.
 * 
 * @author Kirill Vainer
 */
public class JmeConsole extends RenderPass implements KeyInputListener {
    
    /**
     * An output stream that sends all recieved data as commands to it's parent.
     */
    private class JmeConsoleStandardOutput extends OutputStream {
        @Override
        public void write(int b) throws IOException {
            //print1(ColorRGBA.white, "" + ((char)b), 1000, false);
        }
        
        @Override
        public void write(byte[] data){
            write(data, 0, data.length);
        }
        
        @Override
        public void write(byte[] data, int offset, int length){
            String s = new String(data, offset, length);
            print1(ColorRGBA.white, s, 1000 * s.length() * 1000, true);
        }
    }
    
    private final JmeConsoleStandardOutput output = new JmeConsoleStandardOutput();
    
    /**
     * Number of text nodes to hold
     */
    private static final int TEXT_COUNT = 25;

    /**
     * Where you write stuff
     */
    private final Text consoleInput = new Text("InputLine","");   
    
    /**
     * The front buffer of the consoleInput text
     */
    private final StringBuffer inputString = consoleInput.getText().append(">  ");
    
    private final StringBuffer baseString = new StringBuffer();
    
    /**
     * Location of the text writing cursor
     */
    private int cursorLoc = 0;
    
    private boolean consoleVisible = false;
    
    private boolean cursorVisible = false;
    
    private boolean fadeEnabled = false;
    
    private Node consoleNode = new Node("ConsoleNode");
    
    private final List <LogText> texts = new ArrayList <LogText> (TEXT_COUNT);
    private final List <LogText> pool = new ArrayList <LogText> (TEXT_COUNT);
        
    private float pingTime = 0f;
    
//    final List <PrintedLine> synco = new ArrayList <PrintedLine> ();
    
    private final List<ConsoleListener> listeners = new ArrayList<ConsoleListener>();
    
    /**
     * Creates the console. Initializes render states and the text spatials.
     */
    public JmeConsole() {
        super(PassType.POST_RENDER, "Console");
        
        Logger.getLogger("com.radakan").addHandler(new JmeConsoleHandler());
        
        BlendState as = DisplaySystem.getDisplaySystem().getRenderer().createBlendState();
        as.setBlendEnabled( true );
        as.setSourceFunction( SourceFunction.SourceAlpha );
        as.setDestinationFunction( DestinationFunction.One );
        as.setTestEnabled( true );
        as.setTestFunction( TestFunction.GreaterThan );
        consoleNode.setRenderState(as);
        
        TextureState ts = DisplaySystem.getDisplaySystem().getRenderer().createTextureState();
        Texture t = TextureManager.loadTexture("defaultfont.tga", 
                                 MinificationFilter.BilinearNoMipMaps, 
                                 MagnificationFilter.Bilinear);
        ts.setTexture(t);
        consoleNode.setRenderState(ts);

        consoleNode.setTextureCombineMode(TextureCombineMode.Replace);
            
        consoleNode.attachChild(consoleInput);
        
        for (int x = 0; x < TEXT_COUNT; x++){
            LogText lt = new LogText(this,"LogText_"+x,"");
            consoleNode.attachChild(lt);
            pool.add(lt);
        }
        
        consoleNode.updateRenderState();
        consoleNode.updateGeometricState(0,true);
    }
    
    /**
     * @return the command output stream
     */
    public OutputStream getOutputStream(){
        return output;
    }
    
    /**
     * register with keyboard input if available
     */
    @Override
    public void initPass(JmeContext cx){
        if (KeyInput.isInited()){
            KeyInput.get().addListener(this);
        }
    }
    
    public void updateOutputString(){
        if (!cursorVisible){
            // Cursor hidden
            inputString.setLength(0);
            inputString.append("> ").append(baseString);
            if (cursorLoc == baseString.length()) inputString.append(' ');
        }else{
            inputString.setLength(0);
            inputString.append("> ").append(baseString);
            if (cursorLoc == baseString.length()) inputString.append('_');
            else{
                inputString.setCharAt(cursorLoc+2,'_');
            }
        }
    }
    
    public void onKey(char key, int keyId, boolean pressed) {
        if (!pressed)
            return;
        
        if (keyId == KeyInput.KEY_TAB){
            consoleVisible = !consoleVisible;
        }
        
        if (!consoleVisible)
            return;
        
        switch (keyId){
            case KeyInput.KEY_BACK:
                writeBackspace();
                return;
            case KeyInput.KEY_RETURN:
                writeEnter();
                return;
            case KeyInput.KEY_DELETE:
                writeDelete();
                return;
            case KeyInput.KEY_LEFT:
                moveCaret(-1);
                return;
            case KeyInput.KEY_RIGHT:
                moveCaret(1);
                return;
            case KeyInput.KEY_SPACE:
                writeChar(' ');
                return;
        }
        
        if (Character.isLetterOrDigit(key)
         || (key >= '!' && key <= '~')){
            writeChar(key);
        }
    }
    
    /**
     * Renders the console to the screen if it's visible
     */
    @Override
    public void doRender(JmeContext cx){
        if (consoleVisible)
            cx.getRenderer().draw(consoleNode);
    }
    
    /**
     * Does the caret blinking, and fades the expired text spatials.
     */
    @Override
    public void doUpdate(JmeContext cx) {
//        synchronized (synco) {
//            for (PrintedLine pl : synco) {
//                pl.exec();
//            }
//            synco.clear();
//        }

        float tpf = cx.getPassManager().getTPF();
        pingTime += tpf;

        if (pingTime > 0.5f && !cursorVisible) {
            // make cursor visible
            inputString.setCharAt(cursorLoc + 2, '_');
            cursorVisible = true;
        } else if (pingTime > 1f && cursorVisible) {
            // hide cursor
            if (cursorLoc == baseString.length()) {
                inputString.setCharAt(2 + cursorLoc, ' ');
            } else {
                inputString.setCharAt(2 + cursorLoc, baseString.charAt(cursorLoc));
            }
            cursorVisible = false;
            pingTime = 0f;
        }

        if (fadeEnabled) {
            for (int x = 0; x < texts.size(); x++) {
                texts.get(x).update(tpf);
            }
        }
    }
    
    /**
     * Adds a console listener to listen to events of this console.
     * @param listener
     */
    public void addConsoleListener(ConsoleListener listener){
        listeners.add(listener);
    }
    
    public void removeConsoleListener(ConsoleListener listener){
        listeners.remove(listener);
    }
    
    /**
     * Removes all expired text spatials and puts them into the pool.
     */
    protected void clean(){
        for (int i = texts.size()-1; i >= 0; i--){
            LogText lt = texts.get(i);
            if (lt.isActive()) return;
            texts.remove(i);
            pool.add(lt);
        }
    }
    
    /**
     * Clears the entire console from text. 
     */
    public void clear(){
        baseString.setLength(0);
        inputString.setLength(0);
        inputString.append(">  ");
        cursorLoc = 0;
        consoleInput.print(inputString);
        
        for (LogText lt: texts){
            pool.add(lt);
            lt.setTime(0);
            lt.setCullHint(CullHint.Always);
        }
        texts.clear();
        
        consoleNode.updateGeometricState(0,true);
    }
    
    public void writeChar (char c){
        baseString.insert(cursorLoc,c);
        cursorLoc++;
        updateOutputString();
    }
    public void writeBackspace (){
        if (inputString.length()>3){
            cursorLoc--;
            baseString.deleteCharAt(cursorLoc);
            updateOutputString();
        }
            
    }
    public void writeEnter() {
        String toPrint = baseString.toString();
        /*
        if (toPrint.startsWith("%") && toPrint.length()>2){
            char color = toPrint.charAt(1);
            toPrint = toPrint.substring(2);
            ColorRGBA rgba;
            switch (color){
                case 'R': rgba = ColorRGBA.red; break;
                case 'G': rgba = ColorRGBA.green; break;
                case 'B': rgba = ColorRGBA.blue; break;
                case 'Y': rgba = ColorRGBA.yellow; break;
                case 'W': rgba = ColorRGBA.white; break;
                case 'P': rgba = ColorRGBA.magenta; break;
                case 'A': rgba = ColorRGBA.gray; break;
                case 'C': rgba = ColorRGBA.cyan; break;
                case 'O': rgba = ColorRGBA.orange; break;
                default : rgba = ColorRGBA.white; break;
            }
            println(rgba,toPrint);
        }else
            println(toPrint);
        */
        println("> "+toPrint);
        
        for (ConsoleListener listener : listeners)
            listener.commandTyped(this, toPrint);
       
        baseString.setLength(0);
        cursorLoc = 0;
        updateOutputString();
    }
    public void writeDelete(){
        if (cursorLoc < baseString.length()){
            baseString.deleteCharAt(cursorLoc);
            updateOutputString();
        }
    }
    
    public void moveCaret(int direction){
        if (direction < 0){
            if (cursorLoc > 0){
                cursorLoc --;
            }
        }else if (direction > 0){
            if (cursorLoc < baseString.length()){
                cursorLoc ++;
            }
        }
        updateOutputString();
    }
    
     

    
     

    private class JmeConsoleHandler extends Handler {
        @Override
        public void publish(LogRecord record) {
            String clazz = record.getSourceClassName();
            if (clazz.indexOf(".") > 0)
                clazz = clazz.substring(clazz.lastIndexOf(".")+1);
            
            String message = clazz + ": " + record.getMessage();

            Level level = record.getLevel();
            if (level == Level.SEVERE) {
                println(ColorRGBA.red, message);
            } else if (level == Level.WARNING) {
                println(ColorRGBA.orange, message);
            } else if (level == Level.INFO) {
                println(ColorRGBA.magenta, message);
            } else if (level == Level.CONFIG) {
                println(ColorRGBA.cyan, message);
            } else if (level == Level.FINE) {
                println(ColorRGBA.white, message);
            } else if (level == Level.FINER) {
                println(ColorRGBA.gray, message);
            } else if (level == Level.FINEST) {
                println(ColorRGBA.darkGray, message);
            }
        }

        @Override
        public void flush() {
            throw new UnsupportedOperationException("Not supported yet.");
        }

        @Override
        public void close() throws SecurityException {
            throw new UnsupportedOperationException("Not supported yet.");
        }
    }

    public void println(ColorRGBA color, String ln, int time){
        print1(color,ln,time,true);
    }
    public void println(ColorRGBA color, String ln){
        print1(color,ln,ln.length()*1000+1000,true);
    }
    public void println(String ln, int time){
        print1(null,ln,time,true);
    }
    public void println(String ln){
        print1(null,ln,ln.length()*1000+1000,true);
    }
    
    protected void print1  (ColorRGBA color, String ln, int time, boolean newLine){
        List<String> split = Arrays.asList(ln.split("\n"));
        List<String> printList = new ArrayList<String>();
        
        for (String str : split){
            if (str.length() > 70){
                StringBuffer sb = new StringBuffer(str);
                int index = 0;
                int end = 0;
                do
                {
                    end = index + 70;
                    if (end > str.length()){
                        end = str.length();
                    }
                    printList.add(sb.substring(index,end));
                    index += 70;
                }
                while (index < str.length());
            }else
                printList.add(str);
        }
        
        for (String str : printList)
            print0(color, str, time, newLine);
    }
    
    protected void print0  (ColorRGBA color, String ln, int time, boolean newLine){
        LogText lt = null;
        clean();
        if (!newLine && texts.size()>0){
            lt = texts.get(0);
        }else{ 
            if (texts.size()==TEXT_COUNT){
                // Trash out last string
                lt = texts.remove(texts.size()-1);
            }else if (!pool.isEmpty()){
                lt = pool.remove(0);
            }else{
                throw new RuntimeException("Run out of logtext!");
            }
        }
        lt.reset();
        lt.setText(color,ln);
        lt.setTime(time);
        texts.add(0,lt);
        
        float h = consoleInput.getHeight();
        for (LogText tt: texts){
            tt.setLocalTranslation(0,h,0);
            h += tt.getHeight();
        }
        
        consoleNode.updateGeometricState(0,true);
    }
//    protected void print1  (ColorRGBA color, String ln, int time, boolean newLine){
//        synchronized (synco){
//            synco.add(new PrintedLine(color,ln,time,newLine));
//        }
//    }

}
