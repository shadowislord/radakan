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

package com.gibbon.radakan.error;

public class ErrorFrame extends javax.swing.JFrame {
    
    /** Creates new form ErrorFrame */
    public ErrorFrame(String description, Throwable ex) {
        StackTraceElement firstElement = ex.getStackTrace()[0];
        
        StringBuffer report = new StringBuffer();
        report.append(description).append('\n');
        report.append(ex.getClass().getSimpleName()+": "+ex.getLocalizedMessage()).append('\n');
        report.append(" at ")
              .append(firstElement.getFileName())
              .append(':')
              .append(firstElement.getLineNumber())
              .append('\n');
        
        initComponents();
        txtReport.setText(report.toString());
    }
    
    /** This method is called from within the constructor to
     * initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is
     * always regenerated by the Form Editor.
     */
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        lblError1 = new javax.swing.JLabel();
        lblError3 = new javax.swing.JLabel();
        lblError2 = new javax.swing.JLabel();
        lblLogo = new javax.swing.JLabel();
        btnClose = new javax.swing.JButton();
        pnlBorder = new javax.swing.JPanel();
        jScrollPane1 = new javax.swing.JScrollPane();
        txtReport = new javax.swing.JTextArea();

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);

        lblError1.setFont(new java.awt.Font("Bookman Old Style", 2, 24));
        lblError1.setText("An error has occured..");

        lblError3.setText("Please send error.log to the Radakan staff as soon as possible.");

        lblError2.setText("A log file has been created to address the error.");

        lblLogo.setIcon(new javax.swing.ImageIcon(getClass().getResource("/splash.png"))); // NOI18N

        btnClose.setText("Close");
        btnClose.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btnCloseActionPerformed(evt);
            }
        });

        pnlBorder.setBorder(javax.swing.BorderFactory.createTitledBorder("Description: "));

        txtReport.setColumns(20);
        txtReport.setEditable(false);
        txtReport.setRows(5);
        jScrollPane1.setViewportView(txtReport);

        org.jdesktop.layout.GroupLayout pnlBorderLayout = new org.jdesktop.layout.GroupLayout(pnlBorder);
        pnlBorder.setLayout(pnlBorderLayout);
        pnlBorderLayout.setHorizontalGroup(
            pnlBorderLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(pnlBorderLayout.createSequentialGroup()
                .addContainerGap()
                .add(jScrollPane1, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 515, Short.MAX_VALUE)
                .addContainerGap())
        );
        pnlBorderLayout.setVerticalGroup(
            pnlBorderLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(pnlBorderLayout.createSequentialGroup()
                .add(jScrollPane1, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 89, Short.MAX_VALUE)
                .addContainerGap())
        );

        org.jdesktop.layout.GroupLayout layout = new org.jdesktop.layout.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(lblLogo)
            .add(layout.createSequentialGroup()
                .addContainerGap()
                .add(lblError1))
            .add(layout.createSequentialGroup()
                .add(18, 18, 18)
                .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                    .add(lblError2)
                    .add(lblError3))
                .addContainerGap(245, Short.MAX_VALUE))
            .add(org.jdesktop.layout.GroupLayout.TRAILING, layout.createSequentialGroup()
                .addContainerGap(498, Short.MAX_VALUE)
                .add(btnClose)
                .addContainerGap())
            .add(layout.createSequentialGroup()
                .addContainerGap()
                .add(pnlBorder, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addContainerGap())
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(layout.createSequentialGroup()
                .add(lblLogo)
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.UNRELATED)
                .add(lblError1)
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(lblError2)
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(lblError3)
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.UNRELATED)
                .add(pnlBorder, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.UNRELATED)
                .add(btnClose)
                .addContainerGap())
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

    private void btnCloseActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_btnCloseActionPerformed
        dispose();
        System.exit(1);
    }//GEN-LAST:event_btnCloseActionPerformed
    
    /**
     * @param args the command line arguments
     */
    public static void main(String args[]) {
        java.awt.EventQueue.invokeLater(new Runnable() {
            public void run() {
                new ErrorFrame("The parameters are invalid. ", new NumberFormatException("Cannot parse ttya\\")).setVisible(true);
            }
        });
    }
    
    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JButton btnClose;
    private javax.swing.JScrollPane jScrollPane1;
    private javax.swing.JLabel lblError1;
    private javax.swing.JLabel lblError2;
    private javax.swing.JLabel lblError3;
    private javax.swing.JLabel lblLogo;
    private javax.swing.JPanel pnlBorder;
    private javax.swing.JTextArea txtReport;
    // End of variables declaration//GEN-END:variables
    
}
