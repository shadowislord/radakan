package jmetest.intersection.octree;

import com.jme.bounding.BoundingBox;
import com.jme.renderer.ColorRGBA;
import com.jme.scene.Node;
import com.jme.scene.Spatial;
import com.jme.scene.Spatial.CullHint;
import com.jme.scene.shape.Box;
import com.jme.scene.state.WireframeState;
import com.jme.system.DisplaySystem;

/**
 * <code>OctreeDebugger</code> provides an singleton to make Debug Helpers.
 * 
 * @author Lucas Goraieb
 * @version $Id: Octree.java,v 0.5 2007/04/27 20:33:02 nca Exp $
 */
public class OctreeDebugger {

    private static OctreeDebugger instance;

    public static OctreeDebugger getInstance() {
        if (instance == null) {
            instance = new OctreeDebugger();
        }
        return instance;
    }

    public static void initialize(DisplaySystem display, Node rootNode) {
        OctreeDebugger o = getInstance();
        o.display = display;
        o.rootNode = rootNode;
        o.debugNode = new Node();
        o.rootNode.attachChild(o.debugNode);
    }
    public DisplaySystem display;
    private Node rootNode;
    private Node debugNode;

    public Box addBox(BoundingBox bb) {
        Box box = new Box("b", bb.getCenter(), bb.xExtent, bb.yExtent, bb.zExtent);
        WireframeState ws = display.getRenderer().createWireframeState();
        ws.setEnabled(true);
        box.setRenderState(ws);
        box.setDefaultColor(ColorRGBA.white);// ColorRGBA.randomColor());

        debugNode.attachChild(box);
        return box;
    }

    public void hit(DebugOctree node) {
        if (node.getTriangleData() == null) {
            node.getDebugBox().setDefaultColor(ColorRGBA.white);
        } else {
            node.getDebugBox().setDefaultColor(ColorRGBA.red);
        }
        node.getDebugBox().setCullHint(CullHint.Never);
    }

    public void reset() {
        for (Spatial c : debugNode.getChildren()) {
            if (c instanceof Box) {
                ((Box) c).setCullHint(CullHint.Always);
            }
        }
    }
}
