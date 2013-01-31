package petrinets;

import com.thoughtworks.xstream.XStream;
import com.thoughtworks.xstream.io.xml.DomDriver;
import java.util.Vector;

/**
 *
 * @author charlie
 */
public class xmlpacket {


    private String login=null;
    private String passwd=null;
    private String task=null;
    private String path=null;
    private String info=null;
    private String lastVersion=null;
    private String[] Versions=null;
    private String PetriName=null;
    private PetriNet Mypetri=null;
    private String simulated=null;
    /**
     * Konstruktory pro kazdy packet.
     * @param login jmeno uzivatele
     * @param task ukol
     * @param path cesta k souboru na serveru
     * @param file soubor, ktery chce upravovat
     */
    public xmlpacket(String login,String task,String path,String file)
    {
        this.login = login;
    	this.task = task;
        this.lastVersion = file;
    	this.path = path;
    }
    /**
     * Konstruktor pro kazdy packet.
     * @param task ukol.
     * @param petri Ukldana sit.
     */
    public xmlpacket(String task,PetriNet petri)
    {
    	this.task = task;
    	this.Mypetri = petri;
    }
    /**
     * Konstruktor pro kazdy packet.
     * @param task ukol.
     * @param PetriName jmeno site.
     * @param petri Ukladana sit.
     */
    public xmlpacket(String task,String PetriName,PetriNet petri)
    {
    	this.task = task;
    	this.Mypetri = petri;
        this.PetriName = PetriName;
    }
    /**
     * Konstruktor pro kazdy packet.
     * @param login jmeno uzivatele
     * @param Passwd heslo uzivatele.
     * @param task ukol.
     */
    public xmlpacket(String login, String Passwd,String task)
    {
        this.login = login;
        this.passwd = Passwd;
        this.task = task;
    }

    /**
     * Konstruktor pro kazdy packet.
     * @param login jmeno uzivatele.
     * @param Passwd heslo uzivatele.
     * @param task ukol.
     * @param MyPetri sit.
     */
    public xmlpacket(String login, String Passwd,String task,PetriNet MyPetri) {
        this.Mypetri = MyPetri;
        this.login = login;
        this.passwd = Passwd;
        this.task = task;
    }
    /**
     * Konstruktor pro kazdy packet.
     * @param login jmeno uzivatele.
     * @param Passwd heslo uzivatele.
     * @param task ukol.
     * @param PetriName jmeno site.
     * @param MyPetri sit.
     */
    public xmlpacket(String login, String Passwd, String task, String PetriName, PetriNet MyPetri) {
        this.PetriName = PetriName;
        this.Mypetri = MyPetri;
        this.login = login;
        this.passwd = Passwd;
        this.task = task;
    }
    /**
     * Konstruktor pro kazdy packet.
     * @param path cesta na serveru.
     * @param containts soubory.
     * @param last posledni verze.
     */
    public xmlpacket(String path,String[] containts,String last)
    {
        this.path = path;
        this.lastVersion = last;
        this.Versions=containts;
    }

    /**
     * Funkce na vytvoreni packetu.
     * @param Myxml ukladana data.
     * @return vraci string XML packetu.
     */
    public static String createXML(xmlpacket Myxml)
    {
        XStream xstream = new XStream(new DomDriver());
        return xstream.toXML(Myxml);
    }
    /**
     * Funkce na vytvoreni packetu.
     * @param MyPath ukladana data.
     * @return vraci string XML packetu.
     */
    public static String createXML(Vector<filePath> MyPath)
    {
        XStream xstream = new XStream(new DomDriver());
        return xstream.toXML(MyPath);
    }
    /**
     * Funkce na vytvoreni packetu.
     * @param info ukladana data.
     * @return vraci string XML packetu.
     */
    public static Vector<filePath> getPaths(String info)
    {
        XStream xstream = new XStream(new DomDriver());
        return (Vector<filePath>)xstream.fromXML(info);
    }

    /**
     * Funkce na vytvoreni packetu.
     * @param task ukladana data.
     * @param petri ukladana data.
     * @return vraci string XML packetu.
     */
    public static String createXMLAsString(String task, PetriNet petri)
    {
    	xmlpacket xmlpck = new xmlpacket(task, petri);
    	XStream xstream = new XStream(new DomDriver());
        return xstream.toXML(xmlpck);
    }
    /**
     * Funkce na vytvoreni packetu.
     * @param task ukladana data.
     * @param petri ukladana data.
     * @param info ukladana data.
     * @param simulated ukladana data.
     * @return vraci string XML packetu.
     */
    public static String createXMLAsString(String task, PetriNet petri, String info, String simulated) {
        xmlpacket xmlpck = new xmlpacket(task, petri,info,simulated);
    	XStream xstream = new XStream(new DomDriver());
        return xstream.toXML(xmlpck);
    }
    /**
     * Funkce na vytvoreni packetu.
     * @param task ukladana data.
     * @param petri ukladana data.
     * @param info ukladana data.
     * @return vraci string XML packetu.
     */
    public static String createXMLAsString(String task, PetriNet petri,String info)
    {
    	xmlpacket xmlpck = new xmlpacket(task, petri,info);
    	XStream xstream = new XStream(new DomDriver());
        return xstream.toXML(xmlpck);
    }
    /**
     * Funkce na vytvoreni packetu.
     * @param Myxml ukladana data.
     * @return vraci XML packet.
     */
    public static xmlpacket getData(String Myxml)
    {
        XStream xstream = new XStream(new DomDriver());
        return (xmlpacket)xstream.fromXML(Myxml);
    }
    /**
     * Funkce na vytvoreni packetu.
     * @param MyXml ukladana data.
     * @return vraci XML packet.
     */
    public static PetriNet getPetri(String MyXml)
    {
        XStream xstream = new XStream(new DomDriver());
        return (PetriNet)xstream.fromXML(MyXml);
    }
    /**
     * Nastaveni ukolu.
     * @param string ukol.
     */
    public xmlpacket(String string) {
        this.task = string;
    }

    /**
     * Nastaveni ukolu, petriho site a informaci.
     * @param task ukol.
     * @param petri sit.
     * @param info informace.
     */
    private xmlpacket(String task, PetriNet petri, String info) {
        this.task = task;
        this.Mypetri=petri;
        this.info=info;
    }

    /**
     * Nastaveni ukolu, site, informaci a kdo simuloval.
     * @param task ukol.
     * @param petri petriho sit.
     * @param info informace.
     * @param simulated kdo simuloval.
     */
    private xmlpacket(String task, PetriNet petri, String info, String simulated) {
        this.simulated=simulated;
        this.Mypetri = petri;
        this.task=task;
        this.info=info;
    }
    /**
     * Funkce pro ziskani loginu.
     * @return login.
     */
    public String getLogin() {
        return this.login;
    }
    /**
     * Funkce pro ziskani cesty na serveru.
     * @return vraci cestu.
     */
    public String getPath() {
        return this.path;
    }
    /**
     * Funkce pro vraceni souboru.
     * @return vraci soubor.
     */
    public String getFile() {
        return this.lastVersion;
    }
    /**
     * Funkce pro ziskani hesl uzivatele.
     * @return vraci heslo.
     */
    public String getPasswd() {
        return this.passwd;
    }

    /**
     * Funkce pro ziskani ukolu.
     * @return vraci ukol.
     */
    public String getTask() {
        return this.task;
    }
    /**
     * Funkce pro ziskani jmena site.
     * @return vraci jmeno site.
     */
    public String getName() {
        return this.PetriName;
    }
    /**
     * Funkce pro ziskani site.
     * @return vraci sit.
     */
    public PetriNet getPetri() {
        return this.Mypetri;
    }
    /**
     * Funkce pro ziskani informaci.
     * @return vraci informace.
     */
    public String getInfo() {
        return this.info;
    }
    /**
     * Fukce pro ziskani, kdo simuloval.
     * @return jmena simulovani.
     */
    public String getSimulated() {
        return this.simulated;
    }
}