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
 * Trida na zobrazeni souboru na serveru
 * @author xhalak00
 * @author xzaple29
 */
class ShowFiles extends JOptionPane
    {
        private String file=null;
        private String version=null;
        /**
         * Funkce, ktera vrati jmeno souboru, ktery se bude nacitat.
         * @param files
         * @param myPaths
         * @return pole souboru.
         */
        public String[] ShowFiles(Vector<String> files,final Vector<filePath> myPaths)
        {

        class GetData extends JDialog implements ActionListener
            {
            JComboBox fileChooser;
            JComboBox versionChooser;
            JButton btnOK = new JButton("   OK   ");
            JButton btnHelp = new JButton("help");
            JButton btnCancel = new JButton("Cancel");
            JButton btnInfo = new JButton("  Show info  ");
            JButton btnSim = new JButton("Show simulated");
            JTextField browser = new JTextField();
            String returnlogin = null;
            String returnPasswd = null;
            boolean showDifferent = false;
            /**
             * Funkce na ziskani a zobrazeni stavajicich souboru na serveru.
             */
            public GetData(final Vector<String> files,final Vector<filePath> myPaths)
            {
                setModal(true);
                 browser.setPreferredSize(new Dimension(320,28));
                getContentPane().setLayout(new BorderLayout());
                
                setDefaultCloseOperation(DISPOSE_ON_CLOSE);
                setLocation(400,300);
                btnInfo.addActionListener(this);
                btnSim.addActionListener(this);
                btnHelp.addActionListener(this);
                //tlacitka a texfield na hledani
                JPanel buttonsInfo = new JPanel();
                buttonsInfo.setLayout(new BorderLayout());
                JPanel mainPanel = new JPanel();
                mainPanel.setLayout(new BorderLayout());
                JPanel buttons = new JPanel();
                buttons.setLayout(new BorderLayout());
                buttons.add(btnInfo,BorderLayout.WEST);
                buttons.add(btnHelp,BorderLayout.CENTER);
                buttons.add(btnSim,BorderLayout.EAST);
                JLabel liveFeed = new JLabel("Search on server");
                buttonsInfo.add(buttons,BorderLayout.NORTH);
                buttonsInfo.add(liveFeed,BorderLayout.CENTER);
                buttonsInfo.add(browser,BorderLayout.SOUTH);

                
                //filechooser
                JPanel myFilesPanel = new JPanel();
                myFilesPanel.setLayout(new BorderLayout());

                //versionchooser
                JPanel myVersionChooser=new JPanel();
                myVersionChooser.setLayout(new BorderLayout());

                //uchovani komboboxu
                JPanel comboBoxes = new JPanel();
                comboBoxes.setLayout(new BorderLayout());
                fileChooser = new JComboBox(files);
                /**
                 * Cyklus na zobrazeni jednotlivych projektu na serveru
                 */
                for (filePath versions:myPaths)
                {
                    if (versions.getFile().equals((String)fileChooser.getSelectedItem()))
                    {
                        versionChooser = new JComboBox(versions.getFiles());
                        versionChooser.setSelectedItem(versions.getLast());
                    }
                }
                browser.addActionListener(new java.awt.event.ActionListener() {
                    /**
                     * Funkce na preskupeni a vyber jinych polozek, pokud uzivatel zada neco do textoveho pole.
                     */
                    public void actionPerformed(java.awt.event.ActionEvent evt) {
                        showDifferent = false;
                        Vector<String> options=new Vector<String>();
                        file = (String)fileChooser.getSelectedItem();
                        String author = "author: "+browser.getText();
                        String changed = "Last changed by: "+browser.getText();
                        for (filePath versions:myPaths)
                        {
                            if (versions.getFile().contains(browser.getText()))
                            {
                                System.out.println(browser.getText());
                                System.out.println(versions.getFile());
                                options.add(versions.getFile());
                                fileChooser.setSelectedItem(versions.getFile());
                                browser.setText(browser.getText());
                            }
                            else if (versions.getIntel().contains(author)||versions.getIntel().contains(changed)||versions.getSimulated().contains(browser.getText())){
                                options.add(versions.getFile());
                                showDifferent = true;
                            }
                        }
                        if (showDifferent || browser.getText().equals(""))
                        {
                            fileChooser.setModel(new JComboBox(options).getModel());
                            file = (String)fileChooser.getSelectedItem();
                            for (filePath versions:myPaths)
                            {
                                if (versions.getFile().equals(file))
                                {
                                    versionChooser.setModel(new JComboBox(versions.getFiles()).getModel());
                                    versionChooser.setSelectedItem(versions.getLast());
                                }
                            }
                        } 

                    }
                });
                fileChooser.addActionListener(new java.awt.event.ActionListener() {
                    /**
                     * Funkce na zmenu zobrazenych verzi
                     */
                    public void actionPerformed(java.awt.event.ActionEvent evt) {
                        if(evt.getSource() == fileChooser){                            
                            JComboBox cb = (JComboBox)evt.getSource();
                            file = (String)cb.getSelectedItem();
                            for (filePath versions:myPaths)
                            {
                                if (versions.getFile().equals(file))
                                {
                                    versionChooser.setModel(new JComboBox(versions.getFiles()).getModel());
                                    versionChooser.setSelectedItem(versions.getLast());
                                }
                            }
                        }
                    }
                });

                JLabel info = new JLabel("Choose file on server");
                JLabel info2 = new JLabel("Choose version");
                myFilesPanel.add(info,BorderLayout.NORTH);
                myFilesPanel.add(fileChooser,BorderLayout.CENTER);
                
                myVersionChooser.add(info2,BorderLayout.NORTH);
                if (versionChooser!=null)
                    myVersionChooser.add(versionChooser,BorderLayout.CENTER);
                comboBoxes.add(myFilesPanel,BorderLayout.NORTH);
                if (versionChooser!=null)
                    comboBoxes.add(myVersionChooser,BorderLayout.CENTER);
                mainPanel.add(buttonsInfo,BorderLayout.NORTH);
                mainPanel.add(comboBoxes,BorderLayout.CENTER);
                getContentPane().add(mainPanel,BorderLayout.NORTH);
                
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
             * Tjisteni na ktere tlacitko bylo kliknuto a podle toho zobrazim, nebo navratim data.
             */
            public void actionPerformed(ActionEvent e)
            {
                if (e.getSource() == btnOK){
                    file = (String)fileChooser.getSelectedItem();
                    version = (String)versionChooser.getSelectedItem();
                    dispose();
                }
                else if (e.getSource() == btnCancel)
                    dispose();
                 if (e.getSource() == btnInfo){
                   file = (String)fileChooser.getSelectedItem();
                   for (filePath versions:myPaths)
                    {
                        if (versions.getFile().equals(file))
                        {
                            JOptionPane.showMessageDialog(rootPane, versions.getIntel(), "info",JOptionPane.INFORMATION_MESSAGE);
                        }
                    }
                 }
                if (e.getSource() == btnSim){
                   file = (String)fileChooser.getSelectedItem();
                   for (filePath versions:myPaths)
                    {
                        if (versions.getFile().equals(file))
                        {
                            JOptionPane.showMessageDialog(rootPane, versions.getSimulated(), "Last Simulated by",JOptionPane.INFORMATION_MESSAGE);
                        }
                    }
                 }
                if (e.getSource() == btnHelp){
                    JOptionPane.showMessageDialog(rootPane,"Search on server: Type the name of file or User and it will select files\n" +
                            "that user changed, created and or simulated.\n" +
                            "It also select the name of file, if you type it in there.\n" +
                            "\n" +
                            "Choose file on server: You can choose files on server by clicking on this combobox.\n" +
                            "Choose version: Choose your version of file you wish to open.\n" +
                            "\n" +
                            "Click on OK to load the file or Cancel to close this form.\n" , "Help",JOptionPane.INFORMATION_MESSAGE);
                }

            }
        }
        new GetData(files,myPaths);
        return new String[] {file, version};
        }

}
