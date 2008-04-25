package com.gibbon.radakan.script;

import java.util.ArrayList;
import java.util.List;

public class ActEventObject implements ActObject {

    private List<ActEventListener> listeners = new ArrayList<ActEventListener>();

    public void addListener(ActEventListener listener){
        listeners.add(listener);
    }
    
    public void removeListener(ActEventListener listener){
        listeners.remove(listener);
    }

    public void notifyListeners(List<ActObject> eventParams){
        for (ActEventListener listener : listeners){
            listener.onEvent(eventParams);
        }
    }
    
}
