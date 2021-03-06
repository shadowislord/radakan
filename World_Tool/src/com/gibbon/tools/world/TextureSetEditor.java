package com.gibbon.tools.world;

import com.radakan.util.preview.FileNameExtensionFilter;
import com.jme.image.Texture2D;
import com.jme.util.TextureManager;
import com.radakan.game.Game;
import com.radakan.game.tile.TextureSet;
import com.radakan.game.tile.TextureSet.Detailmap;
import java.awt.Frame;
import java.io.File;
import java.net.MalformedURLException;
import java.util.ArrayList;
import java.util.concurrent.Callable;
import javax.swing.DefaultComboBoxModel;
import javax.swing.DefaultListModel;
import javax.swing.JFileChooser;

public class TextureSetEditor extends javax.swing.JDialog {

	private static final long serialVersionUID = 6216255895314778816L;
	private DefaultComboBoxModel importModel;
    private DefaultListModel importListModel;
    private DefaultListModel textureModel;
    
    private TextureSet original;
    private Callable<?> callback;
    
    private JFileChooser chooser = new JFileChooser();
    
    public TextureSetEditor(Frame parent, TextureSet original) {
        super(parent, true);
        
        FileNameExtensionFilter filter = new FileNameExtensionFilter("Images (*.bmp, *.wbmp, *.png, *.jpg, *.tga, *.dds)",
                                                                            "bmp", "wbmp", "png", "jpg", "tga", "dds");
        chooser.addChoosableFileFilter(filter);
        
        importModel = new DefaultComboBoxModel();
        importListModel = new DefaultListModel();
        textureModel = new DefaultListModel();
        
        if (original != null){
            this.original = original;
            // load the content into the bars..
            for (Detailmap map : original.getDetailmaps()){
                textureModel.addElement(map);
            }
        }else{
            this.original = new TextureSet("");
        }
        
        for (TextureSet set : EditorState.texsetMap.values()){
            if (set == original)
                continue;
            
            importModel.addElement(set);
        }
        initComponents();
        
        assert original != null;
        txtTSetName.setText(original.toString());
        lstImportSets.setSelectedIndex(0);
    }
    
    public void setCallback(Callable<?> callback){
        this.callback = callback;
    }
    
    public TextureSet getEditedTextureSet(){
        return original;
    }
    
    /** This method is called from within the constructor to
     * initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is
     * always regenerated by the Form Editor.
     */
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        pnlImport = new javax.swing.JPanel();
        cmbImportSets = new javax.swing.JComboBox();
        jScrollPane1 = new javax.swing.JScrollPane();
        lstImportSets = new javax.swing.JList();
        btnImportTex = new javax.swing.JButton();
        btnSave = new javax.swing.JButton();
        btnCancel = new javax.swing.JButton();
        jPanel1 = new javax.swing.JPanel();
        jScrollPane2 = new javax.swing.JScrollPane();
        lstTextures = new javax.swing.JList();
        btnNewTex = new javax.swing.JButton();
        btnDelete = new javax.swing.JButton();
        btnSetDefault = new javax.swing.JButton();
        lblPath = new javax.swing.JLabel();
        lblTSetName = new javax.swing.JLabel();
        txtTSetName = new javax.swing.JTextField();

        setDefaultCloseOperation(javax.swing.WindowConstants.DISPOSE_ON_CLOSE);
        setTitle("Edit TextureSet");

        pnlImport.setBorder(javax.swing.BorderFactory.createTitledBorder("Import textures"));

        cmbImportSets.setModel(importModel);
        cmbImportSets.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                cmbImportSetsActionPerformed(evt);
            }
        });

        lstImportSets.setModel(importListModel);
        jScrollPane1.setViewportView(lstImportSets);

        btnImportTex.setText(">");
        btnImportTex.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btnImportTexActionPerformed(evt);
            }
        });

        org.jdesktop.layout.GroupLayout pnlImportLayout = new org.jdesktop.layout.GroupLayout(pnlImport);
        pnlImport.setLayout(pnlImportLayout);
        pnlImportLayout.setHorizontalGroup(
            pnlImportLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(org.jdesktop.layout.GroupLayout.TRAILING, pnlImportLayout.createSequentialGroup()
                .addContainerGap()
                .add(pnlImportLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                    .add(jScrollPane1, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 162, Short.MAX_VALUE)
                    .add(cmbImportSets, 0, 162, Short.MAX_VALUE))
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(btnImportTex)
                .addContainerGap())
        );
        pnlImportLayout.setVerticalGroup(
            pnlImportLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(pnlImportLayout.createSequentialGroup()
                .add(cmbImportSets, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.UNRELATED)
                .add(jScrollPane1, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 299, Short.MAX_VALUE)
                .addContainerGap())
            .add(org.jdesktop.layout.GroupLayout.TRAILING, pnlImportLayout.createSequentialGroup()
                .addContainerGap(149, Short.MAX_VALUE)
                .add(btnImportTex)
                .add(169, 169, 169))
        );

        btnSave.setText("Save");
        btnSave.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btnSaveActionPerformed(evt);
            }
        });

        btnCancel.setText("Cancel");
        btnCancel.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btnCancelActionPerformed(evt);
            }
        });

        jPanel1.setBorder(javax.swing.BorderFactory.createTitledBorder("Textureset"));

        lstTextures.setModel(textureModel);
        lstTextures.addListSelectionListener(new javax.swing.event.ListSelectionListener() {
            public void valueChanged(javax.swing.event.ListSelectionEvent evt) {
                lstTexturesValueChanged(evt);
            }
        });
        jScrollPane2.setViewportView(lstTextures);

        btnNewTex.setText("Add New..");
        btnNewTex.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btnNewTexActionPerformed(evt);
            }
        });

        btnDelete.setText("Delete");
        btnDelete.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btnDeleteActionPerformed(evt);
            }
        });

        btnSetDefault.setText("Set to default");
        btnSetDefault.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btnSetDefaultActionPerformed(evt);
            }
        });

        lblPath.setText("Path: ");

        lblTSetName.setText("Name: ");

        org.jdesktop.layout.GroupLayout jPanel1Layout = new org.jdesktop.layout.GroupLayout(jPanel1);
        jPanel1.setLayout(jPanel1Layout);
        jPanel1Layout.setHorizontalGroup(
            jPanel1Layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(jPanel1Layout.createSequentialGroup()
                .addContainerGap()
                .add(jPanel1Layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                    .add(org.jdesktop.layout.GroupLayout.TRAILING, jScrollPane2, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 279, Short.MAX_VALUE)
                    .add(jPanel1Layout.createSequentialGroup()
                        .add(btnNewTex)
                        .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                        .add(btnDelete)
                        .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED, 28, Short.MAX_VALUE)
                        .add(btnSetDefault))
                    .add(lblPath)
                    .add(jPanel1Layout.createSequentialGroup()
                        .add(lblTSetName)
                        .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                        .add(txtTSetName, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, 98, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)))
                .addContainerGap())
        );
        jPanel1Layout.setVerticalGroup(
            jPanel1Layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(jPanel1Layout.createSequentialGroup()
                .add(jPanel1Layout.createParallelGroup(org.jdesktop.layout.GroupLayout.BASELINE)
                    .add(btnNewTex)
                    .add(btnDelete)
                    .add(btnSetDefault))
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(jScrollPane2, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 250, Short.MAX_VALUE)
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.UNRELATED)
                .add(jPanel1Layout.createParallelGroup(org.jdesktop.layout.GroupLayout.BASELINE)
                    .add(lblTSetName)
                    .add(txtTSetName, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE))
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(lblPath)
                .addContainerGap())
        );

        org.jdesktop.layout.GroupLayout layout = new org.jdesktop.layout.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(layout.createSequentialGroup()
                .addContainerGap()
                .add(pnlImport, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.UNRELATED)
                .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                    .add(layout.createSequentialGroup()
                        .add(183, 183, 183)
                        .add(btnCancel)
                        .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                        .add(btnSave))
                    .add(layout.createSequentialGroup()
                        .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                        .add(jPanel1, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)))
                .addContainerGap())
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(org.jdesktop.layout.GroupLayout.TRAILING, layout.createSequentialGroup()
                .add(37, 37, 37)
                .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                    .add(pnlImport, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .add(jPanel1, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.UNRELATED)
                .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.BASELINE)
                    .add(btnSave)
                    .add(btnCancel))
                .addContainerGap())
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

    private void btnNewTexActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_btnNewTexActionPerformed
        if (chooser.showOpenDialog(this) == JFileChooser.APPROVE_OPTION){
            File f = chooser.getSelectedFile();
            int index = f.getName().lastIndexOf(".");
            String name = index == -1 ? f.getName() : f.getName().substring(0, index);
            Detailmap map = new Detailmap(name);
            map.scale = 1.0f;
            try{
                map.colormap = (Texture2D) TextureManager.loadTexture(f.toURI().toURL(), true);
            } catch (MalformedURLException ex){
                Game.getDebugManager().reportError("Incorrect file name specified", ex);
            }
            textureModel.addElement(map);
        }
}//GEN-LAST:event_btnNewTexActionPerformed

    private void cmbImportSetsActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_cmbImportSetsActionPerformed
        importListModel.clear();
        for (Detailmap map : ((TextureSet) cmbImportSets.getSelectedItem()).getDetailmaps()){
            importListModel.addElement(map);
        }
    }//GEN-LAST:event_cmbImportSetsActionPerformed

    private void btnImportTexActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_btnImportTexActionPerformed
        for (Object tex : lstImportSets.getSelectedValues()){
            textureModel.addElement(tex);
        }
    }//GEN-LAST:event_btnImportTexActionPerformed

    private void lstTexturesValueChanged(javax.swing.event.ListSelectionEvent evt) {//GEN-FIRST:event_lstTexturesValueChanged
        if (lstTextures.getSelectedValue() != null){
            lblPath.setText("Path: "+((Detailmap)lstTextures.getSelectedValue()).colormap.getImageLocation());
        }else{
            lblPath.setText("Path: ");
        }
    }//GEN-LAST:event_lstTexturesValueChanged

    private void btnDeleteActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_btnDeleteActionPerformed
        if (lstTextures.getSelectedValue() != null){
            textureModel.removeElement(lstTextures.getSelectedValue());
        }
}//GEN-LAST:event_btnDeleteActionPerformed

    private void btnSetDefaultActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_btnSetDefaultActionPerformed
        if (lstTextures.getSelectedValue() != null){
            Object selected = lstTextures.getSelectedValue();
            textureModel.removeElement(selected);
            textureModel.add(0, selected);
        }
}//GEN-LAST:event_btnSetDefaultActionPerformed

    private void btnCancelActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_btnCancelActionPerformed
        dispose();
    }//GEN-LAST:event_btnCancelActionPerformed

    private void btnSaveActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_btnSaveActionPerformed
        try {
            original.setName(txtTSetName.getText());

            ArrayList<Detailmap> detailmaps = new ArrayList<Detailmap>();
            for (int i = 0; i < textureModel.size(); i++) {
                detailmaps.add((Detailmap) textureModel.get(i));
            }
            original.setDetailmaps(detailmaps);

            callback.call();
            
            dispose();
        } catch (Exception ex) {
            Game.getDebugManager().reportError("Error while saving textureset", ex);
        }
    }//GEN-LAST:event_btnSaveActionPerformed
    
    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JButton btnCancel;
    private javax.swing.JButton btnDelete;
    private javax.swing.JButton btnImportTex;
    private javax.swing.JButton btnNewTex;
    private javax.swing.JButton btnSave;
    private javax.swing.JButton btnSetDefault;
    private javax.swing.JComboBox cmbImportSets;
    private javax.swing.JPanel jPanel1;
    private javax.swing.JScrollPane jScrollPane1;
    private javax.swing.JScrollPane jScrollPane2;
    private javax.swing.JLabel lblPath;
    private javax.swing.JLabel lblTSetName;
    private javax.swing.JList lstImportSets;
    private javax.swing.JList lstTextures;
    private javax.swing.JPanel pnlImport;
    private javax.swing.JTextField txtTSetName;
    // End of variables declaration//GEN-END:variables
    
}
