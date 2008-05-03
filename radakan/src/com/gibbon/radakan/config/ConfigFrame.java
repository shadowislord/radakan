/*
 * Radakan RPG is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Radakan RPG is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Radakan RPG.  If not, see <http://www.gnu.org/licenses/>.
 */

package com.gibbon.radakan.config;

import com.jme.system.GameSettings;
import java.awt.DisplayMode;
import java.awt.GraphicsDevice;
import java.awt.GraphicsEnvironment;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.DefaultComboBoxModel;
import javax.swing.DefaultListModel;
import javax.swing.UIManager;
import javax.swing.UnsupportedLookAndFeelException;

public class ConfigFrame extends javax.swing.JFrame {
    
    private DefaultListModel model;
    
    private DefaultComboBoxModel resModel = new DefaultComboBoxModel();
    private DefaultComboBoxModel bppModel = new DefaultComboBoxModel();
    private DefaultComboBoxModel freqModel = new DefaultComboBoxModel();
    
    private GameSettings settings;
    
    private Object activeLock = new Object();
    
    private DisplayModeWrapper[] modes;
    private boolean disableUpdate = true;
    
    public Object getActiveLock(){
        return activeLock;
    }
    
    private static class DisplayModeWrapper {
        
        private DisplayMode dm;
        
        public DisplayModeWrapper(DisplayMode dm){ this.dm=dm; }
        
        public String toString(){
            return dm.getWidth()+"x"+dm.getHeight()+"x"+dm.getBitDepth()+" @ "+dm.getRefreshRate()+" Hz";
        }
    }
    
    public boolean filter(DisplayMode mode){
        int w = 0,h = 0,bpp = 0,freq = 0;
        
        if (!cmbRes.getSelectedItem().equals("All")){
            String[] res = ((String) cmbRes.getSelectedItem()).split("x");
            w = Integer.valueOf(res[0]);
            h = Integer.valueOf(res[1]);
        }
        
        if (!cmbBPP.getSelectedItem().equals("All")){
            bpp = Integer.valueOf((String)cmbBPP.getSelectedItem());
        }
        
        if (!cmbFreq.getSelectedItem().equals("All")){
            freq = Integer.valueOf((String)cmbFreq.getSelectedItem());
        }
        
        return  (mode.getWidth()==w || w==0)
             && (mode.getHeight()==h || h==0)
             && (mode.getBitDepth()==bpp || bpp==0)
             && (mode.getRefreshRate()==freq || freq==0);
    }
    
    public void updateModeList(){
        if (disableUpdate) return;
        model.clear();
        for (DisplayModeWrapper mode: modes){
            DisplayMode m = mode.dm;
            if (filter(m)){
                model.addElement(mode);
            }
        }
    }
    
    public ConfigFrame(GameSettings settings) {
        try {
            UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());

            model = new DefaultListModel();
            this.settings = settings;

            initComponents();

            GraphicsDevice gd = GraphicsEnvironment.getLocalGraphicsEnvironment().getDefaultScreenDevice();

            DisplayMode[] m = gd.getDisplayModes();

            resModel.addElement("All");
            bppModel.addElement("All");
            freqModel.addElement("All");

            modes = new DisplayModeWrapper[m.length];
            for (int i = 0; i < m.length; i++) {
                modes[i] = new DisplayModeWrapper(m[i]);

                String res = m[i].getWidth() + "x" + m[i].getHeight();
                if (resModel.getIndexOf(res) == -1) {
                    resModel.addElement(res);
                }

                String bpp = Integer.toString(m[i].getBitDepth());
                if (bppModel.getIndexOf(bpp) == -1) {
                    bppModel.addElement(bpp);
                }

                String freq = Integer.toString(m[i].getRefreshRate());
                if (freqModel.getIndexOf(freq) == -1) {
                    freqModel.addElement(freq);
                }
            }

            disableUpdate = false;
            updateModeList();

            lstMode.requestFocusInWindow();
            int w = settings.getWidth();
            int h = settings.getHeight();
            int bpp = settings.getDepth();
            int freq = settings.getFrequency();
            for (int i = 0; i < modes.length; i++) {
                DisplayMode mode = modes[i].dm;
                if (mode.getWidth() == w && mode.getHeight() == h && mode.getBitDepth() == bpp && mode.getRefreshRate() == freq) {
                    lstMode.setSelectedValue(modes[i], true);
                    lstMode.ensureIndexIsVisible(i);
                }
            }
            lstMode.invalidate();

            cmbQuality.setSelectedIndex(settings.getInt("GameTextureQuality", 3));
            chkFS.setSelected(settings.isFullscreen());
            chkMusic.setSelected(!settings.isMusic());
            lblVol.setEnabled(!settings.isMusic());
            barVol.setEnabled(!settings.isMusic());
            barVol.setValue( (int) (settings.getFloat("GameMusicVolume", 1f) * 100)  );
            txtAA.setText(""+settings.getSamples());
            
            setLocationRelativeTo(null);
        } catch (ClassNotFoundException ex) {
            Logger.getLogger(ConfigFrame.class.getName()).log(Level.SEVERE, null, ex);
        } catch (InstantiationException ex) {
            Logger.getLogger(ConfigFrame.class.getName()).log(Level.SEVERE, null, ex);
        } catch (IllegalAccessException ex) {
            Logger.getLogger(ConfigFrame.class.getName()).log(Level.SEVERE, null, ex);
        } catch (UnsupportedLookAndFeelException ex) {
            Logger.getLogger(ConfigFrame.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    
    public void waitFor() throws InterruptedException{
        synchronized (activeLock){
            while (isVisible()){
                activeLock.wait();
            }
        }
    }
    
    /** This method is called from within the constructor to
     * initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is
     * always regenerated by the Form Editor.
     */
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        lblMode = new javax.swing.JLabel();
        sclMode = new javax.swing.JScrollPane();
        lstMode = new javax.swing.JList();
        btnStart = new javax.swing.JButton();
        btnCancel = new javax.swing.JButton();
        cmbQuality = new javax.swing.JComboBox();
        lblQuality = new javax.swing.JLabel();
        sep = new javax.swing.JSeparator();
        chkFS = new javax.swing.JCheckBox();
        jPanel1 = new javax.swing.JPanel();
        cmbFreq = new javax.swing.JComboBox();
        lblFreq = new javax.swing.JLabel();
        lblBPP = new javax.swing.JLabel();
        cmbBPP = new javax.swing.JComboBox();
        lblRes = new javax.swing.JLabel();
        cmbRes = new javax.swing.JComboBox();
        chkMusic = new javax.swing.JCheckBox();
        chkVsync = new javax.swing.JCheckBox();
        jLabel1 = new javax.swing.JLabel();
        txtAA = new javax.swing.JTextField();
        barVol = new javax.swing.JSlider();
        lblVol = new javax.swing.JLabel();

        setDefaultCloseOperation(javax.swing.WindowConstants.DISPOSE_ON_CLOSE);
        setTitle("Graphics Config");
        addWindowListener(new java.awt.event.WindowAdapter() {
            public void windowClosed(java.awt.event.WindowEvent evt) {
                formWindowClosed(evt);
            }
        });

        lblMode.setText("Select display mode:");

        lstMode.setModel(model);
        sclMode.setViewportView(lstMode);

        btnStart.setText("Start");
        btnStart.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btnStartActionPerformed(evt);
            }
        });

        btnCancel.setText("Cancel");
        btnCancel.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btnCancelActionPerformed(evt);
            }
        });

        cmbQuality.setModel(new javax.swing.DefaultComboBoxModel(new String[] { "Lowest", "Performance", "Balanced", "Quality", "High", "Maximum" }));
        cmbQuality.setSelectedIndex(2);

        lblQuality.setText("Quality: ");

        chkFS.setText("Full screen");
        chkFS.setBorder(javax.swing.BorderFactory.createEmptyBorder(0, 0, 0, 0));
        chkFS.setMargin(new java.awt.Insets(0, 0, 0, 0));

        jPanel1.setBorder(javax.swing.BorderFactory.createTitledBorder(javax.swing.BorderFactory.createTitledBorder("Filter")));

        cmbFreq.setModel(freqModel);
        cmbFreq.addItemListener(new java.awt.event.ItemListener() {
            public void itemStateChanged(java.awt.event.ItemEvent evt) {
                cmbFreqItemStateChanged(evt);
            }
        });

        lblFreq.setText("Frequency:");

        lblBPP.setText("Color Depth:");

        cmbBPP.setModel(bppModel);
        cmbBPP.addItemListener(new java.awt.event.ItemListener() {
            public void itemStateChanged(java.awt.event.ItemEvent evt) {
                cmbBPPItemStateChanged(evt);
            }
        });

        lblRes.setText("Resolution");

        cmbRes.setModel(resModel);
        cmbRes.addItemListener(new java.awt.event.ItemListener() {
            public void itemStateChanged(java.awt.event.ItemEvent evt) {
                cmbResItemStateChanged(evt);
            }
        });

        javax.swing.GroupLayout jPanel1Layout = new javax.swing.GroupLayout(jPanel1);
        jPanel1.setLayout(jPanel1Layout);
        jPanel1Layout.setHorizontalGroup(
            jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel1Layout.createSequentialGroup()
                .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(lblRes)
                    .addGroup(jPanel1Layout.createSequentialGroup()
                        .addContainerGap()
                        .addComponent(cmbRes, javax.swing.GroupLayout.PREFERRED_SIZE, 85, javax.swing.GroupLayout.PREFERRED_SIZE)))
                .addGap(10, 10, 10)
                .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                    .addGroup(jPanel1Layout.createSequentialGroup()
                        .addGap(10, 10, 10)
                        .addComponent(cmbBPP, 0, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                    .addComponent(lblBPP))
                .addGap(16, 16, 16)
                .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                    .addComponent(cmbFreq, 0, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addComponent(lblFreq, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                .addGap(112, 112, 112))
        );
        jPanel1Layout.setVerticalGroup(
            jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel1Layout.createSequentialGroup()
                .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(lblRes)
                    .addComponent(lblBPP)
                    .addComponent(lblFreq))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(cmbFreq, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(cmbBPP, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(cmbRes, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addContainerGap())
        );

        chkMusic.setText("Disable Music");
        chkMusic.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                chkMusicActionPerformed(evt);
            }
        });

        chkVsync.setText("Vertical Sync");

        jLabel1.setText("Antialias x");

        txtAA.setText("1");

        barVol.setMajorTickSpacing(10);
        barVol.setSnapToTicks(true);
        barVol.setValue(100);

        lblVol.setText("Volume: ");

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                    .addComponent(sclMode, javax.swing.GroupLayout.Alignment.LEADING, javax.swing.GroupLayout.DEFAULT_SIZE, 373, Short.MAX_VALUE)
                    .addComponent(jPanel1, javax.swing.GroupLayout.Alignment.LEADING, javax.swing.GroupLayout.DEFAULT_SIZE, 373, Short.MAX_VALUE)
                    .addGroup(javax.swing.GroupLayout.Alignment.LEADING, layout.createSequentialGroup()
                        .addComponent(btnStart)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(btnCancel))
                    .addComponent(sep, javax.swing.GroupLayout.Alignment.LEADING, javax.swing.GroupLayout.DEFAULT_SIZE, 373, Short.MAX_VALUE)
                    .addComponent(lblMode, javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(javax.swing.GroupLayout.Alignment.LEADING, layout.createSequentialGroup()
                        .addComponent(chkMusic)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                        .addComponent(lblVol)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                        .addComponent(barVol, javax.swing.GroupLayout.DEFAULT_SIZE, 142, Short.MAX_VALUE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                        .addComponent(jLabel1)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(txtAA, javax.swing.GroupLayout.PREFERRED_SIZE, 22, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addGroup(javax.swing.GroupLayout.Alignment.LEADING, layout.createSequentialGroup()
                        .addComponent(lblQuality)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(cmbQuality, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, 84, Short.MAX_VALUE)
                        .addComponent(chkVsync)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(chkFS)))
                .addContainerGap())
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, layout.createSequentialGroup()
                .addContainerGap()
                .addComponent(jPanel1, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(lblMode)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(sclMode, javax.swing.GroupLayout.DEFAULT_SIZE, 173, Short.MAX_VALUE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(lblQuality)
                    .addComponent(cmbQuality, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(chkFS)
                    .addComponent(chkVsync))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                        .addComponent(chkMusic)
                        .addComponent(txtAA, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addComponent(jLabel1)
                        .addComponent(lblVol))
                    .addComponent(barVol, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(sep, javax.swing.GroupLayout.PREFERRED_SIZE, 10, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(btnStart)
                    .addComponent(btnCancel))
                .addContainerGap())
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

    private void cmbFreqItemStateChanged(java.awt.event.ItemEvent evt) {//GEN-FIRST:event_cmbFreqItemStateChanged
        updateModeList();
    }//GEN-LAST:event_cmbFreqItemStateChanged

    private void cmbBPPItemStateChanged(java.awt.event.ItemEvent evt) {//GEN-FIRST:event_cmbBPPItemStateChanged
        updateModeList();
    }//GEN-LAST:event_cmbBPPItemStateChanged

    private void cmbResItemStateChanged(java.awt.event.ItemEvent evt) {//GEN-FIRST:event_cmbResItemStateChanged
        updateModeList();
    }//GEN-LAST:event_cmbResItemStateChanged

    private void btnStartActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_btnStartActionPerformed
        DisplayMode mode = ((DisplayModeWrapper)lstMode.getSelectedValue()).dm;
        
        settings.setRenderer("LWJGL");
        settings.setWidth(mode.getWidth());
        settings.setHeight(mode.getHeight());
        settings.setFrequency(mode.getRefreshRate());
        settings.setDepth(mode.getBitDepth());
        settings.setVerticalSync(chkVsync.isSelected());
        
        settings.setDepthBits(8);
        
        settings.setFullscreen(chkFS.isSelected());
        
        settings.setMusic(!chkMusic.isSelected());
        settings.setSFX(false);
        
        settings.setAlphaBits(0);
        settings.setStencilBits(0);
        settings.setSamples(Integer.parseInt(txtAA.getText()));
        
        settings.setFloat("GameMusicVolume", ((float)barVol.getValue()) / 100f );
        settings.setInt("GameTextureQuality", cmbQuality.getSelectedIndex());
        
        setVisible(false);
        
        synchronized (activeLock){
            activeLock.notifyAll();
        }
    }//GEN-LAST:event_btnStartActionPerformed

    private void btnCancelActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_btnCancelActionPerformed
        System.exit(0);
    }//GEN-LAST:event_btnCancelActionPerformed

    private void formWindowClosed(java.awt.event.WindowEvent evt) {//GEN-FIRST:event_formWindowClosed
        System.exit(0);
    }//GEN-LAST:event_formWindowClosed

    private void chkMusicActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_chkMusicActionPerformed
        barVol.setEnabled(!chkMusic.isSelected());
        lblVol.setEnabled(!chkMusic.isSelected());
    }//GEN-LAST:event_chkMusicActionPerformed
   
    
    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JSlider barVol;
    private javax.swing.JButton btnCancel;
    private javax.swing.JButton btnStart;
    private javax.swing.JCheckBox chkFS;
    private javax.swing.JCheckBox chkMusic;
    private javax.swing.JCheckBox chkVsync;
    private javax.swing.JComboBox cmbBPP;
    private javax.swing.JComboBox cmbFreq;
    private javax.swing.JComboBox cmbQuality;
    private javax.swing.JComboBox cmbRes;
    private javax.swing.JLabel jLabel1;
    private javax.swing.JPanel jPanel1;
    private javax.swing.JLabel lblBPP;
    private javax.swing.JLabel lblFreq;
    private javax.swing.JLabel lblMode;
    private javax.swing.JLabel lblQuality;
    private javax.swing.JLabel lblRes;
    private javax.swing.JLabel lblVol;
    private javax.swing.JList lstMode;
    private javax.swing.JScrollPane sclMode;
    private javax.swing.JSeparator sep;
    private javax.swing.JTextField txtAA;
    // End of variables declaration//GEN-END:variables
    
}
