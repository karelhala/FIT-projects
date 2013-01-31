package client.GUI;
import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JTextField;

/**
 * Trida na zobrazeni nastaveni.
 * @author xhalak00
 * @author xzaple29
 */
class showSettings extends JOptionPane
    {
        private String outlines=null;
        private String innerBG=null;
        private String chosenOut=null;
        private String fontCol=null;
        private String joinsCol=null;
        /**
         * Funkce na zobrazeni a navraceni nastaveni
         * @param oldColors obsahuje jednotlive stare barvy.
         * @return pole barev.
         */
        public String[] ShowFiles(String[] oldColors)
        {
        class GetData extends JDialog implements ActionListener
            {
            JComboBox outlinesChooser;
            JComboBox innerBackground;
            JComboBox chosenChoser;
            JComboBox fontColor;
            JComboBox joinsColor;
            JTextField myFile=new JTextField();
            JButton btnOK = new JButton("   OK   ");
            JButton btnCancel = new JButton("Cancel");
            public GetData(String[] oldColors)
            {
                String[] colors = {"red","blue","black","green","white","yellow","cyan","darkGray","gray","lightGray","magenta","orange","pink","None"};
                setModal(true);
                getContentPane().setLayout(new BorderLayout());
                getContentPane().setPreferredSize(new Dimension(320,175));
                setDefaultCloseOperation(DISPOSE_ON_CLOSE);
                setLocation(400,300);

                JPanel outlinesPanel = new JPanel();
                outlinesPanel.setLayout(new BorderLayout());
                JPanel innerPanel = new JPanel();
                innerPanel.setLayout(new BorderLayout());
                JPanel chosenPanel = new JPanel();
                chosenPanel.setLayout(new BorderLayout());
                JPanel fontPanel = new JPanel();
                fontPanel.setLayout(new BorderLayout());
                JPanel joinsPanel = new JPanel();
                joinsPanel.setLayout(new BorderLayout());
                
                JPanel firstLevel = new JPanel();
                firstLevel.setLayout(new BorderLayout());
                JPanel secondLevel = new JPanel();
                secondLevel.setLayout(new BorderLayout());
                JPanel thirdLevel = new JPanel();
                thirdLevel.setLayout(new BorderLayout());
                JPanel fourthLevel = new JPanel();
                fourthLevel.setLayout(new BorderLayout());

                outlinesChooser = new JComboBox(colors);
                if (oldColors[0]!=null)
                    outlinesChooser.setSelectedItem(oldColors[0]);
                else
                    outlinesChooser.setSelectedItem("None");

                innerBackground = new JComboBox(colors);
                if (oldColors[1]!=null)
                    innerBackground.setSelectedItem(oldColors[1]);
                else
                    innerBackground.setSelectedItem("None");

                chosenChoser = new JComboBox(colors);
                if (oldColors[2]!=null)
                    chosenChoser.setSelectedItem(oldColors[2]);
                else
                    chosenChoser.setSelectedItem("None");

                fontColor = new JComboBox(colors);
                if (oldColors[3]!=null)
                    fontColor.setSelectedItem(oldColors[3]);
                else
                    fontColor.setSelectedItem("None");

                joinsColor = new JComboBox(colors);
                if (oldColors[4]!=null)
                    joinsColor.setSelectedItem(oldColors[4]);
                else
                    joinsColor.setSelectedItem("None");

                JLabel second = new JLabel("Outlines");
                JLabel third = new JLabel("Inner Background");
                JLabel fourth = new JLabel("Chosen color");
                JLabel fifth = new JLabel("Font Color");
                JLabel sixth = new JLabel("Joins Color");
                outlinesPanel.add(second,BorderLayout.WEST);
                outlinesPanel.add(outlinesChooser,BorderLayout.EAST);
                innerPanel.add(third,BorderLayout.WEST);
                innerPanel.add(innerBackground,BorderLayout.EAST);
                chosenPanel.add(fourth,BorderLayout.WEST);
                chosenPanel.add(chosenChoser,BorderLayout.EAST);
                fontPanel.add(fifth,BorderLayout.WEST);
                fontPanel.add(fontColor,BorderLayout.EAST);
                joinsPanel.add(sixth,BorderLayout.WEST);
                joinsPanel.add(joinsColor,BorderLayout.EAST);
                
                firstLevel.add(outlinesPanel,BorderLayout.CENTER);
                secondLevel.add(firstLevel,BorderLayout.NORTH);
                secondLevel.add(innerPanel,BorderLayout.CENTER);
                thirdLevel.add(secondLevel,BorderLayout.NORTH);
                thirdLevel.add(chosenPanel,BorderLayout.CENTER);
                fourthLevel.add(thirdLevel,BorderLayout.NORTH);
                fourthLevel.add(fontPanel,BorderLayout.CENTER);
                fourthLevel.add(joinsPanel,BorderLayout.SOUTH);

                getContentPane().add(fourthLevel,BorderLayout.NORTH);

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
             * Funkce na odchyceni zmacknuti tlacitka a vraceni hodnot.
             */
            public void actionPerformed(ActionEvent e)
            {
                if (e.getSource() == btnOK){
                    outlines = (String)outlinesChooser.getSelectedItem();
                    innerBG = (String)innerBackground.getSelectedItem();
                    chosenOut = (String)chosenChoser.getSelectedItem();
                    fontCol = (String)fontColor.getSelectedItem();
                    joinsCol = (String)joinsColor.getSelectedItem();
                    dispose();
                }
                else if (e.getSource() == btnCancel)
                    dispose();

            }
        }
        new GetData(oldColors);
        return new String[] {outlines,innerBG,chosenOut,fontCol,joinsCol};
        }
}
