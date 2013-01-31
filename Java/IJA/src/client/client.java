package client;

import java.io.IOException;

import petrinets.PetriNet;
import client.GUI.GUI;
import petrinets.xmlpacket;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.PrintStream;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.Vector;
import petrinets.filePath;
/**
 * @author libor
 * @author charlie
 */
public class client {
   static String login=null;
   static  String passwd=null;


   PetriNet MyPetri;
   private String task;
   static xmlpacket MyXml;
   static String packet;
   private static Connection conn;
    /**
     * @param args the command line arguments
     * @throws IOException
     * @throws DocumentException
     */
    public static void main(String[] args) throws IOException {
        GUI MyGui = new GUI();
        GUI.createAndShowGUI();
    }
    public static String connectToServer(String packet)
    {
        String response = "";
    	try {
    		Socket s = new Socket("localhost", 9001);
    		PrintStream out = new PrintStream(s.getOutputStream());
    		BufferedReader in = new BufferedReader(new InputStreamReader(s.getInputStream()));
    		packet = packet.replaceAll("\\s+", "");
    		out.println(packet);
    		out.flush();

    		response = in.readLine();
    		System.out.println(response);
    		out.close();
    		in.close();
    		s.close();
    	}
    	catch(UnknownHostException exp)
    	{
    		exp.printStackTrace();
    	}
    	catch(java.io.IOException exp)
    	{
    		exp.printStackTrace();
    	}
        return response;
    }
    /**
     * Metoda logIn zajistuje prihlaseni uzivatele na serveru
     * @param server adresa serveru
     * @param Name jmeno uzivatele
     * @param Passwd heslo uzivatele
     * @return true nebo false podle toho zda byl uzivatel prihlasen
     */
    public static boolean logIn(String server,String Name, String Passwd)
    {
    	String result;
    	String MyPacket = xmlpacket.createXML(new xmlpacket(Name, Passwd, "log_me_in"));
    	conn = new Connection();
    	conn.open(server, 9001);
    	result = conn.send(MyPacket);
    	conn.close();
        if (result==null)
        {
            return false;
        }
        else if(result.equals("logged"))
    	{
    		System.out.println("Nejaky messagebox na uspesne prihlaseni");
    		return true;
    	}
    	else
    	{
    		System.out.println("Nejaky messagebox na neuspesne prihlaseni");
    		return false;
    	}
        //client.packet = xmlpacket.createXML(new xmlpacket(Name, Passwd, "log_me_in"));
        //connectToServer(packet);
    }
    /**
     * Metoda zajistuje registraci uzivatele na serveru
     * @param server adresa serveru
     * @param Name jmeno
     * @param Passwd heslo
     * @return vraci true pokud byl uzivatel uspesne zaregistrovan
     */
    public static boolean register(String server,String Name, String Passwd)
    {
    	String result;
        String MyPacket = xmlpacket.createXML(new xmlpacket(Name, Passwd, "register"));
        conn = new Connection();
    	conn.open(server, 9001);
    	result = conn.send(MyPacket);
    	conn.close();
        if (result==null)
        {
            return false;
        }
        else if(result.equals("user succesfully added"))
    	{
    		System.out.println("Nejaky messagebox na uspesnou registraci");
    		return true;
    	}
    	else
    	{
    		System.out.println("Nejaky messagebox na neuspesnou registraci");
    		return false;
    	}
    }
    /**
     * Metoda slouzici k ulozeni konkretni site na server
     * @param server adresa serveru
     * @param Name jmeno uzivatele
     * @param Passwd heslo uzivatele
     * @param PetriName jmeno site
     * @param MyPetri samotna sit
     * @return true pokud se vse povedlo
     */
    public static boolean saveServer(String server,String Name, String Passwd,String PetriName, PetriNet MyPetri)
    {
    	String result="";
    	conn = new Connection();
    	conn.open(server, 9001);
    	System.out.println(conn.send(xmlpacket.createXML(new xmlpacket(Name, Passwd, "log_me_in"))));
    	conn.close();
    	conn.open(server, 9001);
    	String sendPacket = xmlpacket.createXML(new xmlpacket(Name,Passwd,"save",PetriName, MyPetri));
        result = conn.send(sendPacket);
        System.out.println(result);
    	conn.close();
        return true;
    }
    /**
     * 
     * @return
     */

    public static boolean logOut()
    {
    	if(conn != null)
    		conn.close();
    	return true;
    }

    /**
     * Metoda slouzici k simulovani petriho site na serveru.
     * @param server adresa serveru
     * @param login jmeno uzivatele
     * @param passwd heslo uzivatele
     * @param PetriName jmeno site
     * @param MyPetri samotna sit
     * @return sit po konci simulace
     */
    public static PetriNet simulate(String server,String login,String passwd,String PetriName,PetriNet MyPetri)
    {
    	PetriNet petrinet;

    	conn = new Connection();
    	conn.open(server, 9001);
    	System.out.println(conn.send(xmlpacket.createXML(new xmlpacket(login, passwd, "log_me_in"))));
    	conn.close();
    	conn.open(server, 9001);
    	String sendPacket = xmlpacket.createXML(new xmlpacket(login,passwd,"simulate",PetriName, MyPetri));
    	xmlpacket rcvPacket = xmlpacket.getData(conn.send(sendPacket));
		petrinet = rcvPacket.getPetri();
    	conn.close();
    	return petrinet;
    }

    /**
     * Metoda slouzici k simulovani jednoho kroku simulace na serveru
     * @param server adresa serveru
     * @param login jmeno uzivatele
     * @param passwd heslo uzivatele
     * @param PetriName jmeno site
     * @param MyPetri samotna sit
     * @return sit po provedeni jednoho kroku
     */
    public static PetriNet simulateOneStep(String server,String login,String passwd,String PetriName,PetriNet MyPetri)
    {
    	PetriNet petrinet;
    	conn = new Connection();
    	conn.open(server, 9001);
    	System.out.println(conn.send(xmlpacket.createXML(new xmlpacket(login, passwd, "log_me_in"))));
    	conn.close();
    	conn.open(server, 9001);
    	String sendPacket = xmlpacket.createXML(new xmlpacket(login,passwd,"simulate_one_step",PetriName, MyPetri));
    	xmlpacket rcvPacket = xmlpacket.getData(conn.send(sendPacket));
		petrinet = rcvPacket.getPetri();
    	conn.close();
    	return petrinet;
    }
    /**
     * Metoda slouzici k zobrazeni siti na serveru
     * @param server adresa serveru
     * @param login jmeno uzivatele
     * @param Passwd heslo
     * @return seznam souboru
     */
    public static Vector<filePath> loadFile(String server,String login,String Passwd) {
    	String result="";
    	conn = new Connection();
    	conn.open(server, 9001);
        System.out.println(Passwd);
        System.out.println(login);
        if (conn.send(xmlpacket.createXML(new xmlpacket(login, Passwd, "log_me_in")))==null)
            return null;
    	conn.close();
    	conn.open(server, 9001);
    	String sendPacket = xmlpacket.createXML(new xmlpacket("load"));
        result = conn.send(sendPacket);
    	conn.close();
        result= result.replaceAll("\t+", "\n");
        System.out.println(result);
        return xmlpacket.getPaths(result);
    }
    /**
     * Metoda slouzici k nacteni konkretni site ze serveru
     * @param login jmeno uzivatele
     * @param Passwd heslo uzivatele
     * @param server adresa serveru
     * @param path cesta k souboru
     * @param file soubor
     * @return obsah souboru jako xml
     */
    public static xmlpacket loadPetri (String login,String Passwd,String server, String path, String file)
    {
        conn = new Connection();
    	conn.open(server, 9001);
        if (conn.send(xmlpacket.createXML(new xmlpacket(login, Passwd, "log_me_in")))==null)
            return null;
    	conn.close();
    	conn.open(server, 9001);
    	String sendPacket = xmlpacket.createXML(new xmlpacket(login,"load_petri",path,file));
        sendPacket = sendPacket.replaceAll("\n+","");
        xmlpacket myData;
        String pom="";
        myData = xmlpacket.getData(conn.send(sendPacket));
    	conn.close();
        return myData;
    }
    /**
     * Metoda slouzici k spusteni simulace nad vice sitemi ulozenymi na serveru
     * @param server adresa serveru
     * @param login jmeno uzivatele
     * @param Passwd heslo uzivatele
     * @param string retezec
     * @param Mypetri sit
     * @return sit
     */
    public static PetriNet serverRun(String server, String login, String Passwd, String string, PetriNet Mypetri) {
        conn = new Connection();
    	conn.open(server, 9001);
        if (conn.send(xmlpacket.createXML(new xmlpacket(login, Passwd, "log_me_in")))==null)
            return null;
    	conn.close();
    	conn.open(server, 9001);
    	String sendPacket = xmlpacket.createXML(new xmlpacket(login,Passwd,"run_petri",string,Mypetri));
        sendPacket = sendPacket.replaceAll("\n+","");
        xmlpacket myData;
        System.err.println(conn.send(sendPacket));
    	conn.close();
        //System.out.println(myData.getInfo());
        return null;//myData.getPetri();
    }
}