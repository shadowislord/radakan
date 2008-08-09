/*
 * Radakan is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Radakan is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Radakan.  If not, see <http://www.gnu.org/licenses/>.
 */

package com.gibbon.radakan;


public class Trash {

//    public static CXTerrainPass loadTerrain(JmeContext cx){
//        CollisionTreeManager.getInstance().setTreeType(CollisionTree.AABB_TREE);
//        
//        URL heights = ResourceLocatorTool.locateResource(ResourceLocatorTool.TYPE_TEXTURE, "spirits.int16le");
//        RawHeightMap heightMap = new RawHeightMap(heights, 129, RawHeightMap.FORMAT_16BITLE, false);
//        
//        Vector3f terrainScale = new Vector3f(5, 0.003f, 6);
//        heightMap.setHeightScale(0.001f);
//        //MidPointHeightMap heightMap = new MidPointHeightMap(128, 1.9f);
//        //Vector3f terrainScale = new Vector3f(5,1,5);
//        //TerrainBlock terrain = new TerrainBlock("Terrain", heightMap.getSize(), terrainScale,
//        //                                   heightMap.getHeightMap(),
//        //                                   new Vector3f(0, 0, 0), false);
//        final TerrainPage terrain = new TerrainPage("Terrain", 33, heightMap.getSize(),
//                terrainScale, heightMap.getHeightMap(), false);
//        
//        Texture lightmap = loadTexture("lightmap.tga", false);
//        Texture basetex = loadTexture("baserock.dds", false);
//        
//        Texture alphatex0 = loadTexture("darkrockalpha3.tga", true);
//        Texture alphatex1 = loadTexture("deadalpha3.tga", true);
//        Texture alphatex2 = loadTexture("grassalpha3.tga", true);
//        Texture alphatex3 = loadTexture("roadalpha3.tga", true);
//        
//        Texture detailtex0 = loadTexture("darkrock.dds", false);
//        Texture detailtex1 = loadTexture("deadgrass.dds", false);
//        Texture detailtex2 = loadTexture("nicegrass.dds", false);
//        Texture detailtex3 = loadTexture("road.dds", false);
//
//        // Create the terrain pass
//        CXTerrainPass tPass = new CXTerrainPass();
//        tPass.setRenderMode(TerrainPass.MODE_BEST);
//        
//        // Add a terrain model to the pass
//        tPass.addTerrain(terrain);
//        
//        tPass.setTileScale(90);
//        
//        // Add the tiles
//        tPass.addDetail(basetex,null);
//        tPass.addDetail(detailtex0,alphatex0);
//        tPass.addDetail(detailtex1,alphatex1);
//        tPass.addDetail(detailtex2,alphatex2);
//        tPass.addDetail(detailtex3,alphatex3);
//        tPass.setLightmap(lightmap, 2.0f);
//        
//        ZBufferState zbuf = cx.getRenderer().createZBufferState();
//        zbuf.setFunction(ZBufferState.CF_LESS);
//        tPass.setPassState(zbuf);
//        
//        CullState cull = cx.getRenderer().createCullState();
//        cull.setCullMode(CullState.CS_BACK);
//        tPass.setPassState(cull);
//        
//        terrain.updateRenderState();
//        
//        terrain.setModelBound(new BoundingBox());
//        terrain.updateModelBound();
//        //test protection
//        CollisionTreeManager.getInstance().generateCollisionTree(CollisionTree.AABB_TREE, terrain, true);
//        
//        walkableNode.attachChild(terrain);
//        
//        if (cx.getRenderer().supportsVBO()){
//            cx.execute(new Callable<Void>(){
//                public Void call(){
//                    setVbo(terrain);
//                    return null;
//                }
//            });
//        }
//        
//        terrain.lockBounds();
//        terrain.lockBranch();
//        terrain.lockShadows();
//        terrain.lockTransforms();
//        
//        return tPass;
//    }
    
}
