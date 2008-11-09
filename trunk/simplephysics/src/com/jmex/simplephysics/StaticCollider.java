package com.jmex.simplephysics;

import java.util.List;
import java.util.Set;

import com.jme.bounding.BoundingVolume;
import com.jme.bounding.Octree;
import com.jme.math.Vector3f;
import com.jme.scene.TriMesh;

/**
 * <code>StaticCollider</code> provides an implementation of a Collider to be used for static elements.<br>
 * Like walls, maps, terrains etc.
 * 
 * @author Lucas Goraieb
 */
public class StaticCollider extends Collider {

    private PhysicMaterial physicMaterial = new PhysicMaterial();
    private Octree octree;

    /**
     * Creates a static collider using a TriMesh as source.
     * @param mesh TriMesh source
     * @param trianglesPerNode Max number of triangles in one octree node. Depends on the mesh's size and geometry.
     */
    public StaticCollider(TriMesh mesh, int trianglesPerNode) {
        this.spatial = mesh;
        octree = new Octree(mesh, trianglesPerNode);
    }

    /**
     * Build the octrees. This is called automatically by the CollisionScene.build() method.
     */
    public void build() {
        octree.build();
    }

    protected void intersect(BoundingVolume bv, List<CollisionResult> collisionResult) {
        Set<Integer> triList = octree.intersect(bv);
        if (triList.size() > 0) {
            collisionResult.add(new CollisionResult(this, octree.getMesh(), triList));
        }
    }

    public PhysicMaterial getPhysicMaterial() {
        return physicMaterial;
    }

    /**
     * Get the collider's Mesh
     * @return Collider's mesh
     */
    public TriMesh getMesh() {
        return (TriMesh) spatial;
    }

    @Override
    protected Vector3f scaleFromSpace(Vector3f vector) {
        return vector;
    }

    @Override
    protected Vector3f scaleToSpace(Vector3f vector) {
        return vector;
    }
}
