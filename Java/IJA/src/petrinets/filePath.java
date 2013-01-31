package petrinets;

import java.util.Vector;

/**
 * Trida pro uchovani cest na serveru.
 * @author charlie
 */
public class filePath {
    private String intel=null;
    private String simulatedBy =null;
    private String file=null;
    private String lastVersion=null;
    private Vector<String> files=new Vector<String>();
    /**
     * Konstruktor
     */
    public filePath(String file,Vector<String> files,String lastVersion,String intel,String simulatedBy)
    {
        this.intel = intel;
        this.simulatedBy=simulatedBy;
        this.file=file;
        this.files = files;
        this.lastVersion=lastVersion;
    }
    /**
     * Funkce na vraceni vsech souboru na serveru
     * @return jmena souboru
     */
    public Vector<String> getFiles()
    {
        return this.files;
    }
    /**
     * jmeno souboru
     * @return jmeno souboru
     */
    public String getFile()
    {
        return this.file;
    }
    /**
     * Vrati posledni verzi
     * @return posledni verzi
     */
    public String getLast()
    {
        return this.lastVersion;
    }
    /**
     * Ziska informace o danne siti
     * @return informace danne site
     */
    public String getIntel()
    {
        return this.intel;
    }
    /**
     * Ziska informace o tom kdo a kdy simuloval.
     * @return datum a jmena simulaci.
     */
    public String getSimulated()
    {
        return this.simulatedBy;
    }
}
