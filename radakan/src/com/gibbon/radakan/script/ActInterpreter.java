package com.gibbon.radakan.script;

import java.io.IOException;
import java.io.InputStreamReader;
import java.io.Reader;
import java.io.StreamTokenizer;
import java.util.HashMap;
import java.util.Map;
import org.lwjgl.opengl.Display;
import org.lwjgl.opengl.Pbuffer;
import org.lwjgl.opengl.PixelFormat;


public final class ActInterpreter {
    
    private String subject = null;
    private Map<String, ActObject> scope = new HashMap<String, ActObject>();
    
    public ActInterpreter() {
    }

    public void setSubject(String subject){
        this.subject = subject;
    }
    
    public void setupReader(StreamTokenizer reader){
        reader.commentChar('\'');
        reader.eolIsSignificant(true);
        reader.parseNumbers();
        reader.quoteChar('"');
        reader.ordinaryChar('.');
        reader.ordinaryChar('=');
        reader.ordinaryChar('(');
        reader.ordinaryChar(')');
        reader.ordinaryChar(',');
    }
    
    public String tokenType(int t){
        switch (t){
            case StreamTokenizer.TT_EOF:
                return "EOF";
            case StreamTokenizer.TT_EOL:
                return "EOL";
            case StreamTokenizer.TT_NUMBER:
                return "Number";
            case StreamTokenizer.TT_WORD:
                return "Word";
            default:
                return (char)t + "";
        }
    }
    
    public String readIdentifier(StreamTokenizer reader) throws IOException{
        reader.nextToken();
        
        if (reader.ttype != StreamTokenizer.TT_WORD)
            throw new ActSyntaxException(reader.lineno(), "Expected a word");
        
        String word = reader.sval;
        
        boolean dot = true;
        do{
            reader.nextToken();
            //System.out.println(reader.toString());
            if (reader.ttype == '.' && !dot){
                throw new ActSyntaxException(reader.lineno(), "Expected a word");
            }else if (reader.ttype == StreamTokenizer.TT_WORD && !dot){
                dot = true;
                word = word + "." + reader.sval;
            }else if (reader.ttype == '.' && dot){
                dot = false;
                continue;
            }else if (reader.ttype == StreamTokenizer.TT_EOL){
                reader.pushBack();
                break;
            }else if (dot){
                reader.pushBack();
                break;
            }else
                throw new ActSyntaxException(reader.lineno(), dot ? "Expected '.'" : "Expected a word");

        } while (true);

        if (dot == false){
            throw new ActSyntaxException(reader.lineno(), "Expected a word");
        }
        
        return word;
    }
    
    public boolean readStatement(StreamTokenizer reader) throws IOException{
        reader.nextToken();
        //System.out.println(reader.toString());

        if (reader.ttype == StreamTokenizer.TT_EOF)
            return false;
        
        if (reader.ttype == StreamTokenizer.TT_EOL)
            return true;
        
        if (reader.ttype != StreamTokenizer.TT_WORD)
            throw new ActSyntaxException(reader.lineno(), "Expected a word");
        
        String word = reader.sval;
        ActStatement stat = null;
        
        if (word.equalsIgnoreCase("If")){
            //stat = new ActIfStatement();
            //readIfStatement(reader);
        }else if (word.equalsIgnoreCase("When")){
            //stat = new ActEventStatement();
            //readEventStatement(reader);
        }else if (word.equalsIgnoreCase("Else")){
           
            
        }else{
            String id = readIdentifier(reader);
            
            reader.nextToken();
            
            if (reader.ttype == '='){
                // this is an assignment
            }else{
                if (reader.ttype == StreamTokenizer.TT_NUMBER){
                    
                }
                // routine call
                String arg = readIdentifier(reader);
                
                reader.nextToken();
                
                //if (reader.ttype == ',')
                
            }
            // search for object in map
            
        }
            
        return true;
    }
    
    public void execute(Reader r) throws IOException{
        StreamTokenizer reader = new StreamTokenizer(r);
        setupReader(reader);
        
        while (true){
            if (!readStatement(reader))
                return;
        }
    }
    
    public static void main(String[] args) throws IOException{
        ActInterpreter actip = new ActInterpreter();
        actip.execute(new InputStreamReader(System.in));
    }
    
}
