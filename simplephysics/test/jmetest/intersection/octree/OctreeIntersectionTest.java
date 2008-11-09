package jmetest.intersection.octree;

import java.nio.FloatBuffer;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;

import com.jme.animation.SpatialTransformer;
import com.jme.app.SimpleGame;
import com.jme.bounding.BoundingBox;
import com.jme.bounding.BoundingSphere;
import com.jme.bounding.Octree;
import com.jme.math.FastMath;
import com.jme.math.Quaternion;
import com.jme.math.Vector3f;
import com.jme.renderer.ColorRGBA;
import com.jme.scene.Controller;
import com.jme.scene.Node;
import com.jme.scene.TriMesh;
import com.jme.scene.shape.PQTorus;
import com.jme.scene.shape.Sphere;
import com.jme.util.geom.BufferUtils;

/**
 * Demonstrates an Octree, a DebugOctree and it's intersection methods.<br>
 * This demo is based on the TestCollisionTree demonstration.
 * 
 * @author Lucas Goraieb
 */
public class OctreeIntersectionTest extends SimpleGame {

    ColorRGBA[] colorSpread = {ColorRGBA.white, ColorRGBA.green,
        ColorRGBA.gray
    };
    ColorRGBA[] colorSpread2 = {ColorRGBA.blue, ColorRGBA.red,
        ColorRGBA.yellow
    };
    TriMesh s, r;
    Node n, m, t;
    int count = 0;
    List<Octree> octreeList = new ArrayList<Octree>();
    Map<Octree, Set<Integer>> storedTriangles = new HashMap<Octree, Set<Integer>>();

    public static void main(String[] args) {
        OctreeIntersectionTest app = new OctreeIntersectionTest();
        app.setConfigShowMode(ConfigShowMode.ShowIfNoConfig);
        app.start();
    }

    protected void simpleInitGame() {
        OctreeDebugger.initialize(display, rootNode);
        s = new Sphere("sphere", 10, 10, 1);

        s.setSolidColor(ColorRGBA.white);
        s.setModelBound(new BoundingSphere());
        s.updateModelBound();

        n = new Node("sphere node");

        r = new PQTorus("tort", 5, 4, 2f, .5f, 128, 16);
        r.setLocalTranslation(new Vector3f(1, 0, 0));
        r.setLocalRotation(new Quaternion().fromAngleNormalAxis(FastMath.PI * 0.25f, new Vector3f(0, 1f, 0)));
        r.setLocalScale(0.9f);
        r.setSolidColor(ColorRGBA.white);
        r.setModelBound(new BoundingBox());
        r.updateModelBound();

        m = new Node("tort node");

        SpatialTransformer st = new SpatialTransformer(1);
        st.setRepeatType(Controller.RT_CYCLE);
        st.setObject(m, 0, -1);
        st.setPosition(0, 0, new Vector3f(10, 10, 0));
        st.setPosition(0, 4, new Vector3f(-10, -10, 0));
        st.interpolateMissing();
        r.addController(st);

        FloatBuffer color1 = r.getColorBuffer();
        color1.clear();
        for (int i = 0, bLength = color1.capacity(); i < bLength; i += 4) {
            ColorRGBA c = colorSpread[i % 3];
            color1.put(c.r).put(c.g).put(c.b).put(c.a);
        }
        color1.flip();
        //Octree octree = new Octree(r, j,100); //Uncomment if you want to see without the debugger helpers
        Octree octree = new DebugOctree(r, 100);
        octree.build();
        octreeList.add(octree);


        FloatBuffer color2 = s.getColorBuffer();
        color2.clear();
        for (int i = 0, bLength = color2.capacity(); i < bLength; i += 4) {
            ColorRGBA c = colorSpread[i % 3];
            color2.put(c.r).put(c.g).put(c.b).put(c.a);
        }
        color2.flip();

        n.attachChild(r);
        m.attachChild(s);

        rootNode.attachChild(n);
        rootNode.attachChild(m);

        t = new Node();
        rootNode.attachChild(t);

        lightState.detachAll();
        lightState.setEnabled(false);
    }

    @Override
    protected void simpleUpdate() {
        count++;
        if (count < 5) {
            return;
        }
        count = 0;

        int[] indexBuffer = new int[3];

        int triCount = 0;

        for (Octree octree : octreeList) {

            Set<Integer> oldData = storedTriangles.get(octree);

            if (oldData != null) {
                for (Integer triIndex : oldData) {
                    octree.getMesh().getTriangle(triIndex, indexBuffer);
                    FloatBuffer color2 = octree.getMesh().getColorBuffer();
                    BufferUtils.setInBuffer(colorSpread[indexBuffer[0] % 3], color2, indexBuffer[0]);
                    BufferUtils.setInBuffer(colorSpread[indexBuffer[1] % 3], color2, indexBuffer[1]);
                    BufferUtils.setInBuffer(colorSpread[indexBuffer[2] % 3], color2, indexBuffer[2]);
                }
            }

            storedTriangles.clear();
            t.detachAllChildren();

            Set<Integer> triList = octree.intersect(s.getWorldBound());
            if (triList.size() > 0) {
                triCount += triList.size();
                storedTriangles.put(octree, triList);
                for (Integer triIndex : triList) {
                    octree.getMesh().getTriangle(triIndex, indexBuffer);
                    FloatBuffer color2 = octree.getMesh().getColorBuffer();
                    BufferUtils.setInBuffer(ColorRGBA.blue, color2, indexBuffer[0]);
                    BufferUtils.setInBuffer(ColorRGBA.blue, color2, indexBuffer[1]);
                    BufferUtils.setInBuffer(ColorRGBA.blue, color2, indexBuffer[2]);
                }
            }
        }
        display.setTitle("Triangles: " + triCount);
    }
}