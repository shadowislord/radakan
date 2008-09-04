package com.radakan.graphics.util;

import java.util.ArrayList;

import org.lwjgl.opengl.ARBDepthTexture;
import org.lwjgl.opengl.ARBShadow;
import org.lwjgl.opengl.GL11;

import com.jme.image.Texture;
import com.jme.image.Texture2D;
import com.jme.light.DirectionalLight;
import com.jme.math.Matrix4f;
import com.jme.math.Vector3f;
import com.jme.renderer.AbstractCamera;
import com.jme.renderer.ColorRGBA;
import com.jme.renderer.Renderer;
import com.jme.renderer.TextureRenderer;
import com.jme.renderer.pass.Pass;
import com.jme.scene.Spatial;
import com.jme.scene.state.BlendState;
import com.jme.scene.state.ClipState;
import com.jme.scene.state.ColorMaskState;
import com.jme.scene.state.CullState;
import com.jme.scene.state.LightState;
import com.jme.scene.state.MaterialState;
import com.jme.scene.state.RenderState;
import com.jme.scene.state.TextureState;
import com.jme.system.DisplaySystem;

/**
 * A pass providing a shadow mapping layer across the top of an existing scene.
 * 
 * Based on code by Robert Larsson and Joshua Slack
 * @author kevglass
 */
public class DirectionalShadowMapPass extends Pass {
	/** Bias matrix borrowed from the projected texture utility */
    private static Matrix4f biasMatrix = new Matrix4f(0.5f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f, 0.5f, 0.5f, 0.5f,
            1.0f); // bias from [-1, 1] to [0, 1]
	
    /** The renderer used to produce the shadow map */
	private TextureRenderer shadowMapRenderer;
	/** The texture storing the shadow map */
	private Texture2D shadowMapTexture;
	/** The near plane when rendering the shadow map */
	private float nearPlane = 1f;
	/** The far plane when rendering the shadow map - currently tuned for the test*/
	private float farPlane = 3000.0f;
	/** The location the shadow light source is looking at - must point at the focus of the scene */
	private Vector3f shadowCameraLookAt;
	/** The effective location of the light source - derived based on the distance of casting, look at and direction */
	private Vector3f shadowCameraLocation;
	
	/** The list of occluding nodes */
	private ArrayList<Spatial> occluderNodes = new ArrayList<Spatial>();
	
	/** Culling front faces when rendering shadow maps */
	private CullState cullFrontFace;
	/** Turn off textures when rendering shadow maps */
	private TextureState noTexture;
	/** Turn off colours when rendering shadow maps - depth only */
	private ColorMaskState colorDisabled;
	/** Turn off lighting when rendering shadow maps - depth only */
	private LightState noLights;
	
	/** The blending to both discard the fragements that have been determined to be free of shadows and to blend into the background scene */
	private BlendState discardShadowFragments;
	/** The state applying the shadow map */
	private TextureState shadowTextureState;
	/** The bright light used to blend the shadows version into the scene */
	private LightState brightLights;
	/** The dark material used to blend the shadows into the scene */
	private MaterialState darkMaterial;
	/** Don't perform any plane clipping when rendering the shadowed scene */
	private ClipState noClip;
	
	/** True once the pass has been initialised */
	protected boolean initialised = false;
	/** The direction shadows are being cast from - directional light? */
	protected Vector3f direction;
	/** The size of the shadow map texture */
	private int shadowMapSize;
	/** 
	 * The scaling applied to the shadow map when rendered to - lower number means 
	 * higher res but less ara covered by the shadow map
	 */
	protected float shadowMapScale = .4f;
	/** 
	 * The distance we're modelling the direction light source as being away from the focal point, again 
	 * the higher the number the more of the scene is covered but at lower resolution 
	 */
	protected float dis = 500;
	
	/** A place to internally save previous enforced states setup before rendering this pass */
	private RenderState[] preStates = new RenderState[RenderState.RS_MAX_STATE];
	
	/** The colour of shadows cast */
	private ColorRGBA shadowCol = new ColorRGBA(0,0,0,0.3f);
	
	/**
	 * Create a shadow map pass casting shadows from a light with the direction
	 * given.
	 * 
	 * @param direction The direction of the light casting the shadows
	 */
	public DirectionalShadowMapPass(Vector3f direction) {
		this(direction, 2048);
	}

	/**
	 * Create a shadow map pass casting shadows from a light with the direction
	 * given.
	 * 
	 * @param shadowMapSize The size of the shadow map texture
	 * @param direction The direction of the light casting the shadows
	 */
	public DirectionalShadowMapPass(Vector3f direction, int shadowMapSize) {
		this.shadowMapSize = shadowMapSize;
		this.direction = direction;
		
		setViewTarget(new Vector3f(0,0,0));
	}
	
	/**
	 * Set the colour of the shadows to be cast
	 * 
	 * @param col The colour of the shadows to be cast
	 */
	public void setShadowAlpha(float alpha) {
		shadowCol.a = alpha;
		if (darkMaterial != null) {
			darkMaterial.setDiffuse(shadowCol);
		}
	}
	
	/**
	 * Set the distance of the camera representing the directional light. The further
	 * away the more of the scene will be shadowed but at a lower resolution
	 * 
	 * @param dis The distance to be used for the shadow map camera (default = 500)
	 */
	public void setViewDistance(float dis) {
		this.dis = dis;
	}
	
	/**
	 * Set the scale factor thats used to stretch the shadow map
	 * texture across the scene.
	 * 
	 * Higher the number the more of the scene will be convered but at
	 * a lower resolution.
	 * 
	 * @param scale The scale used to stretch the shadow map across the scene.
	 */
	public void setShadowMapScale(float scale) {
		shadowMapScale = scale;
	}
	
	/**
	 * Set the target of the view. This will be where the camera points 
	 * when generating the shadow map and should be the centre of the scene
	 * 
	 * @param target The target of the view 
	 */
	public void setViewTarget(Vector3f target) {
		if (target.equals(shadowCameraLookAt)) {
			return;
		}
		
		shadowCameraLookAt = new Vector3f(target);
		Vector3f temp = new Vector3f(direction);
		temp.normalizeLocal();
		temp.multLocal(-dis);
		shadowCameraLocation = new Vector3f(target);
		shadowCameraLocation.addLocal(temp);

		if (shadowMapRenderer != null) {
			updateShadowCamera();
		}
	}
	
	/**
	 * saves any states enforced by the user for replacement at the end of the
	 * pass.
	 */
	protected void saveEnforcedStates() {
		for (int x = RenderState.RS_MAX_STATE; --x >= 0;) {
			preStates[x] = context.enforcedStateList[x];
		}
	}

	/**
	 * replaces any states enforced by the user at the end of the pass.
	 */
	protected void replaceEnforcedStates() {
		for (int x = RenderState.RS_MAX_STATE; --x >= 0;) {
			context.enforcedStateList[x] = preStates[x];
		}
	}

	/**
	 * Add a spatial that will occlude light and hence cast a shadow
	 * 
	 * @param occluder The spatial to add as an occluder
	 */
	public void addOccluder(Spatial occluder) {
		occluderNodes.add(occluder);
	}
	
	/**
	 * Initialise the pass render states
	 */
	public void init(Renderer r) {
		if (initialised) {
			return;
		}

		initialised = true; // now it's initialised

		// the texture that the shadow map will be rendered into. Modulated so 
		// that it can be blended over the scene. 
		shadowMapTexture = new Texture2D();
		shadowMapTexture.setApply(Texture.ApplyMode.Modulate);
		shadowMapTexture.setMinificationFilter(Texture.MinificationFilter.NearestNeighborNoMipMaps); 
		shadowMapTexture.setWrap(Texture.WrapMode.Clamp); 
		shadowMapTexture.setMagnificationFilter(Texture.MagnificationFilter.Bilinear);
		shadowMapTexture.setRenderToTextureType(Texture.RenderToTextureType.Depth);
		shadowMapTexture.setMatrix(new Matrix4f());
		shadowMapTexture.setEnvironmentalMapMode(Texture.EnvironmentalMapMode.EyeLinear);
		
		// configure the texture renderer to output to the texture
		shadowMapRenderer = DisplaySystem.getDisplaySystem().createTextureRenderer(shadowMapSize, shadowMapSize,TextureRenderer.Target.Texture2D);
		shadowMapRenderer.setupTexture(shadowMapTexture); 

		// render state to apply the shadow map texture
		shadowTextureState = r.createTextureState();
		shadowTextureState.setTexture(shadowMapTexture, 0);
		
		noClip = r.createClipState();
		noClip.setEnabled(false);
        
		// render states to use when rendering into the shadmop, no textures or colours 
		// are required since we're only interested in recording depth
		// Also only need back faces when rendering the shadow maps
		noTexture = r.createTextureState();
		noTexture.setEnabled(false); 
		colorDisabled = r.createColorMaskState();
		colorDisabled.setAll(false); 
		cullFrontFace = r.createCullState();
		cullFrontFace.setEnabled(true);
		cullFrontFace.setCullFace(CullState.Face.Front);
		noLights = r.createLightState();
		noLights.setEnabled(false);
		
		// Then rendering and comparing the shadow map with the current
		// depth the result will be set to alpha 1 if not in shadow and
		// to 0 if it's is in shadow. However, we're going to blend it into the scene
		// so the alpha will be zero if there is no shadow at this location but
		// > 0 on shadows.
		discardShadowFragments = r.createBlendState();
		discardShadowFragments.setEnabled(true);
		discardShadowFragments.setBlendEnabled(true);
		discardShadowFragments.setSourceFunction(BlendState.SourceFunction.SourceAlpha);
		discardShadowFragments.setDestinationFunction(BlendState.DestinationFunction.OneMinusSourceAlpha);
		discardShadowFragments.setTestEnabled(true);
		discardShadowFragments.setReference(0f);
		discardShadowFragments.setTestFunction(BlendState.TestFunction.GreaterThan); 
		
		// light used to uniformly light the scene when rendering the shadows themselfs
		// this is so the geometry colour can be used as the source for blending - i.e.
		// transparent shadows rather than matte black
		brightLights = r.createLightState();
		brightLights.setEnabled(true);
        DirectionalLight light = new DirectionalLight();
        light.setDiffuse(new ColorRGBA(1, 1, 1, 1f));
        light.setEnabled(true);
        brightLights.attach(light);
        
		darkMaterial = r.createMaterialState();
		darkMaterial.setEnabled(true);
		darkMaterial.setDiffuse(shadowCol);
		darkMaterial.setAmbient(new ColorRGBA(0,0,0,0f));
		darkMaterial.setShininess(0);
		darkMaterial.setSpecular(new ColorRGBA(0,0,0,0));
		darkMaterial.setEmissive(new ColorRGBA(0,0,0,0));
		darkMaterial.setMaterialFace(MaterialState.MaterialFace.Front);
		
		updateShadowCamera();
	}
	
	/**
	 * @see com.jme.renderer.pass.Pass#doRender(com.jme.renderer.Renderer)
	 */
	public void doRender(Renderer r) {
		if (occluderNodes.size() == 0) {
			return;
		}
		
		init(r); 
		updateShadowMap(r);
		renderShadowedScene(r);
	}
	
	/**
	 * Render the scene with shadows
	 * 
	 * @param r The renderer to use
	 */
	protected void renderShadowedScene(Renderer r) {
		saveEnforcedStates();
		context.enforceState(shadowTextureState);
		context.enforceState(discardShadowFragments);
		context.enforceState(brightLights);
		context.enforceState(darkMaterial);
		
		// compare the shadowmap depth wich the current fragment depth
		// this needs to be moved into JME texture class, not sure where yet?
		GL11.glTexParameteri(GL11.GL_TEXTURE_2D, ARBShadow.GL_TEXTURE_COMPARE_MODE_ARB, ARBShadow.GL_COMPARE_R_TO_TEXTURE_ARB);
		GL11.glTexParameteri(GL11.GL_TEXTURE_2D, ARBShadow.GL_TEXTURE_COMPARE_FUNC_ARB, GL11.GL_GEQUAL);
		GL11.glTexParameteri(GL11.GL_TEXTURE_2D, ARBDepthTexture.GL_DEPTH_TEXTURE_MODE_ARB, GL11.GL_INTENSITY);
		
		// draw the scene, only the shadowed bits will be drawn and blended
		// with the shadow coloured geometry
		r.setPolygonOffset(0, -5);
		for (Spatial spat : spatials) {
			spat.onDraw(r);
		}
		r.renderQueue();
		
		r.clearPolygonOffset();
		replaceEnforcedStates();
	}

	/**
	 * Update the shadow map
	 * 
	 * @param r The renderer to being use to display this map
	 */
	protected void updateShadowMap(Renderer r) {
		saveEnforcedStates();
		context.enforceState(noClip);
		context.enforceState(noTexture);
		context.enforceState(colorDisabled); 
		context.enforceState(cullFrontFace);
		context.enforceState(noLights);
	
		r.setPolygonOffset(0, 5); 
		shadowMapRenderer.render(occluderNodes.get(0), shadowMapTexture, true);
		for (int i=1;i<occluderNodes.size();i++) {
			shadowMapRenderer.render(occluderNodes.get(i), shadowMapTexture, false);
		}
		r.clearPolygonOffset();
		replaceEnforcedStates();
	}
	
	/**
	 * Update the direction from which the shadows are cast
	 */
	protected void updateShadowCamera() {
		// render the shadow map, use the texture renderer to render anything
		// thats been added as occluder
		float scale = shadowMapSize * shadowMapScale;

		shadowMapRenderer.getCamera().setLocation(shadowCameraLocation);
		shadowMapRenderer.getCamera().setFrustum(nearPlane, farPlane, -scale, scale, -scale, scale);
		shadowMapRenderer.getCamera().lookAt(shadowCameraLookAt, Vector3f.UNIT_Y.clone());
		shadowMapRenderer.getCamera().setParallelProjection(true);
		shadowMapRenderer.getCamera().update();

		Matrix4f proj = new Matrix4f();
		Matrix4f view = new Matrix4f();
		proj.set(((AbstractCamera) shadowMapRenderer.getCamera()).getProjectionMatrix());
		view.set(((AbstractCamera) shadowMapRenderer.getCamera()).getModelViewMatrix());
		shadowMapTexture.getMatrix().set(view.multLocal(proj).multLocal(biasMatrix)).transposeLocal();
	}
	
	/**
	 * @see com.jme.renderer.pass.Pass#cleanUp()
	 */
	public void cleanUp() {
		super.cleanUp();
		
		if (shadowMapRenderer != null) {
			shadowMapRenderer.cleanup();
		}
	}
	
	/**
	 * Remove the contents of the pass
	 */
	public void clear() {
		occluderNodes.clear();
		spatials.clear();
	}
}