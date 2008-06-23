/*
 * Copyright (c) 2007, MFKARPG
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     - Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     - Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     - Neither the name of the Gibbon Entertainment nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY 'Gibbon Entertainment' "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL 'Gibbon Entertainment' BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

package com.gibbon.mfkarpg.terrain.splatting;

import com.jme.app.SimplePassGame;
import com.jme.image.Texture;
import com.jme.image.Texture.CombinerScale;
import com.jme.image.Texture.MagnificationFilter;
import com.jme.image.Texture.MinificationFilter;
import com.jme.input.FirstPersonHandler;
import com.jme.math.Vector3f;
import com.jme.renderer.pass.RenderPass;
import com.jme.scene.state.CullState;
import com.jme.scene.state.CullState.Face;
import com.jme.scene.state.ZBufferState;
import com.jme.scene.state.ZBufferState.TestFunction;
import com.jme.util.TextureManager;
import com.jmex.terrain.TerrainPage;
import com.jmex.terrain.util.RawHeightMap;

public class TestSplatPage extends SimplePassGame {
    
    private TerrainPage page;
    
    public static void main(String[] args){
        TestSplatPage game = new TestSplatPage();
        game.setConfigShowMode(ConfigShowMode.AlwaysShow);
        game.start();
    }
    
    protected void simpleInitGame() {
        display.setTitle("Hardware Splatting Test");
        
        input = new FirstPersonHandler(cam, 150, 1);
     
        ClassLoader loader = getClass().getClassLoader();
        
        RawHeightMap heightMap = new RawHeightMap(TestSplatPage.class
                .getClassLoader().getResource(
                        "jmetest/data/texture/terrain/heights.raw").getFile(), 129,
                RawHeightMap.FORMAT_16BITLE, false);

        Vector3f terrainScale = new Vector3f(5, 0.003f, 6);
        heightMap.setHeightScale(0.001f);
        page = new TerrainPage("Terrain", 33, heightMap.getSize(),
                terrainScale, heightMap.getHeightMap(), false);
        page.getLocalTranslation().set(0, 10, 0);
        
        // ZBuffer
        ZBufferState buf = display.getRenderer().createZBufferState();
        buf.setFunction(TestFunction.LessThanOrEqualTo);
        
        // Backface culling
        CullState cs = display.getRenderer().createCullState();
        cs.setCullFace(Face.Back);
        cs.setEnabled(true);
        
        
        /*
        // Generate alphamaps by height
        ProceduralAlphaGenerator alphaGen = new ProceduralAlphaGenerator(heightMap);
        
        java.awt.Image alpha0 = alphaGen.generateAlpha(512,-128,0,128);
        Texture alphatex0 = TextureManager.loadTexture(alpha0,Texture.MM_LINEAR_LINEAR,MagnificationFilter.Bilinear,0.0f,true);
        //alphatex0.setStoreTexture(true);
        
        java.awt.Image alpha1 = alphaGen.generateAlpha(512,0,128,256);
        Texture alphatex1 = TextureManager.loadTexture(alpha1,Texture.MM_LINEAR_LINEAR,MagnificationFilter.Bilinear,0.0f,true);
        //alphatex1.setStoreTexture(true);
        
        java.awt.Image alpha2 = alphaGen.generateAlpha(512,128,256,384);
        Texture alphatex2 = TextureManager.loadTexture(alpha2,Texture.MM_LINEAR_LINEAR,MagnificationFilter.Bilinear,0.0f,true);
        //alphatex2.setStoreTexture(true);
         
        // Last one uses terrainTex.png, a pregenerated alphamap
        Texture alphatex3 = TextureManager.loadTexture(loader.getResource("jmetest/data/texture/terrainTex.png"),Texture.MM_LINEAR_LINEAR,MagnificationFilter.Bilinear);
        */
        
        
        
        Texture lightmap = TextureManager.loadTexture(
                loader.getResource("jmetest/data/texture/terrain/lightmap.jpg")
                ,MinificationFilter.Trilinear,MagnificationFilter.Bilinear,0.0f,true);
        
        Texture basetex = TextureManager.loadTexture(
                loader.getResource("jmetest/data/texture/terrain/baserock.jpg")
                ,MinificationFilter.Trilinear,MagnificationFilter.Bilinear,0.0f,false);
        
        Texture alphatex0 = TextureManager.loadTexture(
                loader.getResource("jmetest/data/texture/terrain/darkrockalpha.png")
                ,MinificationFilter.Trilinear,MagnificationFilter.Bilinear,0.0f,true);
        
        Texture alphatex1 = TextureManager.loadTexture(
                loader.getResource("jmetest/data/texture/terrain/deadalpha.png")
                ,MinificationFilter.Trilinear,MagnificationFilter.Bilinear,0.0f,true);
        
        Texture alphatex2 = TextureManager.loadTexture(
                loader.getResource("jmetest/data/texture/terrain/grassalpha.png")
                ,MinificationFilter.Trilinear,MagnificationFilter.Bilinear,0.0f,true);
        
        Texture alphatex3 = TextureManager.loadTexture(
                loader.getResource("jmetest/data/texture/terrain/roadalpha.png")
                ,MinificationFilter.Trilinear,MagnificationFilter.Bilinear,0.0f,true);
        
        Texture detailtex0 = TextureManager.loadTexture(
                loader.getResource("jmetest/data/texture/terrain/darkrock.jpg")
                ,MinificationFilter.Trilinear,MagnificationFilter.Bilinear,0.0f,false);
        
        Texture detailtex1 = TextureManager.loadTexture(
                loader.getResource("jmetest/data/texture/terrain/deadgrass.jpg")
                ,MinificationFilter.Trilinear,MagnificationFilter.Bilinear,0.0f,false);
        
        Texture detailtex2 = TextureManager.loadTexture(
                loader.getResource("jmetest/data/texture/terrain/nicegrass.jpg")
                ,MinificationFilter.Trilinear,MagnificationFilter.Bilinear,0.0f,false);
        
        Texture detailtex3 = TextureManager.loadTexture(
                loader.getResource("jmetest/data/texture/terrain/road.jpg")
                ,MinificationFilter.Trilinear,MagnificationFilter.Bilinear,0.0f,false);
                
        
        // Create the terrain pass
        TerrainPass tPass = new TerrainPass();
        
        tPass.setRenderMode(TerrainPass.MODE_BEST);
        
        // Z test
        tPass.setPassState(buf);
        
        // backface cull
        tPass.setPassState(cs);
        
        // Add a terrain model to the pass
        tPass.addTerrain(page);
        
        tPass.setTileScale(90);
        
        // Add the tiles
        tPass.addDetail(basetex,null);
        tPass.addDetail(detailtex0,alphatex0);
        tPass.addDetail(detailtex1,alphatex1);
        tPass.addDetail(detailtex2,alphatex2);
        tPass.addDetail(detailtex3,alphatex3);
        tPass.setLightmap(lightmap, CombinerScale.Two);
        
        page.updateRenderState();
        
        pManager.add(tPass);
        
        // Render FPS
        RenderPass fpsPass = new RenderPass();
        //fpsPass.add(fpsNode);
        pManager.add(fpsPass);
        
        cam.setLocation(new Vector3f(-401.23486f, 143.13504f, -47.419296f));
        cam.lookAt(page.getWorldTranslation(),Vector3f.UNIT_Y);
        cam.update();
        
    }
   
    @Override
    public void simpleUpdate(){
    }
}
