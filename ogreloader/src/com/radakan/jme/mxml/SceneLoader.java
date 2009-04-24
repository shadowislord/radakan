/*
 * Copyright (c) 2008, OgreLoader
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

package com.radakan.jme.mxml;

import com.jme.light.DirectionalLight;
import com.jme.light.Light;
import com.jme.light.PointLight;
import com.jme.math.Matrix3f;
import com.jme.math.Quaternion;
import com.jme.math.Vector3f;
import com.jme.renderer.Camera;
import com.jme.renderer.ColorRGBA;
import com.jme.scene.Spatial;
import com.jme.scene.state.LightState;
import com.jme.system.DisplaySystem;
import com.jme.util.resource.ResourceLocatorTool;
import com.jme.util.resource.ResourceLocator;
import java.io.IOException;
import java.io.InputStream;
import java.net.URL;
import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.Callable;
import java.net.URI;
import java.net.URL;
import java.net.URLEncoder;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

import static com.radakan.util.XMLUtil.*;

public class SceneLoader {

    private Map<String, Material> materials = new HashMap<String, Material>();
    
    private LightState ls;
    private com.jme.scene.Node scene;
    private Vector3f[] temp = new Vector3f[3];
    private Camera lastLoadedCamera;
    
    public SceneLoader(){
        ls = DisplaySystem.getDisplaySystem().getRenderer().createLightState();
        scene = new com.jme.scene.Node("OgreScene");
        scene.setRenderState(ls);
    }
    
    public Camera getCamera(){
        return lastLoadedCamera;
    }
    
    /*
      <camera name="Camera.001" fov="32.642063" projectionType="perspective">
        <clipping nearPlaneDist="0.100000" farPlaneDist="100.000000"/>
      </camera>
    */
    /*
    private Camera loadCamera(Node camera, Vector3f pos, Quaternion rot){
        
        final DisplaySystem display = DisplaySystem.getDisplaySystem();
        
        Callable<Camera> exe = new Callable<Camera>(){
            public Camera call(){
                return display.getRenderer().createCamera(display.getWidth(), display.getHeight());
            }
        };
        Camera cam = JmeContext.get().execute(exe);
        cam.setFrame(pos, rot);
        
        String name = getAttribute(camera, "name");
        if (getAttribute(camera, "projectionType").equalsIgnoreCase("perspective")){
            float fov = getFloatAttribute(camera, "fov", 45f);
            Node clipping = getChildNode(camera, "clipping");
            float nearPlane = getFloatAttribute(clipping, "nearPlaneDist", 1f);
            float farPlane = getFloatAttribute(clipping, "farPlaneDist", 1000f);
            
            cam.setFrustumPerspective(fov, 
                                     (float) display.getWidth() / display.getHeight(), 
                                     nearPlane, 
                                     farPlane);
        }
        
        return cam;
    }*/
    
    private Light loadLight(Node light, Vector3f pos, Quaternion rot){
        String type = getAttribute(light, "type");
        Light l = null;
        
        if (type.equals("directional")){
            //Vector3f dir = loadVector3(getChildNode(light, "normal"));
            
            DirectionalLight dl = new DirectionalLight();
            dl.setEnabled(true);
            Matrix3f tempMat = rot.toRotationMatrix();
            dl.setDirection(tempMat.getColumn(2));
            l = dl;
        }else if (type.equals("point")){
            PointLight pl = new PointLight();
            pl.setEnabled(true);
            pl.setLocation(pos);
            l = pl;
            
            Node att = getChildNode(light, "lightAttenuation");
            if (att != null){
                pl.setAttenuate(true);
                pl.setConstant(getFloatAttribute(att, "constant"));
                pl.setLinear(getFloatAttribute(att, "linear"));
                pl.setQuadratic(getFloatAttribute(att, "quadratic"));
            }
        }else{
            System.out.println("UNSUPPORTED LIGHT: "+type);
        }
        
        Node diffuseNode = getChildNode(light, "colourDiffuse");
        if (diffuseNode != null)
            l.setDiffuse(loadColor(diffuseNode));

        Node specularNode = getChildNode(light, "colourSpecular");
        if (specularNode != null)
            l.setDiffuse(loadColor(specularNode));
        
        return l;
    }
    
    private Vector3f loadVector3(Node vector){
        Vector3f vec = new Vector3f();
        vec.x = getFloatAttribute(vector, "x");
        vec.y = getFloatAttribute(vector, "y");
        vec.z = getFloatAttribute(vector, "z");
        return vec;
    }
    
    private Quaternion loadQuaternion(Node quaternion){
        Quaternion q = new Quaternion();
        q.x = getFloatAttribute(quaternion, "x");
        q.y = getFloatAttribute(quaternion, "y");
        q.z = getFloatAttribute(quaternion, "z");
        q.w = getFloatAttribute(quaternion, "w");
        return q;
    }
    
    private ColorRGBA loadColor(Node color){
        ColorRGBA c = new ColorRGBA();
        c.r = getFloatAttribute(color, "r");
        c.g = getFloatAttribute(color, "g");
        c.b = getFloatAttribute(color, "b");
        return c;
    }
    
    public void loadNodes(Node nodes) throws IOException{
        Node node = nodes.getFirstChild();
        while (node != null){
            if (node.getNodeName().equals("node")){
                String name = getAttribute(node, "name");
                //System.out.println("ENTITY("+name+")");
                
                com.jme.scene.Node n = new com.jme.scene.Node(name);
                Vector3f pos   = loadVector3(getChildNode(node, "position"));
                Quaternion rot = loadQuaternion(getChildNode(node, "quaternion"));
                Vector3f scale = loadVector3(getChildNode(node, "scale"));
                
                n.setLocalTranslation(pos);
                n.setLocalRotation(rot);
                n.setLocalScale(scale);
                
                Node entity = getChildNode(node, "entity");
                if (entity != null){
                    URL url = ResourceLocatorTool.locateResource(ResourceLocatorTool.TYPE_MODEL,
                                                                 getAttribute(entity, "meshFile") + ".xml");
                    
                    if (url != null){
                        OgreLoader loader = new OgreLoader();
                        loader.setMaterials(materials);
                        Spatial spatial = loader.loadModel(url);

                        n.attachChild(spatial);
                    }
                }
                
                Node light = getChildNode(node, "light");
                if (light != null){
                    ls.attach(loadLight(light, pos, rot));
                }
                
//                Node camera = getChildNode(node, "camera");
//                if (camera != null){
//                    lastLoadedCamera = loadCamera(camera, pos, rot);
//                }
                
                scene.attachChild(n);
            }
            
            node = node.getNextSibling();
        }
    }
    
    public void loadExternals(Node externals) throws IOException{
        Node item = externals.getFirstChild();
        while (item != null){
            if (item.getNodeName().equals("item")){
                if (getAttribute(item, "type").equals("material")){
                    String file = getAttribute(getChildNode(item, "file"), "name");
                    MaterialLoader matloader = new MaterialLoader();
                    URL url = ResourceLocatorTool.locateResource(ResourceLocatorTool.TYPE_TEXTURE, file);
                    if (url != null){
                        InputStream in = url.openStream();
                        matloader.load(in);
                        in.close();
                        materials.putAll(matloader.getMaterials());
                    }
                }
            }
            
            item = item.getNextSibling();
        }
    }
    
    public void loadEnvironment(Node env){
        Node ambient = getChildNode(env, "colourAmbient");
        if (ambient != null){
            ls.setGlobalAmbient(loadColor(ambient));
        }
        
        Node background = getChildNode(env, "colourBackground");
        if (background != null){
            //DisplaySystem.getDisplaySystem().getRenderer().setBackgroundColor(loadColor(background));
        }
    }
    
    public void load(Node scene) throws IOException{
        String version = getAttribute(scene, "formatVersion");
        
        Node externals   = getChildNode(scene, "externals");
        Node nodes       = getChildNode(scene, "nodes");
        Node environment = getChildNode(scene, "environment");
        
        loadExternals(externals);
        loadEnvironment(environment);
        loadNodes(nodes);
    }
            
    public Spatial getScene(){
        return scene;
    }
    
    public void load(InputStream in) throws IOException{
        try {
            DocumentBuilder builder = DocumentBuilderFactory.newInstance().newDocumentBuilder();
            Document doc = builder.parse(in);

            NodeList list = doc.getElementsByTagName("scene");
            if (list.getLength() == 0) {
                return;
            }

            Node sceneNode = list.item(0);
            load(sceneNode);
            
            scene.updateGeometricState(0, true);
            scene.updateRenderState();
        } catch (ParserConfigurationException ex) {
            throw new IOException("Error configuring parser");
        } catch (SAXException ex) {
            throw new IOException("Error with xml parser");
        }
    }

    /**
     * Loads scene from dotScene file at specified URI, automatically adding
     * the containing directory to the resource locator paths for the duration
     * of the load.
     * <P>An example of invoking this method for a filesystem file:<CODE><PRE>
     *  ogreSceneLoader.load(file.toURI());
     *  </PRE></CODE>
     * </P>
     *
     * @see SceneLoader.RelativeResourceLocator
     */
    public void load(URI uri) throws IOException{
        ResourceLocator locator = new RelativeResourceLocator(uri);
        ResourceLocatorTool.addResourceLocator(
                ResourceLocatorTool.TYPE_TEXTURE, locator);
        ResourceLocatorTool.addResourceLocator(
                ResourceLocatorTool.TYPE_MODEL, locator);
        try {
            load(uri.toURL().openStream());
        } finally {
            ResourceLocatorTool.removeResourceLocator(
                    ResourceLocatorTool.TYPE_TEXTURE, locator);
            ResourceLocatorTool.removeResourceLocator(
                    ResourceLocatorTool.TYPE_MODEL, locator);
            locator = null;  // Just to encourage GC
        }
    }

    /**
     * A conservative ResourceLocator implementation that adds to the search
     * path just the parent directory of the specified URI, and it is  only
     * used for resources requested with relative paths.
     *
     * @see com.jme.util.resource.ResourceLocator
     */
    static public class RelativeResourceLocator implements ResourceLocator {
        private URI baseUri;

        public RelativeResourceLocator(URI baseUri) {
            this.baseUri = baseUri;
        }

        public URL locateResource(String resourceName) {
            if (baseUri == null || resourceName == null
                    || resourceName.length() < 2 || resourceName.charAt(0) == '/'
                    || resourceName.charAt(0) == '\\') return null;
            // No-op unless baseUri set for instance, and resourceName is relative.

            /* The remainder is the safe and conservative subset of code copied
             * from SimpleResourceLocator.locateResource(String). */

            try {
                String spec = URLEncoder.encode(resourceName, "UTF-8");
                //this fixes a bug in JRE1.5 (file handler does not decode "+" to
                //spaces)
                spec = spec.replaceAll("\\+", "%20");

                URL rVal = new URL(baseUri.toURL(), spec);
                // open a stream to see if this is a valid resource
                // XXX: Perhaps this is wasteful?  Also, what info will determine validity?
                rVal.openStream().close();
                return rVal;
            } catch (IOException e) {
            } catch (IllegalArgumentException e) {
            }
            return null;
        }
    }
}
