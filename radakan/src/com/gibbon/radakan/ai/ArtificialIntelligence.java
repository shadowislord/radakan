/*
 * Radakan RPG is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Radakan RPG is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Radakan RPG.  If not, see <http://www.gnu.org/licenses/>.
 */

package com.gibbon.radakan.ai;

import java.util.*;

///	ArtificialIntelligence is the basic strategy for living NPCs.
public class ArtificialIntelligence extends Behavior {
    /*private static Set <TiXmlDocument> allBehaviorFiles = new Map <TiXmlDocument> ();*/

    public ArtificialIntelligence(Character newCharacter) {
    	super(newCharacter);
        calm(1, 0, 1);
    }

        ///	The message is stored.
    public void call(Communication message) {
            //	We'll store a copy of the message for now, and process it on the next 'transit'.
        sensoryBuffer.add(message);
    }

    ///	My FSM will process the last stored message.
    public Behavior transit(Nothing message) {
        Communication actualMessage;

        if(! sensoryBuffer.isEmpty()) {
            actualMessage = sensoryBuffer.remove();

            sensoryBuffer.remove(actualMessage);
        }

        character.getBody().walk(0);
        character.getBody().turn(0);

        stateMachine.run(actualMessage);

        return (Behavior)this;
    }

    /*  virtual Boolean evaluateCondition(TiXmlElement * element) {
	
		Boolean result = true;
	
		for(TiXmlAttribute * attribute = element.FirstAttribute(); attribute != null;
			attribute = attribute.Next()) {
			result = result && evaluateExpression(attribute);
		}
	
		return result;
	}
    virtual Boolean evaluateExpression(TiXmlAttribute * attribute) {
	
		String & name = attribute.NameTStr();
		String & value = attribute.ValueStr();
		Engines.Log.log(this) << "evaluate: " << name  << " ? " << value << endl;
		
	
		if(name == "action") {
			Engines.Log.log(this) << "result: "
				<< toString(getCurrentActionName() == value) << endl;
			
			return getCurrentActionName() == value;
		}
	
		Boolean larger =(value.at(0) == '>');
		String temp = value;
		temp.erase(temp.begin());
		float numericValue = toFloat(temp);
	
		Boolean isSmaller;
		if(name == "fearsPlayer") {
			isSmaller =(numericValue < 0.2);
		}
		else if(name == "likesPlayer") {
			isSmaller =(numericValue < 0.5);
		}
		else {
			isSmaller =(numericValue < character.lock().getSkill(name));
		}
	
		Engines.Log.log(this) << "result: " << toString(isSmaller != larger) << endl;
		return(isSmaller != larger);
	}
	*/

    public String getCurrentActionName() {
        if(stateMachine.hasState()) {
            return stateMachine.getState().getClass().getName();
        }
        else {
            return "none";
        }
    }
    
    /*

    void setAction(String actionName) {
    	if(actionName != getCurrentActionName()) {
    			setState(<Actions.Action>
    				(new Actions.Conversing
    					(character.lock(), boost.dynamicPointerCast <ArtificialIntelligence>(this))));
    		}
    			setState(<Actions.Action>
    				(new Actions.Fighting
    					(character.lock(), boost.dynamicPointerCast <ArtificialIntelligence>(this))));
    		}
    		else {
    			
    			setState(<Actions.Action>());
    		}
    	}
    }
    */

    /* Set <TiXmlDocument> getBehaviorFiles();*/

    //	'calm' can vary from 0 to 1.
    BoundedFloat calm;

    StrategyStateMachine <Communication, Action> stateMachine;

    /*private Set <TiXmlDocument> behaviorFiles;*/

    private Queue<Communication> sensoryBuffer = new ArrayDeque<Communication>();
    private Set<Opinion> opinions = new HashSet<Opinion>();
};

