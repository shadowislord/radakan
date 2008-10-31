///*
// * Radakan is free software: you can redistribute it and/or modify
// * it under the terms of the GNU General Public License as published by
// * the Free Software Foundation, either version 3 of the License, or
// * (at your option) any later version.
// *
// * Radakan is distributed in the hope that it will be useful,
// * but WITHOUT ANY WARRANTY; without even the implied warranty of
// * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// * GNU General Public License for more details.
// *
// * You should have received a copy of the GNU General Public License
// * along with Radakan.  If not, see <http://www.gnu.org/licenses/>.
// */
//
//package com.radakan.entity.unit;
//
//import com.jme.math.FastMath;
//import com.jme.math.Vector3f;
//import java.io.IOException;
//import java.io.PrintStream;
//
//import org.w3c.dom.Node;
//
//import com.jme.util.export.JMEExporter;
//import com.jme.util.export.JMEImporter;
//import com.radakan.game.tile.TileManager;
//
//public class MovementUnit extends Unit {
//
//    private float moveSpeed = 0f;
//    private Vector3f targetLoc;
//    private Vector3f curLoc = new Vector3f();
//    private Vector3f invVel = new Vector3f();
//    private Vector3f volVel = new Vector3f();
//    
//    private boolean canWalk = true;
//    
//    private static enum LocationType {
//        Air,
//        Underground,
//        Ground,
//        Water,
//        Unknown
//    }
//    
//    private LocationType getTypeAt(Vector3f pos){
//        float terrainHeight = TileManager.getInstance().getTerrainHeight(pos);
//        float currentHeight = entity.getModel().getModel().getLocalTranslation().y;
//        if ( FastMath.abs(terrainHeight - currentHeight) < FastMath.ZERO_TOLERANCE ){
//            return LocationType.Ground;
//        }else if (terrainHeight > currentHeight){
//            return LocationType.Underground;
//        }else if (terrainHeight < currentHeight){
//            return LocationType.Air;
//        }
//        
//        return LocationType.Unknown;
//    }
//    
//    public void moveTo(Vector3f pos){
//        targetLoc = pos;
//    }
//    
//    @Override
//    public void update(float tpf) {
//        LocationType locType = getTypeAt(curLoc);
//        if (locType == LocationType.Ground){
//            if (canWalk){
//                Vector3f dir = targetLoc.subtract(curLoc).normalizeLocal();
//                
//            }
//        }
//    }
//
//    @Override
//    public void read(JMEImporter im) throws IOException {
//    }
//
//    @Override
//    public void write(JMEExporter ex) throws IOException {
//    }
//
//    public void exportXML(PrintStream stream) {
//    }
//
//    public void importXML(Node rootEntityNode) {
//    }
//
//    public int getType() {
//        return MOVEMENT;
//    }
//
//    public void onUnitEvent(UnitEvent event) {
//    }
//}
