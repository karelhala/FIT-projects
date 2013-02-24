/**
 *
 * @author xhalak00
 * @author xzaple29
 */
package petrinets;

import java.util.ArrayList;

public class Place extends GraphElement{
    private ArrayList<Integer> values = new ArrayList<Integer>();
    /**
     * Kontruktor mista v grafu
     * @param x souradnice
     * @param y souradnice
     * @param width velikost
     * @param height sirka
     * @param string hodnoty
     */
    public Place (int x, int y, int width, int height, String string)
    {
        this.x=x;
        this.y=y;
        this.width=width;
        this.height=height;
        this.string = string;
    }
    /**
    * Přidá hodnotu k místu
    * @param value hodnota, např. 0,3,4,...
    */
    public void addValue(int value)
    {
        values.add(value);
    }

    /**
    * Přidá hodnoty k místu z řetězce
    * @param valuesInStr hodnoty v řetězci, např. "0,3 ,4" apod.
    */
    public void addValues(String valuesInStr)
    {
            String[] split = valuesInStr.split(",");
            for(int i = 0; i < split.length; i++)
            {
                    if(!split[i].trim().equals(""))
                    {
                            this.values.add(Integer.valueOf(split[i].trim()));
                    }
            }
    }

    /**
    * Získá z daného místa vechny hodnoty
    * @return seznam hodnot v ArrayListu
    */
    public ArrayList<Integer> getValues(){
            return values;
    }
    /**
    * Pomocí tagu získá z daného seznamu míst potřebné místo
    * @param tag pro místo (x, y, apod.)
    * @param places seznam míst v ArrayListu, který se má prohledávat
    * @return nalezené místo
    */
    public Place getPlaceWithTag(String tag, ArrayList<Place> places)
    {
/*            for(Place place: places )
            {
                    if(place.getTag().equals(tag))
                            return place;
            }*/
            return null;
    }
}
