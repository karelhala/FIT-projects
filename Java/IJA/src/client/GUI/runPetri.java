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
 * Trida na zobrazeni okna, ktere soubory mame spustit.
 * @author xhalak00
 * @author xzaple29
 */
class runPetri extends JOptionPane
    {
        private String file=null;
        /**
         * Funkce na zobrazeni jednotlivych souboru
         * @param files soubory na serveru.
         * @param myPaths jednotlive cesty.
         * @return soubor, ktery ma server simulovat.
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
             * Rozvrzeni a zobrazeni dat v okne.
             */
            public GetData(Vector<String> files,final Vector<filePath> myPaths)
            {
                setModal(true);
                getContentPane().setLayout(new BorderLayout());
                getContentPane().setPreferredSize(new Dimension(320,90));
                setDefaultCloseOperation(DISPOSE_ON_CLOSE);
                setLocation(400,300);
                JPanel myFilesPanel = new JPanel();
                myFilesPanel.setLayout(new BorderLayout());
                fileChooser = new JComboBox(files);
                JLabel first = new JLabel("Run petrinet on server.");
                myFilesPanel.add(first,BorderLayout.NORTH);
                myFilesPanel.add(fileChooser,BorderLayout.CENTER);
                getContentPane().add(myFilesPanel,BorderLayout.NORTH);
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
             * Po kliknuti na jedno, nebo druhe tlacitko vratime jemno souboru.
             */
            public void actionPerformed(ActionEvent e)
            {
                if (e.getSource() == btnOK){
                    file = (String)fileChooser.getSelectedItem();
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
