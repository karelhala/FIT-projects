package client.GUI;

/**
 * Trida na ulozeni nastaveni.
 * @author xhalak00
 * @author xzaple29
 */
public class settingsClass {
    String outlines = "black";
    String innerBG = "white";
    String chosen = "green";
    String fontColor = "black";
    String joins = "red";
    /**
     * Konstruktor pro vsechny barvy
     * @param outlines okolni cary
     * @param innerBG vnitrni barva
     * @param chosen vybrany objekt
     * @param fontColor barva pisma
     * @param joins spoje
     */
    public settingsClass (String outlines,String innerBG,String chosen,String fontColor,String joins)
    {
        this.outlines = outlines;
        this.innerBG =innerBG;
        this.chosen = chosen;
        this.fontColor=fontColor;
        this.joins = joins;
    }
    /**
     * Konstruktor na prazdne barvy, nastavi se implicitni.
     */
    public settingsClass (){}

    /**
     * Funkce na vraceni barvy pro okolni cary.
     * @return jmeno barvy.
     */
    public String getOutlines()
    {
        return this.outlines;
    }
    /**
     * Funkce na vraceni vyplne objektu.
     * @return jmeno barvy.
     */
    public String getInnerBG()
    {
        return this.innerBG;
    }
    /**
     * Funkce na vraceni barvy vybraneho objektu.
     * @return jmeno barvy.
     */
    public String getChosen()
    {
        return this.chosen;
    }
    /**
     * Barva fontu.
     * @return jmeno barvy.
     */
    public String getFontColor()
    {
        return this.fontColor;
    }
    /**
     * Brava spoju.
     * @return jmeno barvy.
     */
    public String getJoins()
    {
        return this.joins;
    }
    /**
     * Vsechny barvy.
     * @return jmena barev.
     */
    public String[] getAll()
    {
        return new String[] {this.outlines,this.innerBG,this.chosen,this.fontColor,this.joins};
    }
    /**
     * Nastaveni vsech barev.
     * @param saveSettings jmena barev.
     */
    public void setAll(String[] saveSettings)
    {
        this.outlines = saveSettings[0];
        this.innerBG =saveSettings[1];
        this.chosen = saveSettings[2];
        this.fontColor=saveSettings[3];
        this.joins = saveSettings[4];
    }
}
