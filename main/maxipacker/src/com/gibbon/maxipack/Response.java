package com.gibbon.maxipack;

import java.util.Properties;

public class Response extends Properties {

    public int getStatus(){
        return Integer.parseInt(getProperty("status"));
    }
    
}
