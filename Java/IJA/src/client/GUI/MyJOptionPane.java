package client.GUI;

import java.awt.BorderLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JPasswordField;
import javax.swing.JTextField;

/**
 * Trida na Zobrazeni prihlasovaciho okna.
 * @author xhalak00
 * @author xzaple29
 */
class MyJOptionPane extends JOptionPane
    {
        private String login;
        private String passwd;
        /**
         * Funkce na navrat prihlasovacich dat.
         * @return pole jmena a hesla.
         */
        public String[] showInputDialog()
        {
        class GetData extends JDialog implements ActionListener
        {
            JTextField textLogin = new JTextField();
            JPasswordField texPasswd = new JPasswordField(16);
            JButton btnOK = new JButton("   OK   ");
            JButton btnCancel = new JButton("Cancel");
            String returnlogin = null;
            String returnPasswd = null;
            /**
             * Zpracovani dat
             */
            public GetData()
            {
                setModal(true);
                getContentPane().setLayout(new BorderLayout());
                setDefaultCloseOperation(DISPOSE_ON_CLOSE);
                setLocation(400,300);
                JPanel myLogin = new JPanel();
                myLogin.setLayout(new BorderLayout());
                JPanel myPasswd = new JPanel();
                myPasswd.setLayout(new BorderLayout());
                myLogin.add(new JLabel("Username"),BorderLayout.NORTH);
                myLogin.add(textLogin,BorderLayout.CENTER);
                getContentPane().add(myLogin,BorderLayout.NORTH);
                myPasswd.add(new JLabel("Password"),BorderLayout.NORTH);
                myPasswd.add(texPasswd,BorderLayout.CENTER);
                getContentPane().add(myPasswd,BorderLayout.CENTER);
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
             * Kliknuti na tlacitko a nasledne poslani jmena a hesla.
             */
            public void actionPerformed(ActionEvent ae)
            {
                if(ae.getSource() == btnOK){
                    login = textLogin.getText();
                    passwd = texPasswd.getText();
                }
                dispose();

            }
        }
        new GetData();
        return new String[] {login, passwd};
        }

    }    
