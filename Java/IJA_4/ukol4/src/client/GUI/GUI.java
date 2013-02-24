/**
 *
 * @author xhalak00
 * @author xzaple29
 */
package client.GUI;
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
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileWriter;
import java.nio.MappedByteBuffer;
import java.nio.channels.FileChannel;
import java.nio.charset.Charset;
import java.util.*;
import petrinets.Place;
import petrinets.condition;

/**
 * Funkce obalujici kompletne cely program s GUI
 *
*/
public class GUI extends JPanel
                         implements MouseListener{
    private Dimension area;
    private PetriNet Mypetri;
    Vector<Object> vektor = new Vector<Object>();
    private int clicks=0;
    Place First = null;
    Place MoveMe = null;
    Place Second = null;
    private JPanel drawingPane;
    int StartX=0;
    int StartY=0;
    int EndX=0;
    int EndY=0;
    boolean start,end;

    private final Color colors[] = {
        Color.red, Color.blue, Color.green, Color.orange,
        Color.cyan, Color.magenta, Color.darkGray, Color.yellow};
    private final int color_n = colors.length;
    /**
     * Kontruktor pro identifikaci vsech objektu v okne
     */
    public GUI() {
        super(new BorderLayout());
        jMenuBar1 = new javax.swing.JMenuBar();
        jMenu1 = new javax.swing.JMenu();
        jMenu2 = new javax.swing.JMenu();
        jMenuItem6 = new javax.swing.JMenuItem();
        jMenu3 = new javax.swing.JMenu();
        jMenuItem1 = new javax.swing.JMenuItem();
        jMenuItem2 = new javax.swing.JMenuItem();
        jMenuItem3 = new javax.swing.JMenuItem();
        jMenuItem4 = new javax.swing.JMenuItem();
        jMenuItem5 = new javax.swing.JMenuItem();
        jToggleButton1 = new javax.swing.JToggleButton();
        jToggleButton2 = new javax.swing.JToggleButton();
        jToggleButton3 = new javax.swing.JToggleButton();
        objectGroup = new javax.swing.ButtonGroup();
        jToolBar1 = new javax.swing.JToolBar();
        jSeparator1 = new javax.swing.JToolBar.Separator();
        jButton1 = new javax.swing.JButton();
        jButton2 = new javax.swing.JButton();
        jButton3 = new javax.swing.JButton();
        jSeparator2 = new javax.swing.JToolBar.Separator();
        area = new Dimension(0,0);
        fc = new JFileChooser();
        Mypetri = new PetriNet();
        statusBar = new javax.swing.JLabel();

        //polozka v menu pro uchovani file
        jMenu1.setText("File");
        jMenuBar1.add(jMenu1);

        //polozka v menu pro uchovani Run->Run Local
        jMenu2.setText("Run");

        ///polozka v menu pro uchovani Run Local
        jMenuItem6.setText("Run Local");
        jMenu2.add(jMenuItem6);

        jMenuBar1.add(jMenu2);

        //polozka v menu pro uchovani server->connect, load, save a simulate
        jMenu3.setText("server");

        jMenuItem1.setText("connect");
        jMenu3.add(jMenuItem1);

        jMenuItem2.setText("load");
        jMenu3.add(jMenuItem2);

        jMenuItem3.setText("save");
        jMenu3.add(jMenuItem3);

        jMenuItem4.setText("run");
        jMenu3.add(jMenuItem4);

        jMenuItem5.setText("simulate");
        jMenu3.add(jMenuItem5);

        jMenuBar1.add(jMenu3);

        objectGroup.add(jToggleButton1);
        jToggleButton1.setText("Add Place");
        jToggleButton1.setFocusable(false);
        jToggleButton1.setHorizontalTextPosition(javax.swing.SwingConstants.CENTER);
        jToggleButton1.setVerticalTextPosition(javax.swing.SwingConstants.BOTTOM);
        jToggleButton1.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jToggleButton1ActionPerformed(evt);
            }
        });
        jToolBar1.add(jToggleButton1);

        objectGroup.add(jToggleButton2);
        jToggleButton2.setText("Add Transmition");
        jToggleButton2.setFocusable(false);
        jToggleButton2.setHorizontalTextPosition(javax.swing.SwingConstants.CENTER);
        jToggleButton2.setVerticalTextPosition(javax.swing.SwingConstants.BOTTOM);
        jToggleButton2.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jToggleButton2ActionPerformed(evt);
            }
        });
        jToolBar1.add(jToggleButton2);

        objectGroup.add(jToggleButton3);
        jToggleButton3.setText("Add joint");
        jToggleButton3.setFocusable(false);
        jToggleButton3.setHorizontalTextPosition(javax.swing.SwingConstants.CENTER);
        jToggleButton3.setVerticalTextPosition(javax.swing.SwingConstants.BOTTOM);
        jToggleButton3.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jToggleButton3ActionPerformed(evt);
            }
        });
        jToolBar1.add(jToggleButton3);
        jToolBar1.add(jSeparator1);

        jButton1.setText("Save");
        jButton1.setFocusable(false);
        jButton1.setHorizontalTextPosition(javax.swing.SwingConstants.CENTER);
        jButton1.setVerticalTextPosition(javax.swing.SwingConstants.BOTTOM);
        jButton1.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                try {
                    jButton1ActionPerformed(evt);
                } catch (IOException ex) {
                    Logger.getLogger(GUI.class.getName()).log(Level.SEVERE, null, ex);
                }
            }
        });
        jToolBar1.add(jButton1);

        jButton2.setText("Load");
        jButton2.setFocusable(false);
        jButton2.setHorizontalTextPosition(javax.swing.SwingConstants.CENTER);
        jButton2.setVerticalTextPosition(javax.swing.SwingConstants.BOTTOM);
        jButton2.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                try {
                    jButton2ActionPerformed(evt);
                } catch (FileNotFoundException ex) {
                    Logger.getLogger(GUI.class.getName()).log(Level.SEVERE, null, ex);
                } catch (IOException ex) {
                    Logger.getLogger(GUI.class.getName()).log(Level.SEVERE, null, ex);
                }
            }
        });
        jToolBar1.add(jButton2);
        jToolBar1.add(jSeparator2);

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

         statusBar.setText("statusbar");
        JPanel instructionPanel = new JPanel(new GridLayout(0,1));
        instructionPanel.setFocusable(true);
        instructionPanel.add(jMenuBar1);
        instructionPanel.add(jToolBar1);

        //Set up the drawing area.
        drawingPane = new DrawingPane();
        drawingPane.addMouseListener(this);
        

        //Put the drawing area in a scroll pane.
        JScrollPane scroller = new JScrollPane(drawingPane);
        scroller.setPreferredSize(new Dimension(720,640));

        //Lay out this demo.
        add(instructionPanel, BorderLayout.PAGE_START);
        add(scroller, BorderLayout.CENTER);
        add(statusBar, BorderLayout.SOUTH);
    }

    /**
     * funkce pro Vymazani cele pracovni plochy
     * @param evt Argument pro odchytani umistneni a akci s mysi
     */

    private void jButton3MouseClicked(java.awt.event.MouseEvent evt) {
        Mypetri.circles.removeAllElements();
        Mypetri.rect.removeAllElements();
        Mypetri.line.removeAllElements();
        vektor.removeAllElements();
        area.width=0;
        area.height=0;
        drawingPane.setPreferredSize(area);
        drawingPane.revalidate();
        drawingPane.repaint();
    }
    /**
     * Funkce pro odchyceni stavu tlacitak c1 a vypsani informace pro uzivatele
     * @param evt Argument pro odchytani umistneni a akci s mysi
     */
    private void jToggleButton1ActionPerformed(java.awt.event.ActionEvent evt) {
        end = false;
        start = false;
        statusBar.setText("Click on work space to add Place");
    }

    /**
     * Funkce pro odchyceni stavu tlacitak c1 a vypsani informace pro uzivatele
     * @param evt Argument pro odchytani umistneni a akci s mysi
     */
    private void jToggleButton2ActionPerformed(java.awt.event.ActionEvent evt) {
        end = false;
        start = false;
        statusBar.setText("Click on work space to add Edge");
    }
    /**
     * Funkce pro odchyceni stavu tlacitak c1 a vypsani informace pro uzivatele
     * @param evt Argument pro odchytani umistneni a akci s mysi
     */
    private void jToggleButton3ActionPerformed(java.awt.event.ActionEvent evt) {
        end = false;
        start = false;
        statusBar.setText("click on first place, and then on second one");
    }

    /**
     * Funkce pro ukladani site
     * @param evt Argument pro odchytani umistneni a akci s mysi
     * @throws IOException pracujeme se soubory!
     */
    private void jButton1ActionPerformed(java.awt.event.ActionEvent evt) throws IOException {
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
     * Funkce pro nacitani souboru a vygenerovani nactene site
     * @param evt Argument pro odchytani umistneni a akci s mysi
     * @throws FileNotFoundException Pokud se nenjde soubor
     * @throws IOException Pracujeme se soubory!
     */
    private void jButton2ActionPerformed(java.awt.event.ActionEvent evt) throws FileNotFoundException, IOException {
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
            Mypetri = (PetriNet)xstream.fromXML(Mystring);
            drawingPane.repaint();
            statusBar.setText("opening: " + file.getPath());
        } else {
            statusBar.setText("Save command cancelled by user.");
        }
    }

    /**
     * Funkce na tisknuti vsech komponent grafu
     */
    public class DrawingPane extends JPanel{
        void drawArrow(Graphics g1, int x1, int y1, int x2, int y2) {
                Graphics2D g = (Graphics2D) g1.create();

                double dx = x2 - x1, dy = y2 - y1;
                double angle = Math.atan2(dy, dx);
                int len = (int) Math.sqrt(dx*dx + dy*dy);
                AffineTransform at = AffineTransform.getTranslateInstance(x1, y1);
                at.concatenate(AffineTransform.getRotateInstance(angle));
                g.transform(at);

                // Draw horizontal arrow starting in (0, 0)
                g.drawLine(0, 0, len, 0);
                g.fillPolygon(new int[] {len, len-6, len-6, len},
                              new int[] {0, -6, 6, 0}, 4);
            }
        /**
         * Funkce pro samostatne kresleni jednotlivych polozek grafu
         * @param g uchovava graficke elementy
         */
        @Override
        protected void paintComponent(Graphics g) {
            super.paintComponent(g);

            Place Myrect = null;
            condition Line = null;
            for (int i = 0; i < Mypetri.circles.size(); i++) {
                Myrect = Mypetri.circles.elementAt(i);
                g.setColor(Color.black);
                g.drawOval(Myrect.getX(), Myrect.getY(), Myrect.getWidth(),Myrect.getHeight());
                g.drawString(String.format("p%d",i+1),Myrect.getX()+5, Myrect.getY()+15);
                g.drawString(String.format("%s",Myrect.getString()),Myrect.getX(), Myrect.getY()-2);

            }

            for (int i = 0; i < Mypetri.rect.size(); i++) {
                Myrect = Mypetri.rect.elementAt(i);
                g.setColor(Color.black);
                g.drawRect(Myrect.getX(), Myrect.getY(), Myrect.getWidth(),Myrect.getHeight());
                g.drawString(String.format("e%d",i+1),Myrect.getX()+20, Myrect.getY()+15);
                g.drawString(String.format("%s",Myrect.getString()),Myrect.getX(), Myrect.getY()-2);
            }
            for (int i = 0; i < Mypetri.line.size(); i++) {
                Line = Mypetri.line.elementAt(i);
                g.setColor(Color.blue);
                drawArrow(g, Line.getFirstPlace().getX()+15,Line.getFirstPlace().getY()+15,Line.getSecondPlace().getX()+15,Line.getSecondPlace().getY()+15);
//                g.drawLine(Line.Myfirst.x+15,Line.Myfirst.y+15,Line.Mysecond.x+15,Line.Mysecond.y+15);
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
        Place Myrect = null;
        for (int x=0;x<Mypetri.circles.size();x++)
        {
            Myrect = Mypetri.circles.elementAt(x);
            int isX=Math.abs(MouseX-Myrect.getX());
            int isY=Math.abs(MouseY-Myrect.getY());
            if (isX<=30&&isY<=30){
                return true;
            }

        }
        for (int x=0;x<Mypetri.rect.size();x++)
        {
            Myrect = Mypetri.rect.elementAt(x);
            int isX=Math.abs(MouseX-Myrect.getX());
            int isY=Math.abs(MouseY-Myrect.getY());
            if (isX<=45&&isY<=45){
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
                Place Myrect = new Place(x, y, W, H,"0");
                Mypetri.circles.addElement(Myrect);
            }
        }
        if (jToggleButton2.isSelected()){
            W=50;
            H=30;
            x = e.getX() - W/2;
            y = e.getY() - H/2;
            boolean position;
            position = CanI(x, y);
            if (position==false){
                Place Myrect = new Place(x, y, W, H,"0");
                Mypetri.rect.addElement(Myrect);
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
     * Funkce pro vraceni objektu na miste, kde uzivatel klikl
     * @param MouseX X souradnice mysi
     * @param MouseY Y souradnice mysi
     * @return Vraci objekt na danem myste
     */
    private Place coordinates(int MouseX,int MouseY)
    {
        Place Myrect = null;
        for (int x=0;x<Mypetri.circles.size();x++)
            {
                Myrect = Mypetri.circles.elementAt(x);
                int isX=Math.abs(MouseX-Myrect.getX()-15);
                int isY=Math.abs(MouseY-Myrect.getY()-15);
                if (isX<=15&&isY<=15){
                    return Myrect;
                }

            }
            for (int x=0;x<Mypetri.rect.size();x++)
            {
                Myrect = Mypetri.rect.elementAt(x);
                int isX=Math.abs(MouseX-Myrect.getX()-25);
                int isY=Math.abs(MouseY-Myrect.getY()-15);
                if (isX<=25&&isY<=15){
                    return Myrect;
                }
            }
        return Myrect;
    }

    /**
     * Funkce pro odchyceni kliku na plochu, pokud chce uzivatel spojovat, spoji dane objekty, pokud chce zmenit hodnoty, zavola funkci na jejich menu
     * @param e Argument pro odchytani umistneni a akci s mysi
     */
    public void mouseClicked(MouseEvent e){

            if (jToggleButton3.isSelected())
            {
                if (clicks==0){
                    First = coordinates (e.getX(),e.getY());
                    clicks++;
                }else{
                    Second = coordinates (e.getX(),e.getY());
                    clicks--;
                }
            if (clicks==0){
                condition SaveRect = new condition(First,"",Second);
                Mypetri.line.addElement(SaveRect);
                drawingPane.repaint();

            }
        }else
            changeValue(e.getX(),e.getY());
    }

    /**
     * Funkce pro zmenu hodnot daneho prvku grafu
     * @param CoX Souradnice X kliku mysi
     * @param CoY Souradnice Y kliku mysi
     */
    public void changeValue(int CoX,int CoY){
        Place Myrect = null;
        for (int x=0;x<Mypetri.circles.size();x++)
            {
                Myrect = Mypetri.circles.elementAt(x);
                int isX=Math.abs(CoX-Myrect.getX()-15);
                int isY=Math.abs(CoY-Myrect.getY()-15);
                if (isX<=15&&isY<=15){
                    String s = (String) JOptionPane.showInputDialog("enter numbers for PLACE separated by ,",Myrect.getString());
                    if (s!=null)
                        Myrect.setString(s);
                    break;
                }

            }
            for (int x=0;x<Mypetri.rect.size();x++)
            {
                Myrect = Mypetri.rect.elementAt(x);
                int isX=Math.abs(CoX-Myrect.getX()-25);
                int isY=Math.abs(CoY-Myrect.getY()-15);
                if (isX<=25&&isY<=15){
                    String s = (String) JOptionPane.showInputDialog("enter numbers for EDGE separated by ,",Myrect.getString());
                    if (s!=null)
                        Myrect.setString(s);
                    break;
                }
            }
        
        drawingPane.repaint();
    }

    public void mouseEntered(MouseEvent e){}
    public void mouseExited(MouseEvent e){}

    /**
     * Funkce pro posun objektu po plose
     * @param e Argument pro odchytani umistneni a akci s mysi
     */
    public void mousePressed(MouseEvent e){
        if (!jToggleButton3.isSelected()){
            MoveMe = coordinates(e.getX(), e.getY());
            if (MoveMe!=null){
                drawingPane.addMouseMotionListener(new MouseAdapter(){ // add MouseListener
                        @Override
                                public void mouseDragged(MouseEvent e){
                                if (MoveMe!=null){
                                    statusBar.setText(String.format("%d %d",e.getX(),e.getY()));
                                    MoveMe.setX(e.getX()-15);
                                    MoveMe.setY(e.getY()-15);
                                    drawingPane.repaint();}
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
    private javax.swing.JMenu jMenu1;
    private javax.swing.JMenu jMenu2;
    private javax.swing.JMenuItem jMenuItem6;
    private javax.swing.JMenu jMenu3;
    private javax.swing.JMenuBar jMenuBar1;
    private javax.swing.JMenuItem jMenuItem1;
    private javax.swing.JMenuItem jMenuItem2;
    private javax.swing.JMenuItem jMenuItem3;
    private javax.swing.JMenuItem jMenuItem4;
    private javax.swing.JMenuItem jMenuItem5;
     private javax.swing.JButton jButton1;
    private javax.swing.JButton jButton2;
    private javax.swing.JButton jButton3;
    private javax.swing.JToggleButton jToggleButton1;
    private javax.swing.JToggleButton jToggleButton2;
    private javax.swing.JToggleButton jToggleButton3;
     private javax.swing.ButtonGroup objectGroup;
     private javax.swing.JToolBar jToolBar1;
     private javax.swing.JToolBar.Separator jSeparator1;
    private javax.swing.JToolBar.Separator jSeparator2;
    private javax.swing.JLabel statusBar;
    private JFileChooser fc;
}

