package client;

import petrinets.PetriNet;
import client.GUI.GUI;
import com.thoughtworks.xstream.XStream;
import com.thoughtworks.xstream.io.xml.DomDriver;
/**
 *
 * @author xhalak00
 * @author xzaple29
 */
public class client {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        GUI MyGui = new GUI();
        PetriNet MyPetri = new PetriNet();
        GUI.createAndShowGUI();
    }

}
