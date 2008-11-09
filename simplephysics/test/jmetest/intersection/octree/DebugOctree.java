package jmetest.intersection.octree;

import java.util.List;



import com.jme.bounding.BoundingBox;
import com.jme.bounding.Octree;
import com.jme.scene.TriMesh;
import com.jme.scene.shape.Box;

/**
 * <code>DebugOctree</code> provides an Octree implementation with Debug Helpers.<br>
 * Requires OctreeDebugger class.
 * 
 * @author Lucas Goraieb
 * @version $Id: Octree.java,v 0.5 2007/04/27 20:33:02 nca Exp $
 */
public class DebugOctree extends Octree {

    private OctreeDebugger debug = OctreeDebugger.getInstance();
    private Box debugBox;

    public DebugOctree(TriMesh mesh, int trianglePerNode) {
        super(mesh, trianglePerNode);
        debugBox = debug.addBox(getBoundingBox());
    }

    private DebugOctree(DebugOctree parent, BoundingBox bb, List<Integer> tris) {
        super(parent, bb, tris);
        System.out.println("Node created. BoundingBox extent: " + getBoundingBox().xExtent);
        debugBox = debug.addBox(getBoundingBox());
    }

    @Override
    public void onSubdivide() {
        System.out.println("Subdividing triangles: " + getTriangleData().size());
    }

    @Override
    public void onRecurseFinished() {
        System.out.println("Recurse finished. Triangles: " + getTriangleData().size());
    }

    @Override
    public void onIntersectStart() {
        debug.reset();
    }

    @Override
    public void onIntersectHit() {
        debug.hit(this);
    }

    @Override
    protected Octree newOctreeNode(BoundingBox bb, List<Integer> tris) {
        return new DebugOctree(this, bb, tris);
    }

    public Box getDebugBox() {
        return debugBox;
    }

    @Override
    public List<Integer> getTriangleData() {
        return super.getTriangleData();
    }
}
