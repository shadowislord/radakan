package jmetest.simplephysics;

import com.jme.app.SimpleGame;
import com.jme.bounding.BoundingBox;
import com.jme.input.KeyInput;
import com.jme.light.PointLight;
import com.jme.math.FastMath;
import com.jme.math.Matrix3f;
import com.jme.math.Quaternion;
import com.jme.math.Vector3f;
import com.jme.renderer.ColorRGBA;
import com.jme.scene.Node;
import com.jme.scene.SharedMesh;
import com.jme.scene.shape.Box;
import com.jme.scene.shape.Dodecahedron;
import com.jme.scene.shape.Octahedron;
import com.jme.scene.shape.Sphere;
import com.jme.scene.state.CullState;
import com.jme.scene.state.CullState.Face;
import com.jmex.simplephysics.Collider;
import com.jmex.simplephysics.CollisionListener;
import com.jmex.simplephysics.CollisionScene;
import com.jmex.simplephysics.DynamicCollider;
import com.jmex.simplephysics.EllipseCollider;
import com.jmex.simplephysics.StaticCollider;
import com.jmex.terrain.TerrainBlock;
import com.jmex.terrain.util.MidPointHeightMap;

/**
 * Demonstrates the use of the Simple Physics package.<br>
 * This demo is based on the TestObjectWalking demonstration.
 * 
 * @author Lucas Goraieb
 */
public class SimplePhysicsTest extends SimpleGame {

    Node pickNode;
    private CollisionScene collisionScene;
    private DynamicCollider playerCollider,  walkingCollider,  ballCollider;
    private Collider lastCollided = null;
    private Vector3f gravity = new Vector3f(0, -900, 0);

    public static void main(String[] args) {
        SimplePhysicsTest app = new SimplePhysicsTest();
        app.setConfigShowMode(ConfigShowMode.ShowIfNoConfig);
        app.start();
    }

    protected void createScene(){
        CullState cs = display.getRenderer().createCullState();
        cs.setCullFace(Face.Back);
        cs.setEnabled(true);

        PointLight pl = new PointLight();
        pl.setDiffuse(new ColorRGBA(0.4f, 0.4f, 0.4f, 1));
        pl.setEnabled(true);
        pl.setLocation(new Vector3f(500, 1500, 500));
        lightState.attach(pl);
    }
    
    @Override
    protected void simpleInitGame() {

        //Create a normal scene
        key = KeyInput.get();

        createScene();

        MidPointHeightMap heightMap = new MidPointHeightMap(64, 1.9f);
        Vector3f terrainScale = new Vector3f(20, .5f, 20);
        TerrainBlock terrain = new TerrainBlock("Terrain", heightMap.getSize(), terrainScale,
                heightMap.getHeightMap(),
                new Vector3f(0, 0, 0));
        terrain.setModelBound(new BoundingBox());
        terrain.updateModelBound();

        Box b = new Box("b", new Vector3f(0, 0, 0), 10, 10, 10);

        SharedMesh sb = new SharedMesh("Shared box", b);
        sb.setModelBound(new BoundingBox());
        sb.updateModelBound();
        sb.setLocalTranslation(new Vector3f(100, terrain.getHeight(100, 200), 200));

        SharedMesh sb2 = new SharedMesh("Shared box2", b);
        sb2.setModelBound(new BoundingBox());
        sb2.updateModelBound();
        Quaternion quat = new Quaternion();
        quat.fromAngleAxis(3, new Vector3f(1, 0, 0));
        sb2.setLocalRotation(quat);
        sb2.setLocalTranslation(new Vector3f(150, terrain.getHeight(150, 100), 100));


        Octahedron o = new Octahedron("o", 10);
        o.setModelBound(new BoundingBox());
        o.updateModelBound();
        o.setLocalTranslation(new Vector3f(100, terrain.getHeight(100, 150), 150));
        Dodecahedron d = new Dodecahedron("d", 10);
        Sphere s = new Sphere("sphere", 25, 25, 10);

        SharedMesh sm3 = new SharedMesh("Shared d1", d);
        sm3.setModelBound(new BoundingBox());
        sm3.updateModelBound();
        sm3.setLocalTranslation(new Vector3f(150, terrain.getHeight(150, 150), 150));

        SharedMesh sm4 = new SharedMesh("Shared d2", d);
        sm4.setModelBound(new BoundingBox());
        sm4.updateModelBound();
        sm4.setLocalTranslation(new Vector3f(150, terrain.getHeight(150, 200), 200));
        Quaternion qt = new Quaternion();
        qt.fromAngleAxis(2, new Vector3f(1, 0, 1));
        sm4.setLocalRotation(qt);

        SharedMesh sm = new SharedMesh("Shared Sphere1", s);
        sm.setModelBound(new BoundingBox());
        sm.updateModelBound();
        sm.setLocalTranslation(new Vector3f(100, terrain.getHeight(100, 50), 50));

        SharedMesh sm2 = new SharedMesh("Shared Sphere2", s);
        sm2.setModelBound(new BoundingBox());
        sm2.updateModelBound();
        sm2.setLocalTranslation(new Vector3f(50, terrain.getHeight(50, 100), 100));

        Sphere s2 = new Sphere("sphere2", 20, 20, 10);
        s2.setModelBound(new BoundingBox());
        s2.updateModelBound();
        s2.setLocalTranslation(new Vector3f(200, terrain.getHeight(200, 50), 50));
        s2.setLocalScale(2);

        Box bridge = new Box("Bridge", new Vector3f(0, 0, 0), 100, 5, 10);
        bridge.setModelBound(new BoundingBox());
        bridge.updateModelBound();
        bridge.setLocalTranslation(new Vector3f(100, terrain.getHeight(100, 50) + 5, 50));

        //Create the colliders
        Sphere walking = new Sphere("walking", 20, 20, 1);
        walking.setLocalScale(new Vector3f(2, 3, 2));
        walkingCollider = new EllipseCollider(new Vector3f(100, terrain.getHeight(100, 150) + 3, 150), walking.getLocalScale(), walking);
        walkingCollider.getPhysicMaterial().set(0.05f, 2, 0.7f, 0.05f, true);
        walkingCollider.getPhysicState().setGravity(gravity);

        Sphere ball = new Sphere("ball", 20, 20, 1);
        ball.setLocalScale(4);
        ballCollider = new EllipseCollider(new Vector3f(100, terrain.getHeight(100, 50) + 25, 50), ball.getLocalScale(), ball);
        ballCollider.getPhysicState().setGravity(gravity);
        ballCollider.getPhysicMaterial().set(0.01f, 1, 1, 0.5f, true);

        playerCollider = new EllipseCollider(new Vector3f(50, terrain.getHeight(50, 50) + 25, 50), new Vector3f(2, 3, 2), null);
        playerCollider.getPhysicMaterial().set(0.05f, 1, 0.7f, 0.05f, true);
        playerCollider.getPhysicState().setGravity(gravity);

        //Build the collision scene
        collisionScene = new CollisionScene();
        collisionScene.attachDynamic(walkingCollider);
        collisionScene.attachDynamic(ballCollider);
        collisionScene.attachDynamic(playerCollider);
        //Objects won't bounce when hitting the terrain, but will when hitting other statics (bounce is enabled by default)
        collisionScene.attachStatic(new StaticCollider(terrain, 100)).getPhysicMaterial().set(.01f, 0, false);
        collisionScene.attachStatic(new StaticCollider(sb, 25));
        collisionScene.attachStatic(new StaticCollider(sb2, 25));
        collisionScene.attachStatic(new StaticCollider(o, 25)).getPhysicMaterial().set(0.3f, 0, false); //No bounce here to

        collisionScene.attachStatic(new StaticCollider(sm3, 25));
        collisionScene.attachStatic(new StaticCollider(sm4, 25));
        collisionScene.attachStatic(new StaticCollider(sm2, 25));
        collisionScene.attachStatic(new StaticCollider(sm, 25));
        collisionScene.attachStatic(new StaticCollider(s2, 25));
        collisionScene.attachStatic(new StaticCollider(bridge, 25));
        System.out.println("Building octree, wait a minute...");
        collisionScene.build(); //Build octrees (may take a while)

        //Let's try a collision listener
        ballCollider.addListener(new CollisionListener() {

            public void collided(Collider collider, Collider collided, Vector3f position, Vector3f normal, Vector3f velocity) {
                if (lastCollided != collided) {
                    lastCollided = collided;
                } else {
                    return;
                }
                if (collider.getSpatial() == null) {
                    System.out.println("Camera collided with " + collided.getSpatial().getName());
                } else if (collided.getSpatial() == null) {
                    System.out.println(collider.getSpatial().getName() + " collided with Camera");
                } else {
                    System.out.println(collider.getSpatial().getName() + " collided with " + collided.getSpatial().getName());
                }
            }
        });

        //Build the rendering scene
        pickNode = new Node("Pick");
        pickNode.attachChild(terrain);
        pickNode.attachChild(sb);
        pickNode.attachChild(sb2);
        pickNode.attachChild(o);
        pickNode.attachChild(sm3);
        pickNode.attachChild(sm4);
        pickNode.attachChild(sm2);
        pickNode.attachChild(sm);
        pickNode.attachChild(s2);
        pickNode.attachChild(bridge);
        pickNode.updateGeometricState(0, true);
        rootNode.attachChild(pickNode);

        rootNode.attachChild(walking);
        rootNode.attachChild(ball);

        pickNode.lock();
    }
    private KeyInput key;
    private Vector3f walkForce = new Vector3f(100, 0, 0);
    private Vector3f walkAxis = new Vector3f(0, 1, 0);
    private Vector3f force = new Vector3f();
    private Matrix3f rotm = new Matrix3f();
    private float angle = 0;
    private float forceFactor = 250;

    protected void simpleUpdate() {
        force.zero();
        if (key.isKeyDown(KeyInput.KEY_W)) {
            force.z = forceFactor;
        }
        if (key.isKeyDown(KeyInput.KEY_S)) {
            force.z = -forceFactor;
        }
        if (key.isKeyDown(KeyInput.KEY_A)) {
            force.y = forceFactor;
        }
        if (key.isKeyDown(KeyInput.KEY_D)) {
            force.y = -forceFactor;
        }
        if (key.isKeyDown(KeyInput.KEY_SPACE)) {
            force.x = forceFactor * 10;
        }
        if (key.isKeyDown(KeyInput.KEY_F)) {
            force.x = -forceFactor;
        }
        //Add a rotate force based on the cameras direction
        rotm.fromAxes(walkAxis, cam.getLeft(), cam.getDirection());
        rotm.mult(force, force);
        playerCollider.getPhysicState().setForce(force);

        //Adds a rotate force to walk in circles
        angle += (tpf * 0.2) % 1;
        rotm.fromAngleAxis(angle * FastMath.PI, walkAxis);
        rotm.mult(walkForce, force);
        walkingCollider.getPhysicState().setForce(force);

        long startTime = System.nanoTime();
        //Do the magic
        collisionScene.update(tpf);
        long stopTime = System.nanoTime();
        float result = (stopTime - startTime) * 0.00001f;

        //Update the camera's location
        cam.getLocation().set(playerCollider.getLocation());
        cam.update();

        String s = "Calc time: %.4f";
        display.setTitle(String.format(s, result));
    }
}
