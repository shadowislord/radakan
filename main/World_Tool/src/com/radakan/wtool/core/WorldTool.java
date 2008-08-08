package com.radakan.wtool.core;

import java.awt.Cursor;
import java.awt.Image;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.URI;
import java.util.Collection;
import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.Callable;
import java.util.logging.Level;
import java.util.logging.Logger;

import javax.swing.DefaultComboBoxModel;
import javax.swing.DefaultListModel;
import javax.swing.Icon;
import javax.swing.ImageIcon;
import javax.swing.JFileChooser;
import javax.swing.JOptionPane;
import javax.swing.JPopupMenu;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.DefaultTreeModel;

import org.lwjgl.Sys;

import sun.management.FileSystem;
import sun.org.mozilla.javascript.internal.ErrorReporter;

import com.gibbon.jme.context.JmeContext;
import com.gibbon.jme.context.lwjgl.LWJGLCanvas;
import com.gibbon.jme.context.lwjgl.LWJGLContext;
import com.gibbon.jme.pass.RenderPass;
import com.gibbon.tools.FileNameExtensionFilter;
import com.gibbon.tools.world.Brush;
import com.gibbon.tools.world.BrushType;
import com.gibbon.tools.world.EditType;
import com.gibbon.tools.world.EditorState;
import com.gibbon.tools.world.EntityBrush;
import com.gibbon.tools.world.LafMenu;
import com.gibbon.tools.world.NewWorldDialog;
import com.gibbon.tools.world.SelectionEffectPass;
import com.gibbon.tools.world.TextureSet;
import com.gibbon.tools.world.TextureSetEditor;
import com.gibbon.tools.world.TextureSetLoader;
import com.gibbon.tools.world.TileBrush;
import com.gibbon.tools.world.UndoManager;
import com.gibbon.tools.world.World;
import com.gibbon.tools.world.WorldCameraHandler;
import com.gibbon.tools.world.TextureSet.Detailmap;
import com.jme.math.Vector3f;
import com.jme.renderer.AbstractCamera;
import com.jme.scene.Spatial;
import com.jme.util.TextureManager;
import com.jme.util.export.binary.BinaryExporter;
import com.jme.util.export.binary.BinaryImporter;
import com.jme.util.resource.ResourceLocatorTool;
import com.jme.util.resource.SimpleResourceLocator;
import com.sun.naming.internal.ResourceManager;

/**
 * Changes recently made by Tomygun:
 * -changed menuItems's variable name, some had genereic ones like save, save as, open.
 * includes usage of the ResourceManager and DefaultFileSystem to load icon Images.
 * can't use it on GUI because images wouldn't be shown on the Desing view.
 * -added Look & Feel menu.
 * -when new file is opened lastSavedFile is set to null; if "save" would have been presed
 * another file might be overridden
 * -added ".world" when a file is saved without extension
 * -some minor changes to some classes to work with newest jME.
 */
public class WorldTool extends javax.swing.JFrame {

	private static WorldTool instance;
	private EditorState state;
	private LWJGLCanvas glCanvas;
	private JmeContext context;
	private DefaultListModel textures;
	private DefaultComboBoxModel texturesets;
	private RenderPass render;
	private SelectionEffectPass selectionPass;
	private boolean doingMouseAction = false;
	private boolean mouseActionOnce = false;
	private boolean mouseActionEnd = false;
	private int mouseX = 0,  mouseY = 0;
	private JFileChooser chooser = new JFileChooser();
	private File lastSavedFile = null;

	public static int getMouseX() {
		return instance.mouseX;
	}

	public static int getMouseY() {
		return instance.mouseY;
	}

	public static void setMouseXY(int x, int y) {
		instance.mouseX = x;
		instance.mouseY = y;
	}

	public static void setDoingMouseAction(int x, int y) {
		instance.mouseX = x;
		instance.mouseY = y;
		instance.doingMouseAction = true;
		instance.mouseActionOnce = true;
	}

	public static void setNotDoingMouseAction() {
		instance.doingMouseAction = false;
		instance.mouseActionEnd = true;
	}

	public void importTextureSets(File f) {
		if (f.isDirectory()) {
			for (File ff : f.listFiles()) {
				importTextureSets(ff);
			}
			return;
		} else if (!f.getName().endsWith(".xml")) {
			return;
		}

		URI texDir = new File(f.getParent(), "/images/").toURI();
		SimpleResourceLocator tex = new SimpleResourceLocator(texDir);
		ResourceLocatorTool.addResourceLocator(ResourceLocatorTool.TYPE_TEXTURE, tex);
		try {
			InputStream in = new FileInputStream(f);
			TextureSet set = TextureSetLoader.load(in);
			texturesets.addElement(set);
			EditorState.texsetMap.put(set.toString(), set);
			in.close();
		} catch (IOException ex) {
			ex.printStackTrace();
		}

	// save locators in-case we have to load textures from the path again
	//ResourceLocatorTool.removeResourceLocator(ResourceLocatorTool.TYPE_TEXTURE, tex);
	}

	public void importTextureSets() {
		File f = new File(System.getProperty("user.dir") + "/texturesets/");
		if (!f.exists()) {
			f.mkdir();
		}
		System.out.println("Importing texturesets from: " + f);
		importTextureSets(f);

		if (texturesets.getSize() > 0) {
			cmbTSets.setSelectedIndex(0);
		}
		updateTextureSetCombo();
	}

	public void importEntityTypes() {
		try {
			Collection<EditType> types = EntityBrush.loadEntityTypes();
			// organize entites by categories

			Map<String, DefaultMutableTreeNode> treeMapping = new HashMap<String, DefaultMutableTreeNode>();

			DefaultMutableTreeNode root = new DefaultMutableTreeNode("ROOT");

			for (EditType type : types) {
				String[] cat = type.category.split("/");
				DefaultMutableTreeNode treeNode = null;
				for (int i = 0; i < cat.length; i++) {
					// creation of nodes
					String path = "";
					for (int j = 0; j <= i; j++) {
						path += "/" + cat[j];
					}
					if (treeMapping.get(path) == null) {
						DefaultMutableTreeNode newNode = new DefaultMutableTreeNode(cat[i]);
						if (treeNode == null) {
							treeNode = newNode;
						} else {
							treeNode.add(newNode);
							treeNode = newNode;
						}
						if (i == 0) {
							root.add(treeNode);
						}
						treeMapping.put(path, treeNode);
					} else {
						treeNode = treeMapping.get(path);
					}
				}

				// added all tree nodes in path
				// find this categorie's node
				treeNode = treeMapping.get("/" + type.category);

				DefaultMutableTreeNode entityNode = new DefaultMutableTreeNode(type, false);
				treeNode.add(entityNode);
			}

			DefaultTreeModel model = (DefaultTreeModel) treeEntities.getModel();
			model.setRoot(root);

			invalidate();
		} catch (Throwable t) {
			ErrorReporter.reportError("Error occured while loading entities", t);
		}
	}

	public WorldTool() {
		instance = this;

		Logger.getLogger("").setLevel(Level.WARNING);

		FileSystem fileSystem = new DefaultFileSystem("icons");
		ResourceManager.setFileSystem(fileSystem);

		ResourceManager.registerLoader(Image.class, new ImageResourceLoader());
		setIconImages(ResourceManager.loadResources(Image.class, "WT_logo.png", "WT_icon_64.png", "WT_icon_32.png", "WT_icon_16.png")); // Java6 only
		
		state = EditorState.getState();
		initComponents();
		menu.add(LafMenu.setupLafMenu(this, "WorldTool"));
		textures = (DefaultListModel) lstTex.getModel();
		texturesets = (DefaultComboBoxModel) cmbTSets.getModel();

		setLocationRelativeTo(null);

		new Thread() {

			@Override
			public void run() {
				try {
					context.waitFor();

					importTextureSets();
					importEntityTypes();

					// create the model camera handler for the mouse
					EditorState.handler = new WorldCameraHandler(context.getRenderer());
					glCanvas.addMouseWheelListener(EditorState.handler);
					glCanvas.addMouseListener(EditorState.handler);
					glCanvas.addMouseMotionListener(EditorState.handler);

					// create the render pass
					render = new RenderPass() {

						@Override
						public void initPass(JmeContext cx) {
//                            Thread.setDefaultUncaughtExceptionHandler(new EditorExceptionHandler());
						}

						@Override
						public void doUpdate(JmeContext cx) {
							super.doUpdate(cx);
							if ((doingMouseAction || mouseActionEnd) && World.getWorld() != null) {
								if (mouseActionOnce) {
									mouseActionOnce = false;
									Brush.doMouseAction(mouseX, mouseY, false, false);
								} else {
									if (mouseActionEnd) {
										mouseActionEnd = false;
										Brush.doMouseAction(mouseX, mouseY, false, true);
									} else {
										Brush.doMouseAction(mouseX, mouseY, true, false);
									}
								}
							}
						}

						@Override
						public void doRender(JmeContext cx) {
							super.doRender(cx);

//                            if (World.getWorld() != null){
//                                for (Spatial c : World.getWorld().getChildren()){
//                                    if (c.getName().startsWith("GROUP"))
//                                        Debugger.drawBounds(c, cx.getRenderer(), false);
//                                }
//                            }
						}
					};
					context.getPassManager().add(render);

					selectionPass = new SelectionEffectPass();
					context.getPassManager().add(selectionPass);

					// put the camera above the terrain and face the center
					final AbstractCamera cam = (AbstractCamera) JmeContext.get().getRenderer().getCamera();
					cam.setLocation(new Vector3f(0, 50, -50));
					cam.lookAt(Vector3f.ZERO, Vector3f.UNIT_Y);

					context.executeLater(new Callable<Object>() {

						public Object call() {
							cam.getProjectionMatrix();
							cam.getModelViewMatrix();
							cam.update();
							TextureManager.preloadCache(context.getRenderer());
							return null;
						}
					});

				} catch (Throwable ex) {
					ErrorReporter.reportError("Error while initializing model view", ex);
				}
			}
		}.start();
	}

	private LWJGLCanvas createCanvas() {
		try {
			context = JmeContext.create(LWJGLContext.class, JmeContext.CONTEXT_CANVAS);
			context.start();
			glCanvas = (LWJGLCanvas) context.getCanvas();
			return glCanvas;
		} catch (Throwable ex) {
			ErrorReporter.reportError("Error occured while initializing 3D canvas", ex);
		}

		return null;
	}

	/** This method is called from within the constructor to
	 * initialize the form.
	 * WARNING: Do NOT modify this code. The content of this method is
	 * always regenerated by the Form Editor.
	 */
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        brushTypeGroup = new javax.swing.ButtonGroup();
        editTypeGroup = new javax.swing.ButtonGroup();
        menuEditRedo = new javax.swing.JMenuItem();
        split = new javax.swing.JSplitPane();
        canvas = createCanvas();
        tab = new javax.swing.JTabbedPane();
        pnlWorld = new javax.swing.JPanel();
        jButton1 = new javax.swing.JButton();
        pnlTerrain = new javax.swing.JPanel();
        pnlTerrainBrush = new javax.swing.JPanel();
        radRaise = new javax.swing.JToggleButton();
        radLower = new javax.swing.JToggleButton();
        radSmooth = new javax.swing.JToggleButton();
        radPlatau = new javax.swing.JToggleButton();
        radNoise = new javax.swing.JToggleButton();
        sldBrushSize = new javax.swing.JSlider();
        lblTerrainBSize = new javax.swing.JLabel();
        lblStrength = new javax.swing.JLabel();
        sldBrushStr = new javax.swing.JSlider();
        pnlTexture = new javax.swing.JPanel();
        pnlTexBrush = new javax.swing.JPanel();
        jScrollPane1 = new javax.swing.JScrollPane();
        lstTex = new javax.swing.JList();
        cmbTSets = new javax.swing.JComboBox();
        btnTSetImport = new javax.swing.JButton();
        btnEditTSet = new javax.swing.JButton();
        btnNewTSet = new javax.swing.JButton();
        lblTexBSize = new javax.swing.JLabel();
        sldBrushSize1 = new javax.swing.JSlider();
        sldTStr = new javax.swing.JSlider();
        lblTStr = new javax.swing.JLabel();
        pnlEntity = new javax.swing.JPanel();
        jScrollPane2 = new javax.swing.JScrollPane();
        treeEntities = new javax.swing.JTree();
        btnSelection = new javax.swing.JToggleButton();
        menu = new javax.swing.JMenuBar();
        menuFile = new javax.swing.JMenu();
        menuFileNew = new javax.swing.JMenuItem();
        jMenuItem1 = new javax.swing.JMenuItem();
        menuFileSave = new javax.swing.JMenuItem();
        menuFileSaveAs = new javax.swing.JMenuItem();
        jSeparator1 = new javax.swing.JSeparator();
        jMenuItem4 = new javax.swing.JMenuItem();
        menuFileSep1 = new javax.swing.JSeparator();
        menuFileExit = new javax.swing.JMenuItem();
        menuEdit = new javax.swing.JMenu();
        menuEditUndo = new javax.swing.JMenuItem();
        menuEditRedo1 = new javax.swing.JMenuItem();
        menuEditSep = new javax.swing.JSeparator();
        menuEditWorld = new javax.swing.JRadioButtonMenuItem();
        menuEditTerrain = new javax.swing.JRadioButtonMenuItem();
        menuEditTexture = new javax.swing.JRadioButtonMenuItem();
        menuEditEntity = new javax.swing.JRadioButtonMenuItem();
        menuView = new javax.swing.JMenu();
        menuTools = new javax.swing.JMenu();
        menuHelp = new javax.swing.JMenu();
        menuHelpForum = new javax.swing.JMenuItem();
        jSeparator3 = new javax.swing.JSeparator();
        menuHelpAbout = new javax.swing.JMenuItem();

        menuEditRedo.setText("Redo");

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);
        setTitle("WorldTool");

        split.setDividerLocation(400);
        split.setResizeWeight(0.5);
        split.setContinuousLayout(true);

        canvas.setBackground(new java.awt.Color(0, 0, 0));
        split.setLeftComponent(canvas);

        tab.addChangeListener(new javax.swing.event.ChangeListener() {
            public void stateChanged(javax.swing.event.ChangeEvent evt) {
                tabStateChanged(evt);
            }
        });

        jButton1.setText("Preview Lightmaps");
        jButton1.setEnabled(false);
        jButton1.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButton1ActionPerformed(evt);
            }
        });

        org.jdesktop.layout.GroupLayout pnlWorldLayout = new org.jdesktop.layout.GroupLayout(pnlWorld);
        pnlWorld.setLayout(pnlWorldLayout);
        pnlWorldLayout.setHorizontalGroup(
            pnlWorldLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(pnlWorldLayout.createSequentialGroup()
                .addContainerGap()
                .add(jButton1)
                .addContainerGap(62, Short.MAX_VALUE))
        );
        pnlWorldLayout.setVerticalGroup(
            pnlWorldLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(pnlWorldLayout.createSequentialGroup()
                .addContainerGap()
                .add(jButton1)
                .addContainerGap(329, Short.MAX_VALUE))
        );

        tab.addTab("World", pnlWorld);

        pnlTerrainBrush.setBorder(javax.swing.BorderFactory.createTitledBorder("Brush"));

        brushTypeGroup.add(radRaise);
        radRaise.setIcon(new javax.swing.ImageIcon(getClass().getResource("/icons/raise_normal.gif"))); // NOI18N
        radRaise.setSelected(true);
        radRaise.setToolTipText("Raise tool: Click on terrain to make mountains and elevations");
        radRaise.setPressedIcon(new javax.swing.ImageIcon(getClass().getResource("/icons/raise_click.gif"))); // NOI18N
        radRaise.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                radRaiseActionPerformed(evt);
            }
        });

        brushTypeGroup.add(radLower);
        radLower.setIcon(new javax.swing.ImageIcon(getClass().getResource("/icons/lower_normal.gif"))); // NOI18N
        radLower.setToolTipText("Lower tool: Create ridges and valleys");
        radLower.setPressedIcon(new javax.swing.ImageIcon(getClass().getResource("/icons/lower_click.gif"))); // NOI18N
        radLower.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                radLowerActionPerformed(evt);
            }
        });

        brushTypeGroup.add(radSmooth);
        radSmooth.setIcon(new javax.swing.ImageIcon(getClass().getResource("/icons/smooth_normal.gif"))); // NOI18N
        radSmooth.setToolTipText("Smooth tool: Reduce spikes and outstanding terrain features");
        radSmooth.setPressedIcon(new javax.swing.ImageIcon(getClass().getResource("/icons/smooth_click.gif"))); // NOI18N
        radSmooth.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                radSmoothActionPerformed(evt);
            }
        });

        brushTypeGroup.add(radPlatau);
        radPlatau.setIcon(new javax.swing.ImageIcon(getClass().getResource("/icons/plateau_normal.gif"))); // NOI18N
        radPlatau.setToolTipText("Platau tool: Create sections of level terrain");
        radPlatau.setPressedIcon(new javax.swing.ImageIcon(getClass().getResource("/icons/plateau_click.gif"))); // NOI18N
        radPlatau.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                radPlatauActionPerformed(evt);
            }
        });

        brushTypeGroup.add(radNoise);
        radNoise.setIcon(new javax.swing.ImageIcon(getClass().getResource("/icons/random_normal.gif"))); // NOI18N
        radNoise.setToolTipText("Noise tool: ");
        radNoise.setPressedIcon(new javax.swing.ImageIcon(getClass().getResource("/icons/random_click.gif"))); // NOI18N
        radNoise.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                radNoiseActionPerformed(evt);
            }
        });

        org.jdesktop.layout.GroupLayout pnlTerrainBrushLayout = new org.jdesktop.layout.GroupLayout(pnlTerrainBrush);
        pnlTerrainBrush.setLayout(pnlTerrainBrushLayout);
        pnlTerrainBrushLayout.setHorizontalGroup(
            pnlTerrainBrushLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(pnlTerrainBrushLayout.createSequentialGroup()
                .addContainerGap()
                .add(pnlTerrainBrushLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING, false)
                    .add(org.jdesktop.layout.GroupLayout.TRAILING, radPlatau, 0, 0, Short.MAX_VALUE)
                    .add(org.jdesktop.layout.GroupLayout.TRAILING, radRaise, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 45, Short.MAX_VALUE))
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(pnlTerrainBrushLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.TRAILING, false)
                    .add(org.jdesktop.layout.GroupLayout.LEADING, radNoise, 0, 0, Short.MAX_VALUE)
                    .add(org.jdesktop.layout.GroupLayout.LEADING, radLower, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 45, Short.MAX_VALUE))
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(radSmooth, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, 40, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
                .addContainerGap(59, Short.MAX_VALUE))
        );
        pnlTerrainBrushLayout.setVerticalGroup(
            pnlTerrainBrushLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(pnlTerrainBrushLayout.createSequentialGroup()
                .add(pnlTerrainBrushLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                    .add(pnlTerrainBrushLayout.createSequentialGroup()
                        .add(pnlTerrainBrushLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                            .add(radRaise)
                            .add(radLower))
                        .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                        .add(pnlTerrainBrushLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.TRAILING)
                            .add(radPlatau)
                            .add(radNoise)))
                    .add(radSmooth))
                .addContainerGap(org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );

        sldBrushSize.setMaximum(30);
        sldBrushSize.setMinimum(1);
        sldBrushSize.setValue(5);
        sldBrushSize.addChangeListener(new javax.swing.event.ChangeListener() {
            public void stateChanged(javax.swing.event.ChangeEvent evt) {
                sldBrushSizeStateChanged(evt);
            }
        });

        lblTerrainBSize.setText("Brush size: ");

        lblStrength.setText("Brush strength: ");

        sldBrushStr.setMaximum(10);
        sldBrushStr.addChangeListener(new javax.swing.event.ChangeListener() {
            public void stateChanged(javax.swing.event.ChangeEvent evt) {
                sldBrushStrStateChanged(evt);
            }
        });

        org.jdesktop.layout.GroupLayout pnlTerrainLayout = new org.jdesktop.layout.GroupLayout(pnlTerrain);
        pnlTerrain.setLayout(pnlTerrainLayout);
        pnlTerrainLayout.setHorizontalGroup(
            pnlTerrainLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(pnlTerrainLayout.createSequentialGroup()
                .addContainerGap()
                .add(pnlTerrainLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                    .add(pnlTerrainBrush, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .add(pnlTerrainLayout.createSequentialGroup()
                        .add(pnlTerrainLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.TRAILING, false)
                            .add(lblTerrainBSize, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                            .add(lblStrength, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                        .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                        .add(pnlTerrainLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                            .add(sldBrushSize, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, 110, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
                            .add(sldBrushStr, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, 110, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE))))
                .addContainerGap())
        );
        pnlTerrainLayout.setVerticalGroup(
            pnlTerrainLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(pnlTerrainLayout.createSequentialGroup()
                .addContainerGap()
                .add(pnlTerrainBrush, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
                .add(pnlTerrainLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                    .add(pnlTerrainLayout.createSequentialGroup()
                        .add(15, 15, 15)
                        .add(lblTerrainBSize))
                    .add(pnlTerrainLayout.createSequentialGroup()
                        .addPreferredGap(org.jdesktop.layout.LayoutStyle.UNRELATED)
                        .add(sldBrushSize, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)))
                .add(10, 10, 10)
                .add(pnlTerrainLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                    .add(lblStrength)
                    .add(sldBrushStr, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE))
                .addContainerGap(127, Short.MAX_VALUE))
        );

        tab.addTab("Terrain", pnlTerrain);

        pnlTexBrush.setBorder(javax.swing.BorderFactory.createTitledBorder("Texture Set"));

        lstTex.setModel(new DefaultListModel());
        lstTex.addListSelectionListener(new javax.swing.event.ListSelectionListener() {
            public void valueChanged(javax.swing.event.ListSelectionEvent evt) {
                lstTexValueChanged(evt);
            }
        });
        jScrollPane1.setViewportView(lstTex);

        cmbTSets.setModel(new DefaultComboBoxModel());
        cmbTSets.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                cmbTSetsActionPerformed(evt);
            }
        });

        btnTSetImport.setText("Import");
        btnTSetImport.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btnTSetImportActionPerformed(evt);
            }
        });

        btnEditTSet.setText("Edit ...");
        btnEditTSet.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btnEditTSetActionPerformed(evt);
            }
        });

        btnNewTSet.setText("New ...");
        btnNewTSet.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btnNewTSetActionPerformed(evt);
            }
        });

        org.jdesktop.layout.GroupLayout pnlTexBrushLayout = new org.jdesktop.layout.GroupLayout(pnlTexBrush);
        pnlTexBrush.setLayout(pnlTexBrushLayout);
        pnlTexBrushLayout.setHorizontalGroup(
            pnlTexBrushLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(pnlTexBrushLayout.createSequentialGroup()
                .addContainerGap()
                .add(pnlTexBrushLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                    .add(cmbTSets, 0, 189, Short.MAX_VALUE)
                    .add(jScrollPane1, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 189, Short.MAX_VALUE)
                    .add(pnlTexBrushLayout.createSequentialGroup()
                        .add(btnNewTSet)
                        .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                        .add(btnTSetImport))
                    .add(btnEditTSet))
                .addContainerGap())
        );
        pnlTexBrushLayout.setVerticalGroup(
            pnlTexBrushLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(org.jdesktop.layout.GroupLayout.TRAILING, pnlTexBrushLayout.createSequentialGroup()
                .add(cmbTSets)
                .add(4, 4, 4)
                .add(pnlTexBrushLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.BASELINE)
                    .add(btnTSetImport)
                    .add(btnNewTSet))
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(btnEditTSet)
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(jScrollPane1, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, 115, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
                .addContainerGap())
        );

        lblTexBSize.setText("Brush size: ");

        sldBrushSize1.setMaximum(30);
        sldBrushSize1.setMinimum(1);
        sldBrushSize1.setValue(5);
        sldBrushSize1.addChangeListener(new javax.swing.event.ChangeListener() {
            public void stateChanged(javax.swing.event.ChangeEvent evt) {
                sldBrushSize1StateChanged(evt);
            }
        });

        sldTStr.setMaximum(10);
        sldTStr.addChangeListener(new javax.swing.event.ChangeListener() {
            public void stateChanged(javax.swing.event.ChangeEvent evt) {
                sldTStrStateChanged(evt);
            }
        });

        lblTStr.setText("Brush strength: ");

        org.jdesktop.layout.GroupLayout pnlTextureLayout = new org.jdesktop.layout.GroupLayout(pnlTexture);
        pnlTexture.setLayout(pnlTextureLayout);
        pnlTextureLayout.setHorizontalGroup(
            pnlTextureLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(pnlTextureLayout.createSequentialGroup()
                .addContainerGap()
                .add(pnlTextureLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                    .add(pnlTexBrush, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .add(org.jdesktop.layout.GroupLayout.TRAILING, pnlTextureLayout.createSequentialGroup()
                        .add(lblTexBSize, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 103, Short.MAX_VALUE)
                        .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                        .add(sldBrushSize1, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, 110, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE))
                    .add(org.jdesktop.layout.GroupLayout.TRAILING, pnlTextureLayout.createSequentialGroup()
                        .add(lblTStr, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                        .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                        .add(sldTStr, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, 110, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)))
                .addContainerGap())
        );
        pnlTextureLayout.setVerticalGroup(
            pnlTextureLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(pnlTextureLayout.createSequentialGroup()
                .addContainerGap()
                .add(pnlTexBrush, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
                .add(18, 18, 18)
                .add(pnlTextureLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                    .add(lblTexBSize)
                    .add(sldBrushSize1, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE))
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(pnlTextureLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                    .add(lblTStr)
                    .add(sldTStr, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE))
                .addContainerGap(org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );

        tab.addTab("Texture", pnlTexture);

        treeEntities.setRootVisible(false);
        treeEntities.addTreeSelectionListener(new javax.swing.event.TreeSelectionListener() {
            public void valueChanged(javax.swing.event.TreeSelectionEvent evt) {
                treeEntitiesValueChanged(evt);
            }
        });
        jScrollPane2.setViewportView(treeEntities);

        btnSelection.setText("Selection Mode");
        btnSelection.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btnSelectionActionPerformed(evt);
            }
        });

        org.jdesktop.layout.GroupLayout pnlEntityLayout = new org.jdesktop.layout.GroupLayout(pnlEntity);
        pnlEntity.setLayout(pnlEntityLayout);
        pnlEntityLayout.setHorizontalGroup(
            pnlEntityLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(pnlEntityLayout.createSequentialGroup()
                .addContainerGap()
                .add(pnlEntityLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                    .add(btnSelection)
                    .add(jScrollPane2, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 181, Short.MAX_VALUE))
                .addContainerGap())
        );
        pnlEntityLayout.setVerticalGroup(
            pnlEntityLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(pnlEntityLayout.createSequentialGroup()
                .addContainerGap()
                .add(jScrollPane2, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, 194, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.UNRELATED)
                .add(btnSelection)
                .addContainerGap(125, Short.MAX_VALUE))
        );

        tab.addTab("Entity", pnlEntity);

        split.setRightComponent(tab);

        menuFile.setText("File");

        menuFileNew.setText("New");
        menuFileNew.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                menuFileNewActionPerformed(evt);
            }
        });
        menuFile.add(menuFileNew);

        jMenuItem1.setText("Open");
        jMenuItem1.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jMenuItem1ActionPerformed(evt);
            }
        });
        menuFile.add(jMenuItem1);

        menuFileSave.setText("Save");
        menuFileSave.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                menuFileSaveActionPerformed(evt);
            }
        });
        menuFile.add(menuFileSave);

        menuFileSaveAs.setText("Save as...");
        menuFileSaveAs.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                menuFileSaveAsActionPerformed(evt);
            }
        });
        menuFile.add(menuFileSaveAs);
        menuFile.add(jSeparator1);

        jMenuItem4.setText("Export");
        jMenuItem4.setEnabled(false);
        menuFile.add(jMenuItem4);
        menuFile.add(menuFileSep1);

        menuFileExit.setText("Exit");
        menuFileExit.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                menuFileExitActionPerformed(evt);
            }
        });
        menuFile.add(menuFileExit);

        menu.add(menuFile);

        menuEdit.setText("Edit");

        menuEditUndo.setAccelerator(javax.swing.KeyStroke.getKeyStroke(java.awt.event.KeyEvent.VK_Z, java.awt.event.InputEvent.CTRL_MASK));
        menuEditUndo.setText("Undo");
        menuEditUndo.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                menuEditUndoActionPerformed(evt);
            }
        });
        menuEdit.add(menuEditUndo);

        menuEditRedo1.setAccelerator(javax.swing.KeyStroke.getKeyStroke(java.awt.event.KeyEvent.VK_Y, java.awt.event.InputEvent.CTRL_MASK));
        menuEditRedo1.setText("Redo");
        menuEditRedo1.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                menuEditRedo1ActionPerformed(evt);
            }
        });
        menuEdit.add(menuEditRedo1);
        menuEdit.add(menuEditSep);

        editTypeGroup.add(menuEditWorld);
        menuEditWorld.setSelected(true);
        menuEditWorld.setText("World");
        menuEditWorld.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                menuEditWorldActionPerformed(evt);
            }
        });
        menuEdit.add(menuEditWorld);

        editTypeGroup.add(menuEditTerrain);
        menuEditTerrain.setText("Terrain");
        menuEditTerrain.setIcon(new javax.swing.ImageIcon(getClass().getResource("/icons/terrain_normal.gif"))); // NOI18N
        menuEditTerrain.setSelectedIcon(new javax.swing.ImageIcon(getClass().getResource("/icons/terrain_click.gif"))); // NOI18N
        menuEditTerrain.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                menuEditTerrainActionPerformed(evt);
            }
        });
        menuEdit.add(menuEditTerrain);

        editTypeGroup.add(menuEditTexture);
        menuEditTexture.setText("Texture");
        menuEditTexture.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                menuEditTextureActionPerformed(evt);
            }
        });
        menuEdit.add(menuEditTexture);

        editTypeGroup.add(menuEditEntity);
        menuEditEntity.setText("Entity");
        menuEditEntity.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                menuEditEntityActionPerformed(evt);
            }
        });
        menuEdit.add(menuEditEntity);

        menu.add(menuEdit);

        menuView.setText("View");
        menuView.setEnabled(false);
        menu.add(menuView);

        menuTools.setText("Tools");
        menuTools.setEnabled(false);
        menu.add(menuTools);

        menuHelp.setText("Help");

        menuHelpForum.setText("WorldTool forum thread");
        menuHelpForum.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                menuHelpForumActionPerformed(evt);
            }
        });
        menuHelp.add(menuHelpForum);
        menuHelp.add(jSeparator3);

        menuHelpAbout.setText("About..");
        menuHelpAbout.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                menuHelpAboutActionPerformed(evt);
            }
        });
        menuHelp.add(menuHelpAbout);

        menu.add(menuHelp);

        setJMenuBar(menu);

        org.jdesktop.layout.GroupLayout layout = new org.jdesktop.layout.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(split, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 623, Short.MAX_VALUE)
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(org.jdesktop.layout.GroupLayout.TRAILING, split, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 434, Short.MAX_VALUE)
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

	private void updateFromEditorState() {
		switch (state.editType) {
			case TILE:
				tab.setSelectedIndex(0);
				editTypeGroup.setSelected(menuEditWorld.getModel(), true);
				break;
			case TERRAIN:
				tab.setSelectedIndex(1);
				editTypeGroup.setSelected(menuEditTerrain.getModel(), true);
				break;
			case TEXTURE:
				tab.setSelectedIndex(2);
				editTypeGroup.setSelected(menuEditTexture.getModel(), true);
				break;
			case ENTITY:
				tab.setSelectedIndex(3);
				editTypeGroup.setSelected(menuEditEntity.getModel(), true);
				break;
		}

		if (state.editType == EditType.TERRAIN) {
			sldBrushSize.setValue(state.brushSize);
			switch (state.brushType) {
				case RAISE:
					radRaise.setSelected(true);
					break;
				case LOWER:
					radLower.setSelected(true);
					break;
				case SMOOTH:
					radSmooth.setSelected(true);
					break;
				case PLATAU:
					radPlatau.setSelected(true);
					break;
				case NOISE:
					radNoise.setSelected(true);
					break;
			}
			sldBrushStr.setValue((int) (state.brushStrength * 10f));
		}

		if (state.editType == EditType.TEXTURE) {
			sldBrushSize1.setValue(state.brushSize);
			if (state.texSet != texturesets.getSelectedItem()) {
				texturesets.setSelectedItem(state.texSet);
				World.getWorld().setTextureSet(state.texSet);
			}

			lstTex.setSelectedIndex(state.textureIndex);
			sldTStr.setValue((int) (state.brushStrength * 10f));
		}
	}

	private void updateEditorState() {
		switch (tab.getSelectedIndex()) {
			case 0:
				state.editType = EditType.TILE;
				editTypeGroup.setSelected(menuEditWorld.getModel(), true);
				break;
			case 1:
				state.editType = EditType.TERRAIN;
				editTypeGroup.setSelected(menuEditTerrain.getModel(), true);
				break;
			case 2:
				state.editType = EditType.TEXTURE;
				editTypeGroup.setSelected(menuEditTexture.getModel(), true);
				break;
			case 3:
				state.editType = EditType.ENTITY;
				editTypeGroup.setSelected(menuEditEntity.getModel(), true);
				break;
			default:
				throw new RuntimeException("Unsupported operation");
		}

		if (state.editType == EditType.TERRAIN) {
			state.brushSize = sldBrushSize.getValue();
			if (radRaise.isSelected()) {
				state.brushType = BrushType.RAISE;
			} else if (radLower.isSelected()) {
				state.brushType = BrushType.LOWER;
			} else if (radSmooth.isSelected()) {
				state.brushType = BrushType.SMOOTH;
			} else if (radPlatau.isSelected()) {
				state.brushType = BrushType.PLATAU;
			} else if (radNoise.isSelected()) {
				state.brushType = BrushType.NOISE;
			}
			state.brushStrength = sldBrushStr.getValue() / 10f;
		}

		if (state.editType == EditType.TEXTURE) {
			state.brushSize = sldBrushSize1.getValue();
			if (state.texSet != texturesets.getSelectedItem()) {
				state.texSet = (TextureSet) texturesets.getSelectedItem();
				if (World.getWorld() != null) {
					World.getWorld().setTextureSet(state.texSet);
				}
			}

			state.textureIndex = lstTex.getSelectedIndex();
			state.brushStrength = sldTStr.getValue() / 10f;
		}

		if (World.getWorld() != null) {
			World.getWorld().updateFromState();
		}
	}

	private void updateTextureSetCombo() {
		TextureSet set = (TextureSet) texturesets.getSelectedItem();
		textures.clear();
		if (set != null) {
			for (Detailmap map : set.getDetailmaps()) {
				textures.addElement(map);
			}
			if (World.getWorld() != null) {
				World.getWorld().setTextureSet(set);
			}
		}
	}

    private void tabStateChanged(javax.swing.event.ChangeEvent evt) {//GEN-FIRST:event_tabStateChanged
		updateEditorState();
}//GEN-LAST:event_tabStateChanged

    private void sldBrushSizeStateChanged(javax.swing.event.ChangeEvent evt) {//GEN-FIRST:event_sldBrushSizeStateChanged
		updateEditorState();
    }//GEN-LAST:event_sldBrushSizeStateChanged

    private void menuFileExitActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_menuFileExitActionPerformed
		dispose();
		context.dispose();
    }//GEN-LAST:event_menuFileExitActionPerformed

    private void menuFileNewActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_menuFileNewActionPerformed
		if (World.getWorld() != null) {
			render.remove(World.getWorld());
		}
		final NewWorldDialog worldDialog = new NewWorldDialog(this, true);

		Callable<Object> callback = new Callable<Object>() {

			public Object call() {
				World world = new World(worldDialog.getAlphamapRes(),
						worldDialog.getGroupSize(),
						worldDialog.getGridRes());
				render.add(world);

				world.createTile(0, 0);
				World.getWorld().update();

				return null;
			}
		};
		worldDialog.setCallback(callback);
		worldDialog.setLocationRelativeTo(null);
		worldDialog.setVisible(true);
		
		lastSavedFile = null;
}//GEN-LAST:event_menuFileNewActionPerformed

    private void sldBrushSize1StateChanged(javax.swing.event.ChangeEvent evt) {//GEN-FIRST:event_sldBrushSize1StateChanged
		updateEditorState();
    }//GEN-LAST:event_sldBrushSize1StateChanged

    private void btnTSetImportActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_btnTSetImportActionPerformed
		FileNameExtensionFilter filter = new FileNameExtensionFilter("TextureSet (*.xml)", "xml");

		chooser.resetChoosableFileFilters();
		chooser.addChoosableFileFilter(filter);

		if (chooser.showOpenDialog(this) == JFileChooser.APPROVE_OPTION) {
			File f = chooser.getSelectedFile();

			URI texDir = new File(f.getParent(), "/images/").toURI();
			SimpleResourceLocator tex = new SimpleResourceLocator(texDir);
			ResourceLocatorTool.addResourceLocator(ResourceLocatorTool.TYPE_TEXTURE, tex);
			try {
				InputStream in = new FileInputStream(f);
				TextureSet set = TextureSetLoader.load(in);
				texturesets.addElement(set);
				in.close();
			} catch (IOException ex) {
				ex.printStackTrace();
			}

			ResourceLocatorTool.removeResourceLocator(ResourceLocatorTool.TYPE_TEXTURE, tex);

			updateTextureSetCombo();
		}
    }//GEN-LAST:event_btnTSetImportActionPerformed

    private void cmbTSetsActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_cmbTSetsActionPerformed
		updateTextureSetCombo();
    }//GEN-LAST:event_cmbTSetsActionPerformed

    private void lstTexValueChanged(javax.swing.event.ListSelectionEvent evt) {//GEN-FIRST:event_lstTexValueChanged
		updateEditorState();
    }//GEN-LAST:event_lstTexValueChanged

    private void sldBrushStrStateChanged(javax.swing.event.ChangeEvent evt) {//GEN-FIRST:event_sldBrushStrStateChanged
		updateEditorState();
}//GEN-LAST:event_sldBrushStrStateChanged

    private void menuEditWorldActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_menuEditWorldActionPerformed
		if (menuEditWorld.isSelected()) {
			tab.setSelectedIndex(0);
		}
}//GEN-LAST:event_menuEditWorldActionPerformed

    private void menuEditTerrainActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_menuEditTerrainActionPerformed
		if (menuEditTerrain.isSelected()) {
			tab.setSelectedIndex(1);
		}
    }//GEN-LAST:event_menuEditTerrainActionPerformed

    private void menuEditTextureActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_menuEditTextureActionPerformed
		if (menuEditTexture.isSelected()) {
			tab.setSelectedIndex(2);
		}
    }//GEN-LAST:event_menuEditTextureActionPerformed

    private void menuEditEntityActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_menuEditEntityActionPerformed
		if (menuEditEntity.isSelected()) {
			tab.setSelectedIndex(3);
		}
    }//GEN-LAST:event_menuEditEntityActionPerformed

    private void menuHelpForumActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_menuHelpForumActionPerformed
		Sys.openURL("http://www.radakan.org/forums/index.php/topic,609.0.html");
    }//GEN-LAST:event_menuHelpForumActionPerformed

    private void menuHelpAboutActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_menuHelpAboutActionPerformed
		Icon icon = new ImageIcon(getIconImages().get(1));
		JOptionPane.showMessageDialog(this,
				"Copyright(C)\n" +
				"Gibbon Entertainment\n" +
				"Created by MomokoFan for the open source Radakan game",
				"About..",
				JOptionPane.INFORMATION_MESSAGE,
				icon);
    }//GEN-LAST:event_menuHelpAboutActionPerformed

    private void radRaiseActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_radRaiseActionPerformed
		updateEditorState();
    }//GEN-LAST:event_radRaiseActionPerformed

    private void radLowerActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_radLowerActionPerformed
		updateEditorState();
    }//GEN-LAST:event_radLowerActionPerformed

    private void radSmoothActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_radSmoothActionPerformed
		updateEditorState();
    }//GEN-LAST:event_radSmoothActionPerformed

    private void radPlatauActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_radPlatauActionPerformed
		updateEditorState();
    }//GEN-LAST:event_radPlatauActionPerformed

    private void radNoiseActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_radNoiseActionPerformed
		updateEditorState();
    }//GEN-LAST:event_radNoiseActionPerformed

    private void sldTStrStateChanged(javax.swing.event.ChangeEvent evt) {//GEN-FIRST:event_sldTStrStateChanged
		updateEditorState();
}//GEN-LAST:event_sldTStrStateChanged

    private void menuFileSaveAsActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_menuFileSaveAsActionPerformed
		FileNameExtensionFilter filter = new FileNameExtensionFilter("Radakan World (*.world)", "world");

		chooser.resetChoosableFileFilters();
		chooser.addChoosableFileFilter(filter);
		
		if (chooser.showSaveDialog(this) == JFileChooser.APPROVE_OPTION) {
			File f = chooser.getSelectedFile();
			
			//If user writes no extension ".world" is added
			if(f.getName().indexOf('.') == -1){
				f = new File(f.getParent(), f.getName() + ".world");
			}

			setCursor(Cursor.getPredefinedCursor(Cursor.WAIT_CURSOR));

			try {
				BinaryExporter.getInstance().save(state, f);
				lastSavedFile = f;
			} catch (Throwable t) {
				ErrorReporter.reportError("Error while opening world", t);
			} finally {
				setCursor(Cursor.getDefaultCursor());
			}
		}
}//GEN-LAST:event_menuFileSaveAsActionPerformed

    private void jMenuItem1ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jMenuItem1ActionPerformed
		FileNameExtensionFilter filter = new FileNameExtensionFilter("Radakan World (*.world)", "world");

		chooser.resetChoosableFileFilters();
		chooser.addChoosableFileFilter(filter);

		if (chooser.showOpenDialog(this) == JFileChooser.APPROVE_OPTION) {
			File f = chooser.getSelectedFile();

			setCursor(Cursor.getPredefinedCursor(Cursor.WAIT_CURSOR));

			if (World.getWorld() != null) {
				render.remove(World.getWorld());
			}

			try {
				state = (EditorState) BinaryImporter.getInstance().load(f);
				EditorState.setState(state);

				render.add(World.getWorld());
				World.getWorld().update();
				updateFromEditorState();

				lastSavedFile = f;
			} catch (Throwable t) {
				ErrorReporter.reportError("Error while opening world", t);
			} finally {
				setCursor(Cursor.getDefaultCursor());
			}
		}
}//GEN-LAST:event_jMenuItem1ActionPerformed

    private void menuFileSaveActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_menuFileSaveActionPerformed
		if (lastSavedFile == null) {
			menuFileSaveAsActionPerformed(evt);
			return;
		}

		setCursor(Cursor.getPredefinedCursor(Cursor.WAIT_CURSOR));

		try {
			BinaryExporter.getInstance().save(state, lastSavedFile);
		} catch (Throwable t) {
			ErrorReporter.reportError("Error while opening world", t);
		} finally {
			setCursor(Cursor.getDefaultCursor());
		}
}//GEN-LAST:event_menuFileSaveActionPerformed

	private void resetEntityView() {
		if (state.entityTypePrototype != null) {
			Spatial model = state.entityTypePrototype.getUnit(ModelUnit.class).getModel();
			model.removeFromParent();
			state.entityTypePrototype = null;
		}
	}

    private void treeEntitiesValueChanged(javax.swing.event.TreeSelectionEvent evt) {//GEN-FIRST:event_treeEntitiesValueChanged
		DefaultMutableTreeNode node = (DefaultMutableTreeNode) evt.getPath().getLastPathComponent();

		resetEntityView();

		if (node.getUserObject() instanceof EntityType) {
			EditorState edState = EditorState.getState();
			edState.entityType = (EntityType) node.getUserObject();

			edState.entityTypePrototype = EntityBrush.factory.produce(edState.entityType.name, "PROTOTYPE");
//            Spatial model = state.entityTypePrototype.getUnit(ModelUnit.class).getModel();
//            World.getWorld().update();
		} else {
			EditorState.getState().entityType = null;
		}
    }//GEN-LAST:event_treeEntitiesValueChanged

    private void btnSelectionActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_btnSelectionActionPerformed
		state.selectionMode = btnSelection.isSelected();
		if (!state.selectionMode) {
			state.selection.clear();
		}
}//GEN-LAST:event_btnSelectionActionPerformed

    private void jButton1ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButton1ActionPerformed
		TileBrush.previewLightmaps();
    }//GEN-LAST:event_jButton1ActionPerformed

    private void btnEditTSetActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_btnEditTSetActionPerformed
		if (cmbTSets.getSelectedItem() != null) {
			final TextureSetEditor editor = new TextureSetEditor(this, (TextureSet) cmbTSets.getSelectedItem());
			editor.setCallback(new Callable<Object>() {

				public Object call() {
					updateTextureSetCombo();
					return null;
				}
			});
			editor.setLocationRelativeTo(this);
			editor.setVisible(true);
		}
    }//GEN-LAST:event_btnEditTSetActionPerformed

    private void btnNewTSetActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_btnNewTSetActionPerformed
		TextureSetEditor editor = new TextureSetEditor(this, null);
		editor.setVisible(true);
		editor.setCallback(new Callable<Object>() {

			public Object call() {
				updateTextureSetCombo();
				return null;
			}
		});
    }//GEN-LAST:event_btnNewTSetActionPerformed

    private void menuEditUndoActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_menuEditUndoActionPerformed
		UndoManager.doUndo();
    }//GEN-LAST:event_menuEditUndoActionPerformed

    private void menuEditRedo1ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_menuEditRedo1ActionPerformed
		UndoManager.doRedo();
    }//GEN-LAST:event_menuEditRedo1ActionPerformed
    
    /**
     * @param args the command line arguments
     */
    public static void main(String args[]) {
        JPopupMenu.setDefaultLightWeightPopupEnabled(false);
        
        java.awt.EventQueue.invokeLater(new Runnable() {
            public void run() {
                new WorldTool().setVisible(true);
            }
        });
    }
    
    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.ButtonGroup brushTypeGroup;
    private javax.swing.JButton btnEditTSet;
    private javax.swing.JButton btnNewTSet;
    private javax.swing.JToggleButton btnSelection;
    private javax.swing.JButton btnTSetImport;
    private java.awt.Canvas canvas;
    private javax.swing.JComboBox cmbTSets;
    private javax.swing.ButtonGroup editTypeGroup;
    private javax.swing.JButton jButton1;
    private javax.swing.JMenuItem jMenuItem1;
    private javax.swing.JMenuItem jMenuItem4;
    private javax.swing.JScrollPane jScrollPane1;
    private javax.swing.JScrollPane jScrollPane2;
    private javax.swing.JSeparator jSeparator1;
    private javax.swing.JSeparator jSeparator3;
    private javax.swing.JLabel lblStrength;
    private javax.swing.JLabel lblTStr;
    private javax.swing.JLabel lblTerrainBSize;
    private javax.swing.JLabel lblTexBSize;
    private javax.swing.JList lstTex;
    private javax.swing.JMenuBar menu;
    private javax.swing.JMenu menuEdit;
    private javax.swing.JRadioButtonMenuItem menuEditEntity;
    private javax.swing.JMenuItem menuEditRedo;
    private javax.swing.JMenuItem menuEditRedo1;
    private javax.swing.JSeparator menuEditSep;
    private javax.swing.JRadioButtonMenuItem menuEditTerrain;
    private javax.swing.JRadioButtonMenuItem menuEditTexture;
    private javax.swing.JMenuItem menuEditUndo;
    private javax.swing.JRadioButtonMenuItem menuEditWorld;
    private javax.swing.JMenu menuFile;
    private javax.swing.JMenuItem menuFileExit;
    private javax.swing.JMenuItem menuFileNew;
    private javax.swing.JMenuItem menuFileSave;
    private javax.swing.JMenuItem menuFileSaveAs;
    private javax.swing.JSeparator menuFileSep1;
    private javax.swing.JMenu menuHelp;
    private javax.swing.JMenuItem menuHelpAbout;
    private javax.swing.JMenuItem menuHelpForum;
    private javax.swing.JMenu menuTools;
    private javax.swing.JMenu menuView;
    private javax.swing.JPanel pnlEntity;
    private javax.swing.JPanel pnlTerrain;
    private javax.swing.JPanel pnlTerrainBrush;
    private javax.swing.JPanel pnlTexBrush;
    private javax.swing.JPanel pnlTexture;
    private javax.swing.JPanel pnlWorld;
    private javax.swing.JToggleButton radLower;
    private javax.swing.JToggleButton radNoise;
    private javax.swing.JToggleButton radPlatau;
    private javax.swing.JToggleButton radRaise;
    private javax.swing.JToggleButton radSmooth;
    private javax.swing.JSlider sldBrushSize;
    private javax.swing.JSlider sldBrushSize1;
    private javax.swing.JSlider sldBrushStr;
    private javax.swing.JSlider sldTStr;
    private javax.swing.JSplitPane split;
    private javax.swing.JTabbedPane tab;
    private javax.swing.JTree treeEntities;
    // End of variables declaration//GEN-END:variables
    
}
