package client.GUI;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.Vector;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JTextField;
import petrinets.filePath;

/**
 * Trida na zobrazeni ukladaciho okna.
 * @author xhalak00
 * @author xzaple29
 */
class saveFile extends JOptionPane
    {
        private String file=null;
        /**
         * Funkce na zobrazeni ukldadanych dat
         * @param files jednotlive soubory.
         * @param myPaths jednotlive verze
         * @return jmeno ukladaneho souboru.
         */
        public String ShowFiles(Vector<String> files,Vector<filePath> myPaths)
        {
        class GetData extends JDialog implements ActionListener
            {
            JComboBox fileChooser;
            JTextField myFile=new JTextField();
            JButton btnOK = new JButton("   OK   ");
            JButton btnCancel = new JButton("Cancel");
            /**
             * Funkce na zobrazeni jednotlivych dat.
             */
            public GetData(Vector<String> files,final Vector<filePath> myPaths)
            {
                setModal(true);
                getContentPane().setLayout(new BorderLayout());
                getContentPane().setPreferredSize(new Dimension(320,140));
                setDefaultCloseOperation(DISPOSE_ON_CLOSE);
                setLocation(400,300);
                JPanel myFilesPanel = new JPanel();
                myFilesPanel.setLayout(new BorderLayout());
                JPanel myVersionChooser=new JPanel();
                myVersionChooser.setLayout(new BorderLayout());
                fileChooser = new JComboBox(files);
                JLabel first = new JLabel("Choose existing file");
                myFilesPanel.add(first,BorderLayout.NORTH);
                myFilesPanel.add(fileChooser,BorderLayout.CENTER);
                /**
                 * Pokud vybere uzivatel nejaky soubor na serveru, zobrazim jej v textfieldu.
                 */
                fileChooser.addActionListener(new java.awt.event.ActionListener() {
                    public void actionPerformed(java.awt.event.ActionEvent evt) {
                        if(evt.getSource() == fileChooser){
                            JComboBox cb = (JComboBox)evt.getSource();
                            file = (String)cb.getSelectedItem();
                            myFile.setText(file);
                        }
                    }
                });
                JLabel info = new JLabel("Or type name of the new file");
                myVersionChooser.add(info,BorderLayout.NORTH);
                myVersionChooser.add(myFile,BorderLayout.CENTER);
                getContentPane().add(myFilesPanel,BorderLayout.NORTH);
                getContentPane().add(myVersionChooser,BorderLayout.CENTER);
                JPanel jp = new JPanel();
                btnOK.addActionListener(this);
                btnCancel.addActionListener(this);
                jp.add(btnOK);
                jp.add(btnCancel);
                getContentPane().add(jp,BorderLayout.SOUTH);
                pack();
                setVisible(true);
            }
            /**
             * Po kliknuto na OK vratit string.
             */
            public void actionPerformed(ActionEvent e)
            {
                if (e.getSource() == btnOK){
                    if (myFile.getText().equals(""))
                        file = (String)fileChooser.getSelectedItem();
                    else
                        file = myFile.getText();
                    dispose();
                }
                else if (e.getSource() == btnCancel)
                    dispose();

            }
        }
        new GetData(files,myPaths);
        return file;
        }
}

