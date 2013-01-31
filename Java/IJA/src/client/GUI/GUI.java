/**
 *
 * @author xhalak00
 * @author xzaple29
 */
package client.GUI;
import client.client;
import com.thoughtworks.xstream.XStream;
import com.thoughtworks.xstream.io.xml.DomDriver;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.logging.Level;
import java.util.logging.Logger;
import petrinets.PetriNet;
import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.awt.geom.AffineTransform;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileReader;
import java.io.FileWriter;
import java.lang.reflect.Field;
import java.nio.MappedByteBuffer;
import java.nio.channels.FileChannel;
import java.nio.charset.Charset;
import java.util.*;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import petrinets.Place;
import petrinets.Transition;
import petrinets.filePath;
import petrinets.xmlpacket;
/**
 * Funkce obalujici kompletne cely program s GUI
 *
*/

public class GUI extends JPanel
                         implements MouseListener{
    int idPlace=1;
    int idTransition=1;
    private Dimension area;
    private PetriNet Mypetri=null;
    settingsClass MySettings=null;
    Vector<Object> vektor = new Vector<Object>();
    private int clicks=0;
    Place FirstPlace = null;
    Place SecondPlace = null;
    Transition FirstTransition = null;
    Transition SecondTransition = null;
    Place MovePlace = null;
    Transition MoveTransition = null;
    Place MoveMe = null;
    String name, PW;
    private JPanel drawingPane;
    JTabbedPane tabs;
    JScrollPane scroller;
    int EndX=0;
    int EndY=0;
    boolean start,end;
    String address="localhost";
    String netName=null;
    String info=null;
    String simulated=null;
    Vector<PetriNet> tabbedPetries;
    int changedFrom=0;
    /**
     * Kontruktor pro identifikaci vsech objektu v okne
     */
    public GUI() {
        super(new BorderLayout());
        jMenuBar1 = new javax.swing.JMenuBar();
        jMenu1 = new javax.swing.JMenu();
        jMenu2 = new javax.swing.JMenu();
        jMenu3 = new javax.swing.JMenu();
        jMenuItem1 = new javax.swing.JMenuItem();
        jMenuItem2 = new javax.swing.JMenuItem();
        jMenuItem3 = new javax.swing.JMenuItem();
        jMenuItem4 = new javax.swing.JMenuItem();
        jMenuItem5 = new javax.swing.JMenuItem();
        jMenuItem9 = new javax.swing.JMenuItem();
        jMenuItem10 = new javax.swing.JMenuItem();
        jMenuItem11 = new javax.swing.JMenuItem();
        jMenuItem12 = new javax.swing.JMenuItem();
        jMenuItem13 = new javax.swing.JMenuItem();
        jMenuItem14 = new javax.swing.JMenuItem();
        jMenuItem15 = new javax.swing.JMenuItem();
        jMenuItem16 = new javax.swing.JMenuItem();
        jToggleButton1 = new javax.swing.JToggleButton();
        jToggleButton2 = new javax.swing.JToggleButton();
        jToggleButton3 = new javax.swing.JToggleButton();
        jToggleButton4 = new javax.swing.JToggleButton();
        objectGroup = new javax.swing.ButtonGroup();
        jToolBar1 = new javax.swing.JToolBar();
        jButton3 = new javax.swing.JButton();
        jSeparator2 = new javax.swing.JToolBar.Separator();
        jSeparator4 = new javax.swing.JToolBar.Separator();
        jSeparator5 = new javax.swing.JToolBar.Separator();
        jSeparator6 = new javax.swing.JToolBar.Separator();
        area = new Dimension(0,0);
        fc = new JFileChooser();
        Mypetri = new PetriNet();
        statusBar = new javax.swing.JLabel();
        petriName = new javax.swing.JLabel("");
        labelConditions = new javax.swing.JLabel("Conditions");
        labelOutputs = new javax.swing.JLabel("Outputs");
        jTextField1 = new javax.swing.JTextField();
        jTextField2 = new javax.swing.JTextField();
        jSeparator3 = new javax.swing.JToolBar.Separator();
        jButton4 = new javax.swing.JButton();
        jButton5 = new javax.swing.JButton();
        jButton6 = new javax.swing.JButton();
        jButton7 = new javax.swing.JButton();
        jMenuItem7 = new javax.swing.JMenuItem();
        jMenuItem8 = new javax.swing.JMenuItem();
        labelConditions.setVisible(false);
        labelOutputs.setVisible(false);
        jTextField1.setVisible(false);
        jTextField2.setVisible(false);
        jButton4.setVisible(false);
        tabbedPetries = new Vector<PetriNet>();
        //polozka v menu pro uchovani file

        jMenu1.setText("File");
        jMenuBar1.add(jMenu1);
        jMenu2.setText("settings");
        jMenuBar1.add(jMenu2);
        // tlacitko pro lokalni uloziste
        jMenuItem7.setText("Save");
        jMenuItem7.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                try {
                    jMenuItem7ActionPerformed(evt);
                } catch (IOException ex) {
                    Logger.getLogger(GUI.class.getName()).log(Level.SEVERE, null, ex);
                }
            }
        });
        jMenu1.add(jMenuItem7);

        //tlacitko pro nahrani z lokalniho uloziste
        jMenuItem8.setText("Load");
        jMenuItem8.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                try {
                    jMenuItem8ActionPerformed(evt);
                } catch (FileNotFoundException ex) {
                    Logger.getLogger(GUI.class.getName()).log(Level.SEVERE, null, ex);
                } catch (IOException ex) {
                    Logger.getLogger(GUI.class.getName()).log(Level.SEVERE, null, ex);
                }
            }
        });
        jMenu1.add(jMenuItem8);

        //tlacitko na zobrazi informaci o siti
        jMenuItem12.setText("info");
        jMenuItem12.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jMenuItem12ActionPerformed(evt);
            }
        });
        jMenu1.add(jMenuItem12);

        //tlacitko na zobrazeni historie prechodu
        jMenuItem13.setText("show history");
        jMenuItem13.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jMenuItem13ActionPerformed(evt);
            }
        });
        jMenu1.add(jMenuItem13);

        //tlacitko na zmenu nastaveni
        jMenuItem14.setText("Change settings");
        jMenuItem14.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jMenuItem14ActionPerformed(evt);
            }
        });
        jMenu2.add(jMenuItem14);

        //tlacitko na export nastaveni
        jMenuItem15.setText("Save settings");
        jMenuItem15.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                try {
                    jMenuItem15ActionPerformed(evt);
                } catch (IOException ex) {
                    Logger.getLogger(GUI.class.getName()).log(Level.SEVERE, null, ex);
                }
            }
        });
        jMenu2.add(jMenuItem15);

        //tlacitko na import nastaveni
        jMenuItem16.setText("Load settings");
        jMenuItem16.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                try {
                    jMenuItem16ActionPerformed(evt);
                } catch (FileNotFoundException ex) {
                    Logger.getLogger(GUI.class.getName()).log(Level.SEVERE, null, ex);
                } catch (IOException ex) {
                    Logger.getLogger(GUI.class.getName()).log(Level.SEVERE, null, ex);
                }
            }
        });
        jMenu2.add(jMenuItem16);
        //polozka v menu pro uchovani server->connect, load, save a simulate
        jMenu3.setText("server");
        jMenuItem11.setText("server address");
        jMenuItem11.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jMenuItem11ActionPerformed(evt);
            }
        });
        jMenu3.add(jMenuItem11);

        //tlacitko na registraci uzivatele
        jMenuItem9.setText("register");
        jMenuItem9.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jMenuItem9ActionPerformed(evt);
            }
        });
        jMenu3.add(jMenuItem9);

        //tlacitko na prihlaseni se k serveru
        jMenuItem1.setText("log in");
        jMenuItem1.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jMenuItem1ActionPerformed(evt);
            }
        });
        jMenu3.add(jMenuItem1);

        //tlacitko na nahrani dat ze serveru
        jMenuItem2.setText("load");
        jMenuItem2.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jMenuItem2ActionPerformed(evt);
            }
        });
        jMenu3.add(jMenuItem2);

        //tlacitko na ulozeni dat na server
        jMenuItem3.setText("save");
        jMenuItem3.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jMenuItem3ActionPerformed(evt);
            }
        });
        jMenu3.add(jMenuItem3);

        //tlacitko na susteni site na serveru
        jMenuItem4.setText("run");
        jMenuItem4.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jMenuItem4ActionPerformed(evt);
            }
        });
        jMenu3.add(jMenuItem4);

        //tlacitko na simulaci site
        jMenuItem5.setText("simulate");
        jMenuItem5.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jMenuItem5ActionPerformed(evt);
            }
        });
        jMenu3.add(jMenuItem5);

        //tlacitko na odsimulovani jednoho kroku
        jMenuItem10.setText("simulate one step");
        jMenuItem10.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jMenuItem10ActionPerformed(evt);
            }
        });
        jMenu3.add(jMenuItem10);

        jMenuBar1.add(jMenu3);

        //tlacitko na pridani jednoho mista
        objectGroup.add(jToggleButton1);
        jToggleButton1.setText("Place");
        jToggleButton1.setFocusable(false);
        jToggleButton1.setHorizontalTextPosition(javax.swing.SwingConstants.CENTER);
        jToggleButton1.setVerticalTextPosition(javax.swing.SwingConstants.BOTTOM);
        jToggleButton1.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jToggleButton1ActionPerformed(evt);
            }
        });
        jToolBar1.add(jToggleButton1);
        //tlacitko na pridani podminky
        objectGroup.add(jToggleButton2);
        jToggleButton2.setText("Transition");
        jToggleButton2.setFocusable(false);
        jToggleButton2.setHorizontalTextPosition(javax.swing.SwingConstants.CENTER);
        jToggleButton2.setVerticalTextPosition(javax.swing.SwingConstants.BOTTOM);
        jToggleButton2.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jToggleButton2ActionPerformed(evt);
            }
        });
        jToolBar1.add(jToggleButton2);
        //pridani prechodu
        objectGroup.add(jToggleButton3);
        jToggleButton3.setText(" Join ");
        jToggleButton3.setFocusable(false);
        jToggleButton3.setHorizontalTextPosition(javax.swing.SwingConstants.CENTER);
        jToggleButton3.setVerticalTextPosition(javax.swing.SwingConstants.BOTTOM);
        jToggleButton3.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jToggleButton3ActionPerformed(evt);
            }
        });
        jToolBar1.add(jToggleButton3);
        //smazani polozky
        objectGroup.add(jToggleButton4);
        jToggleButton4.setText("Delete");
        jToggleButton4.setFocusable(false);
        jToggleButton4.setHorizontalTextPosition(javax.swing.SwingConstants.CENTER);
        jToggleButton4.setVerticalTextPosition(javax.swing.SwingConstants.BOTTOM);
        jToggleButton4.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jToggleButton4ActionPerformed(evt);
            }
        });
        jToolBar1.add(jToggleButton4);
        jToolBar1.add(jSeparator2);
        //vymazani cele pracovni plochy
        jButton3.setText("Clear All");
        jButton3.setFocusable(false);
        jButton3.setHorizontalTextPosition(javax.swing.SwingConstants.CENTER);
        jButton3.setVerticalTextPosition(javax.swing.SwingConstants.BOTTOM);
        jButton3.addMouseListener(new java.awt.event.MouseAdapter() {
            @Override
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                jButton3MouseClicked(evt);
            }
        });
        jToolBar1.add(jButton3);
        jToolBar1.add(jSeparator3);
        jToolBar1.add(labelConditions);
        jToolBar1.add(jTextField1);
        jToolBar1.add(labelOutputs);
        jToolBar1.add(jTextField2);
        //tlacitko na update podminky
         jButton4.setText("Update");
        jButton4.setFocusable(false);
        jButton4.setHorizontalTextPosition(javax.swing.SwingConstants.CENTER);
        jButton4.setVerticalTextPosition(javax.swing.SwingConstants.BOTTOM);
        jButton4.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButton4ActionPerformed(evt);
            }
        });
        jToolBar1.add(jButton4);

        //tlacitko na pridani panelu
        jButton5.setText("Add panel");
        jButton5.setFocusable(false);
        jButton5.setHorizontalTextPosition(javax.swing.SwingConstants.CENTER);
        jButton5.setVerticalTextPosition(javax.swing.SwingConstants.BOTTOM);
        jButton5.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButton5ActionPerformed(evt);
            }
        });
        jMenuBar1.add(jSeparator5);
        jMenuBar1.add(jButton5);

        //tlacitko na smazani panelu
        jButton6.setText("Remove panel");
        jButton6.setFocusable(false);
        jButton6.setHorizontalTextPosition(javax.swing.SwingConstants.CENTER);
        jButton6.setVerticalTextPosition(javax.swing.SwingConstants.BOTTOM);
        jButton6.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButton6ActionPerformed(evt);
            }
        });
        
        jMenuBar1.add(jButton6);
        jMenuBar1.add(jSeparator6);
        jButton7.setText("Help");
        jButton7.setFocusable(false);
        jButton7.setHorizontalTextPosition(javax.swing.SwingConstants.CENTER);
        jButton7.setVerticalTextPosition(javax.swing.SwingConstants.BOTTOM);
        jButton7.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButton7ActionPerformed(evt);
            }
        });
        jMenuBar1.add(jButton7);
        jToolBar1.add(jSeparator4);
        // status bar pro zobrazeni nekterych akci
         statusBar.setText("statusbar");
        JPanel instructionPanel = new JPanel(new GridLayout(0,1));
        instructionPanel.setFocusable(true);
        instructionPanel.add(jMenuBar1);
        instructionPanel.add(jToolBar1);

        //Nastaeveni kreslici plochy
        drawingPane = new DrawingPane();
        drawingPane.addMouseListener(this);


        //Vlozeni kresliciho platna do scroll panelu
        scroller = new JScrollPane(drawingPane);
        scroller.setPreferredSize(new Dimension(860,540));
        //Zalozky
        tabs= new JTabbedPane();
        tabs.addTab("new_panel", scroller);
        //pridani prazdne site
        tabbedPetries.add(Mypetri);
        tabs.addChangeListener(changeListener);

        //rozvrzeni okna
        add(instructionPanel, BorderLayout.PAGE_START);
        add(tabs, BorderLayout.CENTER);
        add(statusBar, BorderLayout.SOUTH);

        /**
         * Nacteni settings pri startu programu.
         */
        String line = "";
        String OutSettings = "";
        try {
            String readFile = "examples/settings/.actual.xml";
            BufferedReader reader = new BufferedReader(new FileReader(readFile));
            while((line = reader.readLine()) != null)
            {
            	OutSettings+=line;
            }

            reader.close();
            XStream xstream = new XStream(new DomDriver());
            MySettings = (settingsClass)xstream.fromXML(OutSettings);
        }
        catch (IOException e)
        {
            MySettings = new settingsClass();
        }
    }
    /**
     * Funkce na odchyceni zmeny zalozek
     */
    ChangeListener changeListener = new ChangeListener() {
      public void stateChanged(ChangeEvent changeEvent) {
        JTabbedPane sourceTabbedPane = (JTabbedPane) changeEvent.getSource();
        int index = sourceTabbedPane.getSelectedIndex();
        tabbedPetries.set(changedFrom, Mypetri);
        Mypetri=tabbedPetries.elementAt(index);
        JLabel old = new JLabel("not");
        FirstPlace=null;
        FirstTransition=null;
        labelConditions.setVisible(false);
        labelOutputs.setVisible(false);
        jTextField1.setVisible(false);
        jTextField2.setVisible(false);
        jButton4.setVisible(false);
        try{
            tabs.setComponentAt(changedFrom, old);
            tabs.setComponentAt(index, scroller);
            changedFrom=index;
            idPlace = Mypetri.getPlaces().size()+1;
            idTransition = Mypetri.getTransitions().size()+1;
        }
        catch (IndexOutOfBoundsException e)
        {
            
        }
      }
    };
    /**
     * Funkce na ulozeni dat na lokalnim ulozisti
     * @param evt prace s mysi
     * @throws IOException prace se soubory
     */
    private void jMenuItem7ActionPerformed(java.awt.event.ActionEvent evt) throws IOException {
         try {
            File f = new File(new File(".").getCanonicalPath());
            fc.setCurrentDirectory(f);
        } catch (IOException e) {
        }
        int returnVal = fc.showSaveDialog(GUI.this);
        if (returnVal == JFileChooser.APPROVE_OPTION) {
            File file = fc.getSelectedFile();
            if (Mypetri!=null){
                XStream xstream = new XStream(new DomDriver());
                String Myxml = xstream.toXML(Mypetri);
                FileWriter fstream = new FileWriter(file.getPath());
                BufferedWriter out = new BufferedWriter(fstream);
                out.write(Myxml);
                //Close the output stream
                out.close();
            }
            statusBar.setText("Saving: " + file.getPath());
        } else {
            statusBar.setText("Save command cancelled by user.");
        }
    }

    /**
     * Funkce na nacteni dat z lokalniho uloziste
     * @param evt prace s mysi
     * @throws FileNotFoundException prace s nacitaim souboru
     * @throws IOException odchyceni chyby
     */
    private void jMenuItem8ActionPerformed(java.awt.event.ActionEvent evt) throws FileNotFoundException, IOException {
        try {
            File f = new File(new File(".").getCanonicalPath());
            fc.setCurrentDirectory(f);
        } catch (IOException e) {
        }
       int returnVal = fc.showOpenDialog(GUI.this);
        String Mystring="";
        if (returnVal == JFileChooser.APPROVE_OPTION) {
            File file = fc.getSelectedFile();
            FileInputStream stream = new FileInputStream(file.getPath());
            try {
                FileChannel filechannel = stream.getChannel();
                MappedByteBuffer bb = filechannel.map(FileChannel.MapMode.READ_ONLY, 0, filechannel.size());
                /* Instead of using default, pass in a decoder. */
                Mystring = Charset.defaultCharset().decode(bb).toString();
            }finally{
                stream.close();
            }
            XStream xstream = new XStream(new DomDriver());            
            tabbedPetries.add((PetriNet)xstream.fromXML(Mystring));
            JLabel loading = new JLabel("loading");
            int selIndex = tabs.getSelectedIndex();
            changedFrom = selIndex;
            tabs.addTab("local_net", loading);
            selIndex = tabs.getTabCount() - 1;
            //tabs.setComponentAt(selIndex, old);
            tabs.setSelectedIndex(selIndex);
        } else {
            statusBar.setText("Open command cancelled by user.");
        }
    }

    /**
     * Funkce na vlozeni noveho jmena serveru, automaticky je nastaveno local
     * @param evt prace s mysi
     */
    private void jMenuItem11ActionPerformed(java.awt.event.ActionEvent evt) {
        String string = (String) JOptionPane.showInputDialog("Enter name of the server",address);
        if (string!=null)
            address = string;
    }

    /**
     * Funkce na zobrazeni informaci o siti
     * @param evt
     */
    private void jMenuItem12ActionPerformed(java.awt.event.ActionEvent evt) {
        String showMe="Local net.";
        if (info!=null)
            JOptionPane.showMessageDialog(drawingPane, info, "info",JOptionPane.INFORMATION_MESSAGE);
        else{
            showMe+=Mypetri.getInfo();
            JOptionPane.showMessageDialog(drawingPane, showMe, "info",JOptionPane.INFORMATION_MESSAGE);
        }
    }
    /**
     * Funkce na zobrazeni uzivatelu, kteri simulovali dannou sit na serveru.
     * @param evt prace s mysi
     */
    private void jMenuItem13ActionPerformed(java.awt.event.ActionEvent evt) {
        if (simulated!=null)
            JOptionPane.showMessageDialog(drawingPane, simulated, "simulated by:",JOptionPane.INFORMATION_MESSAGE);
        else
            JOptionPane.showMessageDialog(drawingPane, "Local net.", "info",JOptionPane.INFORMATION_MESSAGE);
    }
    /**
     * Funkce na ulozeni zmeneneho nastaveni
     * @param evt prace s mysi
     */
    private void jMenuItem14ActionPerformed(java.awt.event.ActionEvent evt) {
        String[] saveOption = new showSettings().ShowFiles(MySettings.getAll());
        if (saveOption[0]!=null)
            MySettings.setAll(saveOption);
        try {
            // Create a File object containing the canonical path of the
            // desired directory
            XStream xstream = new XStream(new DomDriver());
            String Myxml = xstream.toXML(MySettings);
            boolean success = (new File("./examples/settings/")).mkdirs();
            FileWriter fstream = new FileWriter("./examples/settings/.actual.xml");
            BufferedWriter out = new BufferedWriter(fstream);
            out.write(Myxml);            
            out.close();
            // Set the current directory
        } catch (IOException e) {
        }
        
        drawingPane.repaint();

    }

    /**
     * Funkce na ulozeni nastaveni
     * @param evt prace s mysi
     * @throws IOException prace se soubory
     */
    private void jMenuItem15ActionPerformed(java.awt.event.ActionEvent evt) throws IOException {
        JFileChooser chooser = new JFileChooser();

        try {
            File f = new File(new File(".").getCanonicalPath());
            chooser.setCurrentDirectory(f);
        } catch (IOException e) {
        }
        int returnVal = chooser.showSaveDialog(GUI.this);
        if (returnVal == JFileChooser.APPROVE_OPTION) {
            File file = chooser.getSelectedFile();
            if (MySettings!=null){
                XStream xstream = new XStream(new DomDriver());
                String Myxml = xstream.toXML(MySettings);
                FileWriter fstream = new FileWriter(file.getPath());
                BufferedWriter out = new BufferedWriter(fstream);
                out.write(Myxml);
                //Close the output stream
                out.close();
            }
            statusBar.setText("Saving: " + file.getPath());
        } else {
            statusBar.setText("Save command cancelled by user.");
        }
    }
    /**
     * Funkce na nacteni nastaveni
     * @param evt prace s mysi
     * @throws FileNotFoundException exception na nelazeny soubor
     * @throws IOException exception na praci se soubory
     */
    private void jMenuItem16ActionPerformed(java.awt.event.ActionEvent evt) throws FileNotFoundException, IOException {
        try {
            File f = new File(new File(".").getCanonicalPath());
            // Set the current directory
            fc.setCurrentDirectory(f);
        } catch (IOException e) {
        }
       int returnVal = fc.showOpenDialog(GUI.this);
        String Mystring="";
        if (returnVal == JFileChooser.APPROVE_OPTION) {
            File file = fc.getSelectedFile();
            FileInputStream stream = new FileInputStream(file.getPath());
            try {
                FileChannel filechannel = stream.getChannel();
                MappedByteBuffer bb = filechannel.map(FileChannel.MapMode.READ_ONLY, 0, filechannel.size());
                /* Instead of using default, pass in a decoder. */
                Mystring = Charset.defaultCharset().decode(bb).toString();
            }finally{
                stream.close();
            }
            XStream xstream = new XStream(new DomDriver());
            MySettings = (settingsClass)xstream.fromXML(Mystring);
            drawingPane.repaint();
            statusBar.setText("opening: " + file.getPath());
            netName=null;
            idPlace = Mypetri.getPlaces().size()+1;
            idTransition = Mypetri.getTransitions().size()+1;
        } else {
            statusBar.setText("Open command cancelled by user.");
        }
    }

    /**
     * Funkce na nacteni site ze serveru a pote ulozeni do prislusneho tabu
     * @param evt prace s mysi
     */
    private void jMenuItem2ActionPerformed(java.awt.event.ActionEvent evt) {
        Vector <filePath> myPath = new Vector<filePath>();
        Vector <String> myFiles = new Vector <String>();
        System.out.println(name);
        xmlpacket MyData;
        myPath =client.loadFile(address,name,PW);
        if (name!=null&&PW!=null){
            for (filePath pom: myPath)
            {
                myFiles.add(pom.getFile());
            }
            String[] string = new ShowFiles().ShowFiles(myFiles,myPath);
            if (string[0]!=null&&string[1]!=null){
                MyData = client.loadPetri(name,PW,address,string[0],string[1]);
                if (MyData!=null){
                    //Mypetri = client.loadPetri(name,PW,address,string[0],string[1]);
                    info =MyData.getInfo().replaceAll("\t", "\n");
                    simulated = MyData.getSimulated().replaceAll("\t", "\n");

                    statusBar.setText(String.format("File was loaded %s", string[1]));
                    netName = string[1];
                    petriName.setText(string[1]);
                    drawingPane.repaint();

                    tabbedPetries.add(MyData.getPetri());
                    JLabel loading = new JLabel("loading");
                    int selIndex = tabs.getSelectedIndex();
                    changedFrom = selIndex;
                    tabs.addTab(string[1], loading);
                    selIndex = tabs.getTabCount() - 1;
                    //tabs.setComponentAt(selIndex, old);
                    tabs.setSelectedIndex(selIndex);
                    idPlace = Mypetri.getPlaces().size()+1;
                    idTransition = Mypetri.getTransitions().size()+1;
                }
            }
        }
        else
            JOptionPane.showMessageDialog(drawingPane, "You're not logged in!", "ERROR",JOptionPane.WARNING_MESSAGE);
        //JOptionPane.showMessageDialog(drawingPane, "Error while retrieving data!", "ERROR",JOptionPane.WARNING_MESSAGE);
    }
    /**
     * Funkce na prihlaseni uzivatele k serveru
     * @param evt prace s mysi
     */
     private void jMenuItem1ActionPerformed(java.awt.event.ActionEvent evt) {
        String[] loginWindow = new MyJOptionPane().showInputDialog();
        if (loginWindow[0]!=null && loginWindow[1]!=null){
            if (loginWindow[0].length()<4)
                JOptionPane.showMessageDialog(drawingPane, "Username is too short, use at least 4 characters", "ERROR",JOptionPane.WARNING_MESSAGE);
            else if (loginWindow[1].length()<4)
                JOptionPane.showMessageDialog(drawingPane, "Password is too short, use at least 4 characters", "ERROR",JOptionPane.WARNING_MESSAGE);
            else
            {

                if (client.logIn(address,loginWindow[0],loginWindow[1])==false){
                    JOptionPane.showMessageDialog(drawingPane, "Wrong username or pasword", "ERROR",JOptionPane.WARNING_MESSAGE);
                    name =null;
                    PW=null;
                }else{
                    name = loginWindow[0];
                    PW = loginWindow[1];
                    statusBar.setText("You were logged in.");
                }
            }
        }
    }

     /**
      * Funkce na pridani noveho uzivatele
      * @param evt prace s mysi
      */
    private void jMenuItem9ActionPerformed(java.awt.event.ActionEvent evt) {
        String[] loginWindow = new MyJOptionPane().showInputDialog();
        if (loginWindow[0]!=null && loginWindow[1]!=null){
            if (loginWindow[0].length()<4)
                JOptionPane.showMessageDialog(drawingPane, "Moc kratke jmeno, pouzijte nejmene 4 znaky", "ERROR",JOptionPane.WARNING_MESSAGE);
            else if (loginWindow[1].length()<4)
                JOptionPane.showMessageDialog(drawingPane, "Moc kratke heslo, pouzijte nejmene 4 znaky", "ERROR",JOptionPane.WARNING_MESSAGE);
            else
               if (client.register(address,loginWindow[0],loginWindow[1])==false)
                    JOptionPane.showMessageDialog(drawingPane, "User allready exists!", "ERROR",JOptionPane.WARNING_MESSAGE);
               else
                    JOptionPane.showMessageDialog(drawingPane, "User sucsefully aded.", "User added",JOptionPane.INFORMATION_MESSAGE);
        }
            
    }
    /**
     * Funkce na ulozeni site na server
     * @param evt prace s mysi
     */
    private void jMenuItem3ActionPerformed(java.awt.event.ActionEvent evt) {
        if (name!=null && PW != null)
        {
            Vector <filePath> myPath = new Vector<filePath>();
            Vector <String> myFiles = new Vector <String>();
            System.out.println(name);
            myPath =client.loadFile(address,name,PW);
            for (filePath pom: myPath)
            {
                myFiles.add(pom.getFile());
            }
            String string = new saveFile().ShowFiles(myFiles, myPath);
            if (string!=null){
                if (string.equals("serverNets"))
                    JOptionPane.showMessageDialog(drawingPane, "You can't name your net serverNets!", "ERROR",JOptionPane.WARNING_MESSAGE);
                else{
                    client.saveServer(address,name, PW,string, Mypetri);
                    statusBar.setText("Petri Net's was saved!");
                    petriName.setText(string);
                }
            }
        }
        else
          JOptionPane.showMessageDialog(drawingPane, "You're not logged in!", "ERROR",JOptionPane.WARNING_MESSAGE);
    }
    /**
     * Funkce na spusteni simulace na serveru a ulozeni vysledku
     * @param evt prace s mysi
     */
    private void jMenuItem4ActionPerformed(java.awt.event.ActionEvent evt) {
    if (name!=null && PW != null)
        {
            Vector <filePath> myPath = new Vector<filePath>();
            Vector <String> myFiles = new Vector <String>();
            System.out.println(name);
            myPath =client.loadFile(address,name,PW);
            for (filePath pom: myPath)
            {
                myFiles.add(pom.getFile());
            }
            String string = new runPetri().ShowFiles(myFiles, myPath);
            if (string!=null){
                if (string.equals("serverNets"))
                    JOptionPane.showMessageDialog(drawingPane, "You can't name your net serverNets!", "ERROR",JOptionPane.WARNING_MESSAGE);
                else{
                    client.serverRun(address,name, PW,string, Mypetri);
                    statusBar.setText("Petri Net's was saved!");
                    petriName.setText(string);
                }
            }
        }
        else
          JOptionPane.showMessageDialog(drawingPane, "You're not logged in!", "ERROR",JOptionPane.WARNING_MESSAGE);
    }

    /**
     * Funkce na simulaci site
     * @param evt prace s mysi
     */
    protected void jMenuItem5ActionPerformed(ActionEvent evt) {
        if (name!=null&&PW!=null)
            Mypetri = client.simulate(address,name,PW,netName,Mypetri);
        else
            JOptionPane.showMessageDialog(drawingPane, "You're not logged in!", "ERROR",JOptionPane.WARNING_MESSAGE);
        drawingPane.repaint();

    }
    /**
     * Funkce na simulaci jednoho kroku site.
     * @param evt prace s mysi.
     */
    protected void jMenuItem10ActionPerformed(ActionEvent evt) {
        if (name!=null&&PW!=null)
            Mypetri = client.simulateOneStep(address,name,PW,netName,Mypetri);
        else
            JOptionPane.showMessageDialog(drawingPane, "You're not logged in!", "ERROR",JOptionPane.WARNING_MESSAGE);
        drawingPane.repaint();

    }
    /**
     * Tlacitko pro vycisteni pracovni plochy.
     * @param evt
     */
    private void jButton3MouseClicked(java.awt.event.MouseEvent evt) {
        netName = null;
        info=null;
        simulated=null;
        Mypetri.clear();
        area.width=0;
        area.height=0;
        drawingPane.setPreferredSize(area);
        drawingPane.revalidate();
        drawingPane.repaint();
    }
    /**
     * Tlacitko pro zmenu hodnot ve strazi.
     * @param evt prace s mysi
     */
    private void jButton4ActionPerformed(java.awt.event.ActionEvent evt) {
        String oldConditions = FirstTransition.getConditionsAsString();
        String oldOutputs = FirstTransition.getOutputsAsString();
        if (jTextField1!=null && jTextField2!= null && FirstTransition!=null){
            if (FirstTransition.setConditionsFromString(jTextField1.getText()) &&FirstTransition.setOutputsFromString(jTextField2.getText())){
                if ((FirstTransition.getOutputsAsString().length()*10) > (FirstTransition.getConditionsAsString().length()*10))
                    FirstTransition.setWidth((FirstTransition.getOutputsAsString().length()*10));
                else if ((FirstTransition.getConditionsAsString().length()*10)>(FirstTransition.getOutputsAsString().length()*10))
                    FirstTransition.setWidth(((FirstTransition.getConditionsAsString().length()*10)));
                FirstTransition=null;
                drawingPane.repaint();
            }
            else if (!jTextField1.getText().equals("")||!jTextField2.getText().equals(""))
            {
                if (!FirstTransition.setConditionsFromString(jTextField1.getText())){
                    FirstTransition.setConditionsFromString(oldConditions);
                    JOptionPane.showMessageDialog(drawingPane, "Wrong arguments for conditions!", "ERROR",JOptionPane.WARNING_MESSAGE);
                }
                if (!FirstTransition.setOutputsFromString(jTextField2.getText())){
                    FirstTransition.setOutputsFromString(oldOutputs);
                    JOptionPane.showMessageDialog(drawingPane, "Wrong arguments for outputs!", "ERROR",JOptionPane.WARNING_MESSAGE);
                }

            }
        }
        jTextField1.setText("");
        jTextField2.setText("");
    }
    /**
     * Funkce na pridani noveho panelu
     * @param evt prace s mysi
     */
    private void jButton5ActionPerformed(java.awt.event.ActionEvent evt) {

            tabbedPetries.add(new PetriNet());
            JLabel loading = new JLabel("loading");
            int selIndex = tabs.getSelectedIndex();
            changedFrom = selIndex;
            tabs.addTab("local_net", loading);
            selIndex = tabs.getTabCount() - 1;
            //tabs.setComponentAt(selIndex, old);
            tabs.setSelectedIndex(selIndex);
    }
    /**
     * Funkce na odebrani zalozky
     * @param evt prace s mysi
     */
    private void jButton6ActionPerformed(java.awt.event.ActionEvent evt) {
        int tabToClose = tabs.getSelectedIndex();
        if (tabToClose!=0){
            tabs.setSelectedIndex(tabToClose-1);
            tabs.remove(tabToClose);
            tabbedPetries.removeElementAt(tabToClose);
        }
    }
    private void jButton7ActionPerformed(java.awt.event.ActionEvent evt) {
        JOptionPane.showMessageDialog(drawingPane, "Vytváření síťí\n"+
"Jednotlivé sítě se zobrazují v tabech a je možné pracovat s více\n"+
"sítěmi najednou. Síť je tvořena z míst, přechodů a hran.\n"+
"Místa se vytváří pomocí tlačítka Place a kliknutím do oblasti, kde se\n"+
"má místo vytvořit. Poté vyskočí dialogové okno, kde je možné zadat\n"+
"hodnoty pro dané místo.\n"+
"Tyto hodnoty je možné později změnit Stejně tak funguje vytváření\n"+
"přechodů pomocí tlačítka Transition.\n"+
"Pokud chcete spojit nějaké místo s přechodem, tak je to možné pomocí\n"+
"tlačítka Join. Klikněte na tlačítko a klikněte na vstupní prvek (ať už\n"+
"místo nebo hranu) a na výstupní prvek.\n"+
"Vyskočí dialogové okno, které bude požadavat označení pro dané místo v\n"+
"daném přechodě a pod tímto označením potom můžete v přechodě u\n"+
"podmínek a výstupů na dané místo odkazovat.\n"+
"Automaticky se vytvoří hrana a šipka ukazuje, zda místo vstupuje (nebo\n"+
"vystupuje) do (nebo z) přechodu.\n"+
"Jakmile máte spojeno nějaké místo s přechodem a označeny hrany, tak\n"+
"můžete upravit přechod a přidat do něj podmínky nebo výstupy.\n"+
"Pokud chcete nějaké prvky upravovat, tak je nutné kliknout na tlačítko\n"+
"Place nebo Transition a kliknout na daný prvek.\n"+
"Pokud máte tedy vstupní místa do přechodu, tak můžete vytvořit\n"+
"podmínky jako x > 0 (kde x je označení hrany) a jednotlivé podmínky\n"+
"oddělit &.\n"+
"Stejně tak pokud máte výstupní místa z přechodu, tak můžete vytvořit\n"+
"výstupy typu a = x - 2, kde a je označení hrany do výstupního místa a\n"+
"x je označení hrany vstupního místa.\n"+
"Obsahy prvků je možné kdykoliv měnit, stejně tak je možné pomocí\n"+
"tlačítka delete smazat určité prvky nebo pomocí Clear.\n"+
"Ukládání síťí\n"+
"Sítě je možné ukládat jak lokálně pomocí položek Load a Save pod\n"+
"položkou file nebo po přihlášení k serveru položkami load a save pod\n"+
"položkou server.\n"+
"Simulace síťí\n"+
"Simulace síťí probíhá na serveru a operace jsou přístupné pod položkou\n"+
"server. Nejdříve je potřeba se na serveru registrovat pomocí jména a\n"+
"hesla a poté se pomocí těchto údajů přihlásit.\n"+
"Simulaci je možné provádět po jednotlivých krocích pomocí simulate one\n"+
"step nebo provést celou simulaci naráz pomocí simulate.\n"+
"Pokud jsou na serveru uložené nějaké sítě, tak je možné jich simulovat\n"+
"více najednou pomocí Run.\n", "Help",JOptionPane.INFORMATION_MESSAGE);
    }
    /**
     * Funkce pro zakliknuti tlacitka a nastaveni pracovni plochy.
     * @param evt Argument pro odchytani umistneni a akci s mysi
     */
    private void jToggleButton1ActionPerformed(java.awt.event.ActionEvent evt) {

        jTextField1.setVisible(false);
        jTextField2.setVisible(false);
        jButton4.setVisible(false);
        FirstPlace=null;
        SecondPlace = null;
        FirstTransition = null;
        SecondTransition=null;
        jTextField1.setText(null);
        jTextField2.setText(null);
        end = false;
        start = false;
        statusBar.setText("Click on work space to add Place");
        drawingPane.repaint();
    }
    /**
     * Funkce pro mazani jednotlivych polozek
     * @param evt prace s mysi
     */
    private void jToggleButton4ActionPerformed(java.awt.event.ActionEvent evt) {
        labelConditions.setVisible(false);
        labelOutputs.setVisible(false);
        jTextField1.setVisible(false);
        jTextField2.setVisible(false);
        jButton4.setVisible(false);
        FirstPlace=null;
        SecondPlace = null;
        FirstTransition = null;
        SecondTransition=null;
        jTextField1.setText(null);
        jTextField2.setText(null);
        end = false;
        start = false;
        statusBar.setText("Click on objects to delete them");
        drawingPane.repaint();
    }

    /**
     * Funkce pro zakliknuti tlacitka pro pridani strazi a nastaveni pracovni plochy
     * @param evt Aprace s mysi
     */
    private void jToggleButton2ActionPerformed(java.awt.event.ActionEvent evt) {
        labelConditions.setVisible(false);
        labelOutputs.setVisible(false);
        jTextField1.setVisible(false);
        jTextField2.setVisible(false);
        jButton4.setVisible(false);
        FirstPlace=null;
        SecondPlace = null;
        FirstTransition = null;
        SecondTransition=null;
        jTextField1.setText(null);
        jTextField2.setText(null);
        end = false;
        start = false;
        statusBar.setText("Click on work space to add transition");
        drawingPane.repaint();
    }
    /**
     * Funkce zakliknuti tlacitka pro spojovani objektu a nastaveni pracovni plochy
     * @param evt Argument pro odchytani umistneni a akci s mysi
     */
    private void jToggleButton3ActionPerformed(java.awt.event.ActionEvent evt) {
        labelConditions.setVisible(false);
        labelOutputs.setVisible(false);
        jTextField1.setVisible(false);
        jTextField2.setVisible(false);
        jButton4.setVisible(false);
        FirstPlace=null;
        SecondPlace = null;
        FirstTransition = null;
        SecondTransition=null;
        jTextField1.setText(null);
        jTextField2.setText(null);
        end = false;
        start = false;
        statusBar.setText("click on first place, and then on second one");
        drawingPane.repaint();
    }

    /**
     * Funkce pro prevod stringu na barvu
     * @param value string s barvou
     * @return barva
     */
    public static Color stringToColor(final String value) {
        if (value == null) {
          return null;
        }
        if (value.equals("None"))
            return null;
        try {
          // get color by hex or octal value
          return Color.decode(value);
        } catch (NumberFormatException nfe) {
          // if we can't decode lets try to get it by name
          try {
            // try to get a color by name using reflection
            final Field f = Color.class.getField(value);

            return (Color) f.get(null);
          } catch (Exception ce) {
            // if we can't get any color return black
            return null;
          }
        }
      }
     /**
     * Funkce na tisknuti vsech komponent grafu
     */
    public class DrawingPane extends JPanel{
        void drawArrow(Graphics g1, int x1, int y1, int x2, int y2) {
                Graphics2D g = (Graphics2D) g1.create();
                g.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
                double dx = x2 - x1, dy = y2 - y1;
                double angle = Math.atan2(dy, dx);
                int len = (int) Math.sqrt(dx*dx + dy*dy);
                AffineTransform at = AffineTransform.getTranslateInstance(x1, y1);
                at.concatenate(AffineTransform.getRotateInstance(angle));
                g.transform(at);

                // Draw horizontal arrow starting in (0, 0)
                g.drawLine(0, 0, len, 0);
                g.fillPolygon(new int[] {len-len/2, len-6-len/2, len-6-len/2, len-len/2},
                              new int[] {0, -6, 6,0}, 4);
            }
        /**
         * Funkce pro samostatne kresleni jednotlivych polozek grafu
         * @param g uchovava graficke elementy
         */
        @Override
        protected void paintComponent(Graphics g1) {
            super.paintComponent(g1);
            Graphics2D g = (Graphics2D) g1.create();
            g.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
            for (Transition transition: Mypetri.getTransitions()) {
                for (Place place: transition.getInPlaces()){
                    if (place!=null){
                        g.setColor(stringToColor(MySettings.getJoins()));
                        drawArrow(g, place.getX()+place.getWidth()/2,place.getY()+place.getHeight()/2,transition.getX()+transition.getWidth()/2,transition.getY()+transition.getHeight()/2);
                        g.setColor(stringToColor(MySettings.getFontColor()));
                        g.drawString(place.getTag(),(place.getX()+transition.getX())/2,(place.getY()+transition.getY())/2);
                    }
                }
                for (Place place: transition.getOutPlaces()){
                    if (place!=null){
                        g.setColor(stringToColor(MySettings.getJoins()));
                        drawArrow(g, transition.getX()+transition.getWidth()/2,transition.getY()+transition.getHeight()/2,place.getX()+place.getWidth()/2,place.getY()+place.getHeight()/2);
                        g.setColor(stringToColor(MySettings.getFontColor()));
                        g.drawString(place.getTag(),(place.getX()+transition.getX())/2,(place.getY()+transition.getY())/2);
                    }
                }
            }
            if (jToggleButton3.isSelected()&&FirstPlace!=null&&(EndX!=0 && EndY!=0))
            {
                g.setColor(stringToColor(MySettings.getChosen()));
                drawArrow(g, FirstPlace.getX()+FirstPlace.getWidth()/2,FirstPlace.getY()+FirstPlace.getHeight()/2,EndX,EndY);
            }
            if (jToggleButton3.isSelected()&&FirstTransition!=null&&(EndX!=0 && EndY!=0))
            {
                g.setColor(stringToColor(MySettings.getChosen()));
                drawArrow(g, FirstTransition.getX()+FirstTransition.getWidth()/2,FirstTransition.getY()+FirstTransition.getHeight()/2,EndX,EndY);
            }
            for (Place place: Mypetri.getPlaces()) {
                if (place!=null){
                    g.setColor(stringToColor(MySettings.getInnerBG()));
                    g.fillOval(place.getX(), place.getY(), place.getWidth(),place.getHeight());
                    g.setColor(stringToColor(MySettings.getOutlines()));
                    g.drawOval(place.getX(), place.getY(), place.getWidth(),place.getHeight());
                    g.setColor(stringToColor(MySettings.getFontColor()));
                    g.drawString(String.format("%s",place.getName()),place.getX()+5, place.getY()+15);
                    g.drawString(String.format("%s",place.getValuesAsString()),place.getX(), place.getY()-2);
                }

            }

            for (Transition transition: Mypetri.getTransitions()){
                if (transition!=null){
                    g.setColor(stringToColor(MySettings.getInnerBG()));
                    g.fillRect(transition.getX(), transition.getY(), transition.getWidth(),transition.getHeight());
                    g.setColor(stringToColor(MySettings.getOutlines()));
                    g.drawRect(transition.getX(), transition.getY(), transition.getWidth(),transition.getHeight());
                    g.drawLine(transition.getX()+4,transition.getY()+transition.getHeight()/2,transition.getX()+transition.getWidth()-3, transition.getY()+transition.getHeight()/2);
                    g.setColor(stringToColor(MySettings.getFontColor()));
                    g.drawString(String.format("%s",transition.getName()),transition.getX(), transition.getY()-2);
                    g.setColor(stringToColor(MySettings.getFontColor()));
                    g.drawString(String.format("%s",transition.getConditionsAsString()),transition.getX()+10, transition.getY()+transition.getHeight()/2-4);
                    g.setColor(stringToColor(MySettings.getFontColor()));
                    g.drawString(String.format("%s",transition.getOutputsAsString()), transition.getX()+10, transition.getY()+transition.getHeight()-4);
                }
            }
            if (FirstPlace!=null)
            {
                g.setColor(stringToColor(MySettings.getChosen()));
                g.drawOval(FirstPlace.getX(), FirstPlace.getY(), FirstPlace.getWidth(),FirstPlace.getHeight());
            }
            if (FirstTransition!=null)
            {
                g.setColor(stringToColor(MySettings.getChosen()));
                g.drawRect(FirstTransition.getX(), FirstTransition.getY(), FirstTransition.getWidth(),FirstTransition.getHeight());
            }
            

        }
    }
    /**
     * Funkce pro urceni, zda se na danem miste muze nachazet objekt grafu
     * @param MouseX X souradnice mysi
     * @param MouseY Y souradnice mysi
     * @return vraci true(nachazi se) false(nenachazi se objekt)
     */
    public boolean CanI (int MouseX,int MouseY)
    {
        for (Place place : Mypetri.getPlaces())
        {
            int isX=Math.abs(MouseX-place.getX());
            int isY=Math.abs(MouseY-place.getY());
            if (isX<=place.getHeight()&&isY<=place.getWidth()){
                return true;
            }

        }
        for (Transition transition :Mypetri.getTransitions())
        {
            int isX=Math.abs(MouseX-transition.getX());
            int isY=Math.abs(MouseY-transition.getY());
            if (isX<=transition.getWidth()&&isY<=transition.getHeight()){
                return true;
            }

        }
        return false;
    }

    /**
     * Funkce pro Odchyceni kliknuti na pracovni plochu a pripadneho nastaveni co a kam vytisknout
     * @param e Argument pro odchytani umistneni a akci s mysi
     */
    public void mouseReleased(MouseEvent e) {
        MoveMe = null;
        int W = 20;
        int H = 20;
        boolean changed = false;
        int x = e.getX() - W/2;
        int y = e.getY() - H/2;
        if (x < 0) x = 0;
        if (y < 0) y = 0;
        if (jToggleButton1.isSelected()){
            W = 30;
            H = 30;
            x = e.getX() - W/2;
            y = e.getY() - H/2;
            boolean position;
            position = CanI(x, y);
            if (position==false){
                Place Myrect = new Place(x, y, W, H, idPlace, "", "");
                idPlace++;
                Mypetri.addPlace(Myrect);
            }
        }
        if (jToggleButton2.isSelected()){
            W=75;
            H=40;
            x = e.getX() - W/2;
            y = e.getY() - H/2;
            boolean position;
            position = CanI(x, y);
            if (position==false){
               Transition Myrect = new Transition(x, y, W, H, idTransition);
               idTransition++;
                Mypetri.addTransition(Myrect);
            }
        }

        int this_width = (x + W + 2);
        if (this_width > area.width) {
            area.width = this_width; changed=true;
        }

        int this_height = (y + H + 2);
        if (this_height > area.height) {
            area.height = this_height; changed=true;
        }
        if (changed) {
            //Update client's preferred size because
            //the area taken up by the graphics has
            //gotten larger or smaller (if cleared).
            drawingPane.setPreferredSize(area);

            //Let the scroll pane know to update itself
            //and its scrollbars.
            drawingPane.revalidate();
        }
        drawingPane.repaint();
    }

    /**
     * Funkce pro vraceni mista, kde uzivatel klikl
     * @param MouseX X souradnice mysi
     * @param MouseY Y souradnice mysi
     * @return Vraci misto na danem kliku
     */
    private Place PlaceCo(int MouseX,int MouseY)
    {
        for (Place place:Mypetri.getPlaces())
            {
                int isX=Math.abs(MouseX-(place.getX()+place.getWidth()/2));
                int isY=Math.abs(MouseY-(place.getY()+place.getHeight()/2));
                if (isX<=place.getWidth()/2&&isY<=place.getHeight()/2){
                    return place;
                }

            }
        return null;
    }
    /**
     * Funkce pro vraceni straze, kde uzivatel klikl
     * @param MouseX X souradnice mysi
     * @param MouseY Y souradnice mysi
     * @return Vraci straz na danem kliku
     */
    private Transition TransitionCo(int MouseX,int MouseY){
            for (Transition transition: Mypetri.getTransitions())
            {
                int isX=Math.abs(MouseX-(transition.getX()+transition.getWidth()/2));
                int isY=Math.abs(MouseY-(transition.getY()+transition.getHeight()/2));
                if (isX<=transition.getWidth()/2&&isY<=transition.getHeight()/2){
                    return transition;
                }
            }
        return null;
    }

    /**
     * Funkce pro odchyceni kliku na plochu, pokud chce uzivatel spojovat, spoji dane objekty, pokud chce zmenit hodnoty, zavola funkci na jejich menu
     * @param e Argument pro odchytani umistneni a akci s mysi
     */
    public void mouseClicked(MouseEvent e){


            if (jToggleButton3.isSelected())
            {
                jTextField1.setText(null);
                jTextField2.setText(null);
                if (clicks==0){
                    FirstPlace= PlaceCo (e.getX(),e.getY());
                    FirstTransition= TransitionCo(e.getX(),e.getY());
                    clicks++;
                }else{
                    SecondPlace = PlaceCo(e.getX(),e.getY());
                    SecondTransition = TransitionCo(e.getX(),e.getY());
                    clicks--;
                }
            if ((FirstPlace!=null && SecondTransition!=null)){
                String string = (String) JOptionPane.showInputDialog("enter name of the connection,","");
                    if (string != null)
                SecondTransition.addInPlace(FirstPlace, string );
                FirstPlace = null;
                SecondTransition = null;

            }
            else if ((FirstTransition!=null && SecondPlace!=null))
            {
                String string = (String) JOptionPane.showInputDialog("enter name of the connection,","");
                if (string != null)
                    FirstTransition.addOutPlace(SecondPlace, string);
                SecondPlace = null;
                FirstTransition = null;
            }
            else if (SecondPlace!=null || SecondTransition!=null)
            {
                FirstPlace = SecondPlace;
                FirstTransition= SecondTransition;
                SecondPlace=null;
                SecondTransition = null;
                clicks=1;
            }
                if (FirstPlace!=null || FirstTransition!=null){
                drawingPane.addMouseMotionListener(new MouseAdapter(){ // add MouseListener
                        @Override
                                public void mouseMoved(MouseEvent e){
                                    EndX=e.getX();
                                    EndY=e.getY();
                                    drawingPane.repaint();
                                }
                        });
                }
            drawingPane.repaint();
        }else if (jToggleButton4.isSelected())
        {
            deleteObject(PlaceCo(e.getX(),e.getY()),TransitionCo(e.getX(),e.getY()));
        }else{
            FirstPlace=null;
            FirstTransition=null;
            SecondPlace=null;
            SecondTransition=null;
            FirstPlace= PlaceCo (e.getX(),e.getY());
            FirstTransition= TransitionCo(e.getX(),e.getY());
            changeValue();
        }
    }

    /**
     * Funkce pro zmenu hodnot daneho prvku grafu
     * @param CoX Souradnice X kliku mysi
     * @param CoY Souradnice Y kliku mysi
     */
    public void changeValue(){
        if (FirstPlace!=null){
            labelConditions.setVisible(false);
            labelOutputs.setVisible(false);
            jTextField1.setVisible(false);
            jTextField2.setVisible(false);
            jButton4.setVisible(false);
            FirstTransition=null;
            jTextField1.setText(null);
            jTextField2.setText(null);
            String string = (String) JOptionPane.showInputDialog("enter numbers for PLACE separated by ,",FirstPlace.getValuesAsString());
            String oldValue=FirstPlace.getValuesAsString();
            if (string!=null)
                if (!FirstPlace.addValues(string))
                {
                    JOptionPane.showMessageDialog(drawingPane, "Wrong arguments for outputs!", "ERROR",JOptionPane.WARNING_MESSAGE);
                    FirstPlace.addValues(oldValue);
                }
            FirstPlace=null;
        }
        else if (FirstTransition!=null)
        {
            labelConditions.setVisible(true);
            labelOutputs.setVisible(true);
            jTextField1.setVisible(true);
            jTextField2.setVisible(true);
            jButton4.setVisible(true);
            FirstPlace=null;
            jTextField1.setText(FirstTransition.getConditionsAsString());
            jTextField2.setText(FirstTransition.getOutputsAsString());
        }

        drawingPane.repaint();
    }
    /**
     * Funkce pro mazani jednotlivych polozek grafu
     * @param deletePlace mazae misto
     * @param deleteTransition mazana straz
     */
    public void deleteObject(Place deletePlace,Transition deleteTransition)
    {
        if (deletePlace!=null){
            for (Transition transition: Mypetri.getTransitions()) {
                transition.removeInPlace(deletePlace);
            }
            for (Transition transition: Mypetri.getTransitions()) {
                transition.removeOutPlace(deletePlace);
            }
            Mypetri.removePlace(deletePlace);
        }
        else if (deleteTransition!=null)
        {
            Mypetri.removeTransition(deleteTransition);
        }
        drawingPane.repaint();
    }
    public void mouseEntered(MouseEvent e){}

    /**
     * Pokud uzivatel pusti pracovni plochu s kurzorem, nastavim spoj do leveho horniho rohu, kde jej nevykresluji
     * @param e
     */
    public void mouseExited(MouseEvent e){
        EndX=0;
        EndY=0;
        drawingPane.repaint();
    }

    /**
     * Funkce pro posun objektu po plose
     * @param e Argument pro odchytani umistneni a akci s mysi
     */
    public void mousePressed(MouseEvent e){
        if (!jToggleButton3.isSelected()){
            FirstPlace=null;
            SecondPlace=null;
            FirstTransition=null;
            SecondTransition=null;
            MovePlace = PlaceCo(e.getX(), e.getY());
            MoveTransition = TransitionCo(e.getX(), e.getY());
            if (MovePlace!=null || MoveTransition!=null){
                drawingPane.addMouseMotionListener(new MouseAdapter(){ // add MouseListener
                        @Override
                                public void mouseDragged(MouseEvent e){
                                    statusBar.setText(String.format("%d %d",e.getX(),e.getY()));
                                    if (MovePlace!=null){
                                        MovePlace.setX(e.getX()-MovePlace.getWidth()/2);
                                        MovePlace.setY(e.getY()-MovePlace.getHeight()/2);}
                                    else if (MoveTransition!=null){
                                        MoveTransition.setX(e.getX()-MoveTransition.getWidth()/2);
                                        MoveTransition.setY(e.getY()-MoveTransition.getHeight()/2);
                                    }
                                    drawingPane.repaint();
                                }
                        });
            }
        }
    }

    /**
     * Vytvori a zobrazi GUI
     */
    public static void createAndShowGUI() {
        //Create and set up the window.
        JFrame frame = new JFrame("Petri's nets");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        //Create and set up the content pane.
        JComponent newContentPane = new GUI();
        newContentPane.setOpaque(true); //content panes must be opaque
        frame.setContentPane(newContentPane);

        //Display the window.
        frame.pack();
        frame.setVisible(true);
    }
    private javax.swing.JLabel petriName;
    private javax.swing.JLabel labelConditions;
    private javax.swing.JLabel labelOutputs;
    private javax.swing.JMenu jMenu1;
    private javax.swing.JMenu jMenu2;
    private javax.swing.JMenu jMenu3;
    private javax.swing.JMenuBar jMenuBar1;
    private javax.swing.JMenuItem jMenuItem1;
    private javax.swing.JMenuItem jMenuItem2;
    private javax.swing.JMenuItem jMenuItem3;
    private javax.swing.JMenuItem jMenuItem4;
    private javax.swing.JMenuItem jMenuItem5;
    private javax.swing.JMenuItem jMenuItem7;
    private javax.swing.JMenuItem jMenuItem8;
    private javax.swing.JMenuItem jMenuItem9;
    private javax.swing.JMenuItem jMenuItem10;
    private javax.swing.JMenuItem jMenuItem11;
    private javax.swing.JMenuItem jMenuItem12;
    private javax.swing.JMenuItem jMenuItem13;
    private javax.swing.JMenuItem jMenuItem14;
    private javax.swing.JMenuItem jMenuItem15;
    private javax.swing.JMenuItem jMenuItem16;
    private javax.swing.JButton jButton3;
    private javax.swing.JButton jButton5;
    private javax.swing.JButton jButton6;
    private javax.swing.JButton jButton7;
    private javax.swing.JToggleButton jToggleButton1;
    private javax.swing.JToggleButton jToggleButton2;
    private javax.swing.JToggleButton jToggleButton3;
    private javax.swing.JToggleButton jToggleButton4;
    private javax.swing.ButtonGroup objectGroup;
    private javax.swing.JToolBar jToolBar1;
    private javax.swing.JToolBar.Separator jSeparator2;
    private javax.swing.JLabel statusBar;
    private JFileChooser fc;
    private javax.swing.JTextField jTextField1;
    private javax.swing.JTextField jTextField2;
    private javax.swing.JToolBar.Separator jSeparator3;
    private javax.swing.JToolBar.Separator jSeparator4;
    private javax.swing.JToolBar.Separator jSeparator5;
    private javax.swing.JToolBar.Separator jSeparator6;
    private javax.swing.JButton jButton4;
}